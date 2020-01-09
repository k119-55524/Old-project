
#define MAX_WEIGHTS	4
#define MAX_BONE_IN_OBJECT 25

cbuffer CB_SceletAnimMatrix : register( b0 )
{
	matrix mObjWorld;
}

struct BONE_ANIMATION
{
	float4 Rotation;
	float4 Offsets;
};

cbuffer CB_SceletAnimData : register( b1 )
{
	BONE_ANIMATION BoneAnimData[MAX_BONE_IN_OBJECT];
}

struct VERTEXWEIGHT
{
	uint  BoneID;
	float Weigth;
};

struct CS_INPUT_NORMAL_BONE
{
	float3 Pos;
	float3 Nor;
	float2 UV;
	uint NumWeigth;
	VERTEXWEIGHT Weigth[MAX_WEIGHTS];
};

struct CS_INPUT_TBN_BONE
{
	float3 Pos;
	float3 Nor;
	float3 Tan;
	float3 Bin;
	float2 UV;
	uint NumWeigth;
	VERTEXWEIGHT Weigth[MAX_WEIGHTS];
};

inline void MulVec3BoneTrans( float3 v, out float3 retv, float3 n, out float3 retn, BONE_ANIMATION bt )
{
	float xxzz = bt.Rotation.x * bt.Rotation.x - bt.Rotation.z * bt.Rotation.z;
	float wwyy = bt.Rotation.w * bt.Rotation.w - bt.Rotation.y * bt.Rotation.y;
	float xw2  = bt.Rotation.x * bt.Rotation.w * 2.0f;
	float xy2  = bt.Rotation.x * bt.Rotation.y * 2.0f;
	float xz2  = bt.Rotation.x * bt.Rotation.z * 2.0f;
	float yw2  = bt.Rotation.y * bt.Rotation.w * 2.0f;
	float yz2  = bt.Rotation.y * bt.Rotation.z * 2.0f;
	float zw2  = bt.Rotation.z * bt.Rotation.w * 2.0f;

	bt.Rotation *= bt.Rotation;

	retv = 
		float3( ( xxzz + wwyy )					 * v.x + ( xy2 + zw2 ) * v.y + ( xz2 - yw2 )*v.z,
			   ( xy2 - zw2 )					 * v.x + ( bt.Rotation.y	 + bt.Rotation.w
			   - bt.Rotation.x - bt.Rotation.z ) * v.y + ( yz2 + xw2 ) * v.z,
			   ( xz2 + yw2 )					 * v.x + ( yz2 - xw2 ) * v.y + ( wwyy - xxzz )*v.z );

	retn = 
		float3( ( xxzz + wwyy )					 * n.x + ( xy2 + zw2 ) * n.y + ( xz2 - yw2 ) * n.z,
			   ( xy2 - zw2 )					 * n.x + ( bt.Rotation.y	 + bt.Rotation.w
			   - bt.Rotation.x - bt.Rotation.z ) * n.y + ( yz2 + xw2 ) * n.z,
			   ( xz2 + yw2 )					 * n.x + ( yz2 - xw2 ) * n.y + ( wwyy - xxzz ) * n.z );

	retv += bt.Offsets;
}
