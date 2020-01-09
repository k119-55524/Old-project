
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

cbuffer DataBuffer : register( b0 )
{
	matrix	mVPInv;	 
	float1	Width;
	float1	Height;
	uint1	LightPointCount;
	uint1	LightDirectionCount;
}

#define MaxLight 100
cbuffer LightPointDataBuffer : register( b1 )
{
	CS_LIGHT_POINT LightPointData[MaxLight];
}

cbuffer LightDirectionDataBuffer : register( b2 )
{
	CS_LIGHT_DIRECTION LightDirectionData[MaxLight];
}

PS_INPUT_QUAD VS_QuadLight( VS_INPUT_QUAD In )
{
	PS_INPUT_QUAD Out;

	Out.Pos = float4( In.Pos, 1.0f );
	Out.UV = In.UV;

	return Out;
}

float4 PS_QuadLight( PS_INPUT_QUAD In ) : SV_Target
{
	float4 Color = g_DiffuseTexture.SampleLevel( Point_Sampler, In.UV, 0 );

	float Material = g_MaterialTexture.SampleLevel( Point_Sampler, In.UV, 0 );
	if ( Material == 0 )
		return Color;

	float3 Position;
	Position.x = Color.w;
	Color.xyz = 0.0f;
	Color.w = 1.0f;

	float4 nor = g_NormalTexture.SampleLevel( Point_Sampler, In.UV, 0 );
	Position.yz = nor.zw;

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
				float4 FinalColor = saturate( LightIntensity * dot( Normal, NewDirection ) );

				Color += FinalColor;
			}
		}
	}

	if ( LightDirectionCount > 0 )
	{
		for ( uint i = 0; i < LightDirectionCount; i++ )
		{
			float3 LightColor = LightDirectionData[i].Color.xyz * LightDirectionData[i].Color.w;
			float4 FinalColor = float4( saturate( LightColor * dot( LightDirectionData[i].Direction, Normal ) ), 1.0f );

			Color += FinalColor;
		}
	}

	return Color;
}

technique11 RenderLight
{
	pass P0
	{
		SetVertexShader( CompileShader ( vs_5_0, VS_QuadLight() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader ( ps_5_0, PS_QuadLight() ) );

		SetDepthStencilState( DisableDepth, 1 );
	}
}


// Красная полоса
//float e = ( 1.0 - LightPointData[i].Rad / Dist ) * LightPointData[i].Rad;
//float4 LightIntensity = float4( LightPointData[i].Color, 1.0f ) / pow( e, 2.0f );
//float4 FinalColor = saturate( LightIntensity * dot( Normal, NewDirection ) );