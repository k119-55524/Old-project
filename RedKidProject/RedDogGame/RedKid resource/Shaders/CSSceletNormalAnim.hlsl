
#include "CS header.hlsl"
#include "HLSL header.hlsl"

StructuredBuffer<CS_INPUT_NORMAL_BONE> InBuff : register( t0 );

RWStructuredBuffer<VS_INPUT_NOR_UV> OutBuff : register( u0 );
RWStructuredBuffer<int3> OutAABBBuff : register( u1 );

groupshared int3 MinAABB;
groupshared int3 MaxAABB;

[numthreads( 32, 1, 1)]
void CSSceletNormalAnim( uint3 DTid : SV_DispatchThreadID, uint3 GroupThreadID : SV_GroupThreadID )
{
	uint count, strige;
	InBuff.GetDimensions( count, strige );

	if ( DTid.x >= count )
		DTid = 0;

	CS_INPUT_NORMAL_BONE vIn = InBuff[DTid.x];
	VS_INPUT_NOR_UV vOut;

	GroupMemoryBarrierWithGroupSync();

	BONE_ANIMATION boneTransform;
	float  w;
	float3 bontr;
	float3 nrtr;

	boneTransform = BoneAnimData[vIn.Weigth[0].BoneID];
	w = vIn.Weigth[0].Weigth;
	MulVec3BoneTrans( vIn.Pos, bontr, vIn.Nor, nrtr, boneTransform );
	bontr *= w;
	nrtr *= w;

	vOut.Pos = bontr;
	vOut.Nor = nrtr;

	for ( uint j = 1; j < vIn.NumWeigth; j++ )
	{
		boneTransform = BoneAnimData[vIn.Weigth[j].BoneID];
		w = vIn.Weigth[j].Weigth;
		MulVec3BoneTrans( vIn.Pos, bontr, vIn.Nor, nrtr, boneTransform );
		bontr *= w;
		nrtr *= w;

		vOut.Pos += bontr;
		vOut.Nor += nrtr;
	}

	vOut.Pos = mul( mObjWorld, float4( vOut.Pos, 1.0f ) ).xyz;
	vOut.UV = vIn.UV;

	///////////////////////////////
	// AABB
	int3 Pos = vOut.Pos * 100000.0f;

	GroupMemoryBarrierWithGroupSync();
	if ( GroupThreadID.x == 0 )
		MinAABB = MaxAABB = Pos;

	GroupMemoryBarrierWithGroupSync();
	InterlockedMin( MinAABB.x, Pos.x );
	InterlockedMin( MinAABB.y, Pos.y );
	InterlockedMin( MinAABB.z, Pos.z );

	GroupMemoryBarrierWithGroupSync();
	InterlockedMax( MaxAABB.x, Pos.x );
	InterlockedMax( MaxAABB.y, Pos.y );
	InterlockedMax( MaxAABB.z, Pos.z );

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
	/////////////////////////////////

	GroupMemoryBarrierWithGroupSync();
	
	OutBuff[DTid.x] = vOut;
}

// Копирование первой точки для просчёта AABB
[numthreads( 1, 1, 1 )]
void CS_CopyCalcAABB_Scelet()
{
	CS_INPUT_NORMAL_BONE vIn = InBuff[0];
	VS_INPUT_NOR_UV vOut;

	BONE_ANIMATION boneTransform;
	float  w;
	float3 bontr;
	float3 nrtr;

	boneTransform = BoneAnimData[vIn.Weigth[0].BoneID];
	w = vIn.Weigth[0].Weigth;
	MulVec3BoneTrans( vIn.Pos, bontr, vIn.Nor, nrtr, boneTransform );
	bontr *= w;
	nrtr *= w;

	vOut.Pos = bontr;
	vOut.Nor = nrtr;

	for ( uint j = 1; j < vIn.NumWeigth; j++ )
	{
		boneTransform = BoneAnimData[vIn.Weigth[j].BoneID];
		w = vIn.Weigth[j].Weigth;
		MulVec3BoneTrans( vIn.Pos, bontr, vIn.Nor, nrtr, boneTransform );
		bontr *= w;
		nrtr *= w;

		vOut.Pos += bontr;
		vOut.Nor += nrtr;
	}

	OutAABBBuff[0] = OutAABBBuff[1] = ( mul( mObjWorld, float4( vOut.Pos, 1.0f ) ) * 100000.0f ).xyz;
}