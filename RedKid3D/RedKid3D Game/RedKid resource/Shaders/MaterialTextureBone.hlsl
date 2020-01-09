
PS_INPUT_NOR_UV VS_Texture_Bone( VS_INPUT_NOR_UV In )
{
	PS_INPUT_NOR_UV Out;

	Out.XYZ = In.Pos;
	Out.Nor = In.Nor;

	Out.Pos = mul( float4( In.Pos, 1.0f ), ViewProj );
	Out.UV = In.UV;

	return Out;
}

technique11 RenderTextureBone
{
	pass P0
	{
		SetVertexShader( CompileShader ( vs_5_0, VS_Texture_Bone() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader ( ps_5_0, PS_Texture() ) );

		SetBlendState( NoBlend, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
		SetRasterizerState( rsCulling );
		SetDepthStencilState( EnableDepth, 0 );
	}
}