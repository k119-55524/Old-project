
PS_INPUT_NOR_UV VS_Texture( VS_INPUT_NOR_UV In )
{
	PS_INPUT_NOR_UV Out;
	Out.Pos = mul( float4( In.Pos, 1.0f ), World );

	Out.XYZ = Out.Pos;
	Out.Nor = mul( In.Nor, World );

	Out.Pos = mul( Out.Pos, ViewProj );
	Out.UV = In.UV;

	return Out;
}

technique11 RenderTexture
{
	pass P0
	{
		SetVertexShader( CompileShader ( vs_5_0, VS_Texture() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader ( ps_5_0, PS_Texture() ) );

		SetBlendState( NoBlend, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
		SetRasterizerState( rsCulling );
		SetDepthStencilState( EnableDepth, 0 );
	}
}
