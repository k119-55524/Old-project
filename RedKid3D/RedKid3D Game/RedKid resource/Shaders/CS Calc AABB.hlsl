
#include "CS header.hlsl"
#include "HLSL header.hlsl"

//ByteAddressBuffer InAABBBuff : register( t0 );
StructuredBuffer<VS_INPUT_NOR_UV> InBuff : register( t0 );

// Буффер MinMax AABB
RWStructuredBuffer<int3> OutAABBBuff : register( u0 );

#define BlockSize	128

groupshared int3 MinAABB[BlockSize];
groupshared int3 MaxAABB[BlockSize]; 

// Просчёт AABB
[numthreads( BlockSize, 1, 1 )]
void CS_CalcAABB( uint DTid : SV_DispatchThreadID, uint GroupThreadID : SV_GroupThreadID )
{
	float3 fPos;
	int3 Pos;

	fPos = InBuff[ DTid.x ].Pos;
	Pos = ( mul( mObjWorld, float4( fPos, 1.0f ) ) * 100000.0f ).xyz;

	MinAABB[GroupThreadID.x] = Pos;
	MaxAABB[GroupThreadID.x] = Pos;

	//GroupMemoryBarrierWithGroupSync();
	//if ( GroupThreadID < 128 )
	//{
	//	MinAABB[GroupThreadID].x = min( MinAABB[GroupThreadID].x, MinAABB[GroupThreadID + 128].x );
	//	MinAABB[GroupThreadID].y = min( MinAABB[GroupThreadID].y, MinAABB[GroupThreadID + 128].y );
	//	MinAABB[GroupThreadID].z = min( MinAABB[GroupThreadID].z, MinAABB[GroupThreadID + 128].z );

	//	MaxAABB[GroupThreadID.x].x = max( MaxAABB[GroupThreadID].x, MaxAABB[GroupThreadID.x + 128].x );
	//	MaxAABB[GroupThreadID.x].y = max( MaxAABB[GroupThreadID].y, MaxAABB[GroupThreadID.x + 128].y );
	//	MaxAABB[GroupThreadID.x].z = max( MaxAABB[GroupThreadID].z, MaxAABB[GroupThreadID.x + 128].z );
	//}

	GroupMemoryBarrierWithGroupSync();
	if ( GroupThreadID < 64 )
	{
		MinAABB[GroupThreadID].x = min( MinAABB[GroupThreadID].x, MinAABB[GroupThreadID + 64].x );
		MinAABB[GroupThreadID].y = min( MinAABB[GroupThreadID].y, MinAABB[GroupThreadID + 64].y );
		MinAABB[GroupThreadID].z = min( MinAABB[GroupThreadID].z, MinAABB[GroupThreadID + 64].z );

		MaxAABB[GroupThreadID.x].x = max( MaxAABB[GroupThreadID].x, MaxAABB[GroupThreadID.x + 64].x );
		MaxAABB[GroupThreadID.x].y = max( MaxAABB[GroupThreadID].y, MaxAABB[GroupThreadID.x + 64].y );
		MaxAABB[GroupThreadID.x].z = max( MaxAABB[GroupThreadID].z, MaxAABB[GroupThreadID.x + 64].z );
	}

	GroupMemoryBarrierWithGroupSync();
	if ( GroupThreadID < 32 )
	{
		MinAABB[GroupThreadID].x = min( MinAABB[GroupThreadID].x, MinAABB[GroupThreadID + 32].x );
		MinAABB[GroupThreadID].y = min( MinAABB[GroupThreadID].y, MinAABB[GroupThreadID + 32].y );
		MinAABB[GroupThreadID].z = min( MinAABB[GroupThreadID].z, MinAABB[GroupThreadID + 32].z );

		MaxAABB[GroupThreadID.x].x = max( MaxAABB[GroupThreadID].x, MaxAABB[GroupThreadID.x + 32].x );
		MaxAABB[GroupThreadID.x].y = max( MaxAABB[GroupThreadID].y, MaxAABB[GroupThreadID.x + 32].y );
		MaxAABB[GroupThreadID.x].z = max( MaxAABB[GroupThreadID].z, MaxAABB[GroupThreadID.x + 32].z );
	}

	GroupMemoryBarrierWithGroupSync();
	if ( GroupThreadID < 16 )
	{
		MinAABB[GroupThreadID].x = min( MinAABB[GroupThreadID].x, MinAABB[GroupThreadID + 16].x );
		MinAABB[GroupThreadID].y = min( MinAABB[GroupThreadID].y, MinAABB[GroupThreadID + 16].y );
		MinAABB[GroupThreadID].z = min( MinAABB[GroupThreadID].z, MinAABB[GroupThreadID + 16].z );

		MaxAABB[GroupThreadID.x].x = max( MaxAABB[GroupThreadID].x, MaxAABB[GroupThreadID.x + 16].x );
		MaxAABB[GroupThreadID.x].y = max( MaxAABB[GroupThreadID].y, MaxAABB[GroupThreadID.x + 16].y );
		MaxAABB[GroupThreadID.x].z = max( MaxAABB[GroupThreadID].z, MaxAABB[GroupThreadID.x + 16].z );
	}

	GroupMemoryBarrierWithGroupSync();
	if ( GroupThreadID < 8 )
	{
		MinAABB[GroupThreadID].x = min( MinAABB[GroupThreadID].x, MinAABB[GroupThreadID + 8].x );
		MinAABB[GroupThreadID].y = min( MinAABB[GroupThreadID].y, MinAABB[GroupThreadID + 8].y );
		MinAABB[GroupThreadID].z = min( MinAABB[GroupThreadID].z, MinAABB[GroupThreadID + 8].z );

		MaxAABB[GroupThreadID.x].x = max( MaxAABB[GroupThreadID].x, MaxAABB[GroupThreadID.x + 8].x );
		MaxAABB[GroupThreadID.x].y = max( MaxAABB[GroupThreadID].y, MaxAABB[GroupThreadID.x + 8].y );
		MaxAABB[GroupThreadID.x].z = max( MaxAABB[GroupThreadID].z, MaxAABB[GroupThreadID.x + 8].z );
	}

	GroupMemoryBarrierWithGroupSync();
	if ( GroupThreadID < 4 )
	{
		MinAABB[GroupThreadID].x = min( MinAABB[GroupThreadID].x, MinAABB[GroupThreadID + 4].x );
		MinAABB[GroupThreadID].y = min( MinAABB[GroupThreadID].y, MinAABB[GroupThreadID + 4].y );
		MinAABB[GroupThreadID].z = min( MinAABB[GroupThreadID].z, MinAABB[GroupThreadID + 4].z );

		MaxAABB[GroupThreadID.x].x = max( MaxAABB[GroupThreadID].x, MaxAABB[GroupThreadID.x + 4].x );
		MaxAABB[GroupThreadID.x].y = max( MaxAABB[GroupThreadID].y, MaxAABB[GroupThreadID.x + 4].y );
		MaxAABB[GroupThreadID.x].z = max( MaxAABB[GroupThreadID].z, MaxAABB[GroupThreadID.x + 4].z );
	}

	GroupMemoryBarrierWithGroupSync();
	if ( GroupThreadID < 2 )
	{
		MinAABB[GroupThreadID].x = min( MinAABB[GroupThreadID].x, MinAABB[GroupThreadID + 2].x );
		MinAABB[GroupThreadID].y = min( MinAABB[GroupThreadID].y, MinAABB[GroupThreadID + 2].y );
		MinAABB[GroupThreadID].z = min( MinAABB[GroupThreadID].z, MinAABB[GroupThreadID + 2].z );

		MaxAABB[GroupThreadID.x].x = max( MaxAABB[GroupThreadID].x, MaxAABB[GroupThreadID.x + 2].x );
		MaxAABB[GroupThreadID.x].y = max( MaxAABB[GroupThreadID].y, MaxAABB[GroupThreadID.x + 2].y );
		MaxAABB[GroupThreadID.x].z = max( MaxAABB[GroupThreadID].z, MaxAABB[GroupThreadID.x + 2].z );
	}

	GroupMemoryBarrierWithGroupSync();
	if ( GroupThreadID < 1 )
	{
		MinAABB[GroupThreadID].x = min( MinAABB[GroupThreadID].x, MinAABB[GroupThreadID + 1].x );
		MinAABB[GroupThreadID].y = min( MinAABB[GroupThreadID].y, MinAABB[GroupThreadID + 1].y );
		MinAABB[GroupThreadID].z = min( MinAABB[GroupThreadID].z, MinAABB[GroupThreadID + 1].z );

		MaxAABB[GroupThreadID.x].x = max( MaxAABB[GroupThreadID].x, MaxAABB[GroupThreadID.x + 1].x );
		MaxAABB[GroupThreadID.x].y = max( MaxAABB[GroupThreadID].y, MaxAABB[GroupThreadID.x + 1].y );
		MaxAABB[GroupThreadID.x].z = max( MaxAABB[GroupThreadID].z, MaxAABB[GroupThreadID.x + 1].z );
	}

	GroupMemoryBarrierWithGroupSync();
	if ( GroupThreadID == 0 )
	{
		InterlockedMin( OutAABBBuff[0].x, MinAABB[0].x );
		InterlockedMin( OutAABBBuff[0].y, MinAABB[0].y );
		InterlockedMin( OutAABBBuff[0].z, MinAABB[0].z );

		InterlockedMax( OutAABBBuff[1].x, MaxAABB[0].x );
		InterlockedMax( OutAABBBuff[1].y, MaxAABB[0].y );
		InterlockedMax( OutAABBBuff[1].z, MaxAABB[0].z );
	}
}

// Копирование первой точки для просчёта AABB
[numthreads( 1, 1, 1 )]
void CS_CopyCalcAABB()
{
	float3 fPos = InBuff[0].Pos;
	OutAABBBuff[0] = OutAABBBuff[1] = ( mul( mObjWorld, float4( fPos, 1.0f ) ) * 100000.0f ).xyz;
}