
PS_INPUT_QUAD VS_Quad( VS_INPUT_QUAD In )
{
	PS_INPUT_QUAD Out;

	Out.Pos = float4( In.Pos, 1.0f );
	Out.UV = In.UV;

	return Out;
}

float4 PS_Quad( PS_INPUT_QUAD In ) : SV_Target
{
	return g_DiffuseTexture.Sample( Linear_Sampler, In.UV );
}

technique11 RenderQuad
{
	pass P0
	{
		SetVertexShader( CompileShader ( vs_5_0, VS_Quad() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader ( ps_5_0, PS_Quad() ) );
	}
}