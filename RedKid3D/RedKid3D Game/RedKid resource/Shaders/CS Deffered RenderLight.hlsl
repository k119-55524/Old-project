
struct CS_LIGHT_DIRECTION
{
	float4 Color;
	float4 Direction;
};

struct CS_LIGHT_POINT
{
	float3 Pos;
	float  Rad;
	float4 Color;
};

Texture2D	g_DiffuseTexture		: register( t0 );	// Диффузная текстура объектов
Texture2D	g_NormalTexture			: register( t1 );	// Текстура нормалей
Texture2D	g_MaterialTexture		: register( t2 );	// Текстура нормалей

RWTexture2D<float4> g_OutTexture	: register( u0 );
RWStructuredBuffer<float> SummColor : register( u1 );
RWTexture2D<float4> g_BloomTexture	: register( u2 );

cbuffer DataBuffer : register( b0 )
{
	matrix	mVPInv;	 
	float1	Width;
	float1	Height;
	uint1	LightPointCount;
	uint1	LightDirectionCount;
	uint1	XBlockCount;
}

#define MaxLight	100
#define BlockSize	32
#define BloomText	8

static const float4 LUM_VECTOR = float4( 0.299f, 0.587f, 0.114f, 0.0f );

// Массив в shared памяти для подсчёта суммы
groupshared float	accum[BlockSize * BlockSize];
groupshared	uint3	summBloom[ ( BlockSize / BloomText ) ][ ( BlockSize / BloomText ) ];

cbuffer LightPointDataBuffer : register( b1 )
{
	CS_LIGHT_POINT LightPointData[MaxLight];
}

cbuffer LightDirectionDataBuffer : register( b2 )
{
	CS_LIGHT_DIRECTION LightDirectionData[MaxLight];
}

[numthreads( BlockSize, BlockSize, 1 )]
void CS_RenderLight( uint3 DispatchThreadID : SV_DispatchThreadID, uint3 GroupThreadID : SV_GroupThreadID, uint3 GroupID : SV_GroupID )//uint3 DTid : SV_DispatchThreadID )
{
	float4 Color = g_DiffuseTexture[DispatchThreadID.xy];

	float3 Position;
	Position.x = Color.w;
	Color.w = 1.0f;

	GroupMemoryBarrierWithGroupSync();
	float4 nor = g_NormalTexture[DispatchThreadID.xy];
	Position.yz = nor.zw;

	GroupMemoryBarrierWithGroupSync();
	float Material = g_MaterialTexture[DispatchThreadID.xy];
	if ( Material != 0 )
	{
		// UnPack Normal
		float3 Normal;
		float s = 1.0f;

		if ( nor.y >= 1.0f )
		{
			nor.y -= 1.0f;
			s = -1.0f;
		}
		Normal.x = 2.0f * nor.x - 1.0f;
		Normal.y = 2.0f * nor.y - 1.0f;
		Normal.z = s * sqrt( 1.0f - Normal.x * Normal.x - Normal.y * Normal.y );
		Normal = normalize( Normal );

		if ( LightPointCount > 0 )
		{
			for ( uint i = 0; i < LightPointCount; i++ )
			{
				float Dist = distance( Position, LightPointData[i].Pos );

				if ( Dist < LightPointData[i].Rad  )
				{
					float3 PixelToLight = LightPointData[i].Pos - Position;
					float3 NewDirection = normalize( PixelToLight );
					float4 LightColor = float4( LightPointData[i].Color.xyz * LightPointData[i].Color.w, 1.0f );
					float4 LightIntensity = ( LightColor * LightPointData[i].Rad ) / pow( length( PixelToLight ), 2.0f );
					float4 FinalColor = max( LightIntensity * dot( Normal, NewDirection ), 0.0f );

					Color += FinalColor;
				}
			}
		}

		if ( LightDirectionCount > 0 )
		{
			for ( uint i = 0; i < LightDirectionCount; i++ )
			{
				float3 LightColor = LightDirectionData[i].Color.xyz * LightDirectionData[i].Color.w;
				float4 FinalColor = float4( max( LightColor * dot( normalize( LightDirectionData[i].Direction ), Normal ), 0.0f ), 1.0f );

				Color += FinalColor;
			}
		}

	}

	GroupMemoryBarrierWithGroupSync();
	g_OutTexture[DispatchThreadID.xy] = Color;

	//////////////////////////////////////////////////////////////////////////////////////
	// Bloom summ
	uint blX = floor( GroupThreadID.x / BloomText );
	uint blY = floor( GroupThreadID.y / BloomText );
	bool fOutBloom = false;

	GroupMemoryBarrierWithGroupSync();
	if ( ( (float)GroupThreadID.x / (float)BloomText == (float)blX ) ||
		 ( (float)GroupThreadID.y / (float)BloomText == (float)blY ) )
	{
		summBloom[blX][blY] = uint3( 0.0f, 0.0f, 0.0f );
		fOutBloom = true;
	}

	GroupMemoryBarrierWithGroupSync();
	InterlockedAdd( summBloom[blX][blY].x, uint( Color.x * 100000 ), summBloom[blX][blY].x );
	GroupMemoryBarrierWithGroupSync();
	InterlockedAdd( summBloom[blX][blY].y, uint( Color.y * 100000 ), summBloom[blX][blY].y );
	GroupMemoryBarrierWithGroupSync();
	InterlockedAdd( summBloom[blX][blY].z, uint( Color.z * 100000 ), summBloom[blX][blY].z );
	//
	//////////////////////////////////////////////////////////////////////////////////////
	// Calc HDR
	// Вычисляем индекс для сохранения в accum
	uint tid = GroupThreadID.y * BlockSize + GroupThreadID.x;

	accum[tid] = dot( Color.xyz, LUM_VECTOR );

	GroupMemoryBarrierWithGroupSync();
    if ( tid < 16 )
		accum[tid] += accum[tid + 16];

	GroupMemoryBarrierWithGroupSync();
    if ( tid < 8 )
		accum[tid] += accum[tid + 8];

	GroupMemoryBarrierWithGroupSync();
    if ( tid < 4 )
		accum[tid] += accum[tid + 4];

	GroupMemoryBarrierWithGroupSync();
    if ( tid < 2 )
		accum[tid] += accum[tid + 2];

	GroupMemoryBarrierWithGroupSync();
    if ( tid < 1 )
		accum[tid] += accum[tid + 1];

	if ( tid == 0 )
		SummColor[XBlockCount * GroupID.y + GroupID.x] = accum[0];

	// Out bloom texture
	if ( fOutBloom )
	{
		uint2 bltXY;
		bltXY.x= (uint)floor( DispatchThreadID.x / BloomText );
		bltXY.y = (uint)floor( DispatchThreadID.y / BloomText );
		float4 blCol = float4( ( (float3)summBloom[blX][blY] / (float)( BloomText * BloomText ) ) / 100000.0f, 1.0f );
		
		if ( length( blCol.xyz > 0.85f ) )
		//if ( blCol.x > 0.85f || blCol.y > 0.85f || blCol.z > 0.85f )
			g_BloomTexture[bltXY] = blCol;
		else
			g_BloomTexture[bltXY] = 0.0f;
	}
}