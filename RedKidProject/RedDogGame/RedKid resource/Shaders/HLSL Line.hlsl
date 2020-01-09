//-----------------------------------------------------------------------
// Функция вершинного шейдера
//-----------------------------------------------------------------------
PS_INPUT_LINE VS_Line( VS_INPUT_LINE Data )
{
    PS_INPUT_LINE Out;

	Out.Pos = mul( float4( Data.Pos, 1.0f ), ViewProj );
	Out.Col = Data.Col;
   
    return Out;
}

//-----------------------------------------------------------------------
// Функция пиксельного шейдера (рисует текстуру) 
//-----------------------------------------------------------------------
float4 PS_Line( PS_INPUT_LINE Input ) : SV_Target
{
	return Input.Col;
}

technique11 RenderLine
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS_Line() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS_Line() ) );

		SetDepthStencilState( EnableDepth, 0 );
    }
}