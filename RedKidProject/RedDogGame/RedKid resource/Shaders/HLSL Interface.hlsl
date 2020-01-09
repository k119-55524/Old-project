
PS_INPUT_INTERFACE VS_RenderInterface( VS_INPUT_INTERFACE In )
{
	PS_INPUT_INTERFACE Out = (PS_INPUT_INTERFACE)0;

	Out.Pos = float4( In.Pos, 1.0f );
	Out.Tex = In.Tex;

    return Out;
}

float4 PS_RenderInterface( PS_INPUT_INTERFACE In ) : SV_Target
{
	return g_DiffuseTexture.Sample( Linear_Sampler, In.Tex );
}

technique11 RenderInterface
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS_RenderInterface() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS_RenderInterface() ) );

        SetBlendState( AlfaTexture, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff );
		SetDepthStencilState( DisableDepth, 0 );
		SetRasterizerState( rsNoCulling );
     }
}