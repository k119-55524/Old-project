
#include "CS header.hlsl"
#include "HLSL header.hlsl"

StructuredBuffer<CS_INPUT_TBN_BONE> InBuff : register( t0 );

RWStructuredBuffer<VS_INPUT_TBN_UV> OutBuff : register( u0 );

[numthreads( 32, 1, 1)]
void CSSceletTBNCopy( uint3 DTid : SV_DispatchThreadID )
{
	CS_INPUT_TBN_BONE vIn = InBuff[DTid.x];
	VS_INPUT_TBN_UV vOut;

	GroupMemoryBarrierWithGroupSync();

	vOut.Pos = mul( mObjWorld, float4( vIn.Pos, 1.0f ) ).xyz;
	vOut.Nor = mul( mObjWorld, vIn.Nor ).xyz;
	vOut.Tan = mul( mObjWorld, vIn.Tan ).xyz;
	vOut.Bin = mul( mObjWorld, vIn.Bin ).xyz;
	vOut.UV = vIn.UV;

	GroupMemoryBarrierWithGroupSync();

	uint count, stride;
	OutBuff.GetDimensions( count, stride );
	if ( DTid.x < count )
		OutBuff[DTid.x] = vOut;
}