
#include "CS header.hlsl"
#include "HLSL header.hlsl"

StructuredBuffer<CS_INPUT_NORMAL_BONE> InBuff : register( t0 );

RWStructuredBuffer<VS_INPUT_NOR_UV> OutBuff : register( u0 );
RWStructuredBuffer<int3> OutAABBBuff : register( u1 );

groupshared int3 MinAABB;
groupshared int3 MaxAABB;

[numthreads( 32, 1, 1)]
void CSSceletNormalCopy( uint3 DTid : SV_DispatchThreadID, uint3 GroupThreadID : SV_GroupThreadID )
{
	uint count, strige;
	InBuff.GetDimensions( count, strige );

	if ( DTid.x >= count )
		DTid = 0;

	CS_INPUT_NORMAL_BONE vIn = InBuff[DTid.x];
	VS_INPUT_NOR_UV vOut;

	GroupMemoryBarrierWithGroupSync();

	vOut.Pos = mul( mObjWorld, float4( vIn.Pos, 1.0f ) ).xyz;
	vOut.Nor = mul( mObjWorld, vIn.Nor ).xyz;
	vOut.UV = vIn.UV;

	///////////////////////////////
	// AABB
	int3 Pos = vOut.Pos * 100000.0f;

	GroupMemoryBarrierWithGroupSync();
	if ( GroupThreadID.x == 0 )
	{
		MinAABB.x = MaxAABB.x = Pos.x;
		MinAABB.y = MaxAABB.y = Pos.y;
		MinAABB.z = MaxAABB.z = Pos.z;
	}

	GroupMemoryBarrierWithGroupSync();

	InterlockedMin( MinAABB.x, Pos.x );
	InterlockedMin( MinAABB.y, Pos.y );
	InterlockedMin( MinAABB.z, Pos.z );

	InterlockedMax( MaxAABB.x, Pos.x );
	InterlockedMax( MaxAABB.y, Pos.y );
	InterlockedMax( MaxAABB.z, Pos.z );

	GroupMemoryBarrierWithGroupSync();

	if ( GroupThreadID.x == 0 )
	{
		InterlockedMin( OutAABBBuff[0].x, MinAABB.x );
		InterlockedMin( OutAABBBuff[0].y, MinAABB.y );
		InterlockedMin( OutAABBBuff[0].z, MinAABB.z );

		InterlockedMax( OutAABBBuff[1].x, MaxAABB.x );
		InterlockedMax( OutAABBBuff[1].y, MaxAABB.y );
		InterlockedMax( OutAABBBuff[1].z, MaxAABB.z );
	}
	// AABB
	///////////////////////////////

	GroupMemoryBarrierWithGroupSync();

	OutBuff[DTid.x] = vOut;
}