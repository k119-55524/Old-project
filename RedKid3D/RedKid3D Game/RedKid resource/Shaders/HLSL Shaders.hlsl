#include "HLSL Header.hlsl"

///////////////////////////////////////////////////////////
// Block free programm
PS_OUT_RT PS_Texture( PS_INPUT_NOR_UV In )
{
	PS_OUT_RT Out;

	float3 n = normalize( In.Nor );

	// Расчёт направленного источника( солнца )
	Out.Diffuse = g_DiffuseTexture.Sample( Linear_Sampler, In.UV );
	Out.Diffuse.w = In.XYZ.x;

	// Pack Normal
	Out.Normal.x = 0.5f * n.x + 0.5f;
	Out.Normal.y = 0.5f * n.y + 0.5f;
	if ( n.z < 0.0f )
		Out.Normal.y += 1.0f;

	Out.Normal.zw = In.XYZ.yz;

	Out.Material = 1.0f;

	return Out;
}

//
///////////////////////////////////////////////////////////

#include "HLSL Quad.hlsl"
#include "HLSL SMAA.hlsl"
#include "HLSL Line.hlsl"
#include "HLSL Interface.hlsl"
#include "MaterialTexture.hlsl"
#include "MaterialTextureBone.hlsl"
#include "HLSL Deffered RenderLight.hlsl"