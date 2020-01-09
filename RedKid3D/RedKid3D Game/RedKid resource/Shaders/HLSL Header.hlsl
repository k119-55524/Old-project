
cbuffer GlobalConstant : register( b0 )
{
	matrix	World;
	matrix	View;
	matrix	Projection;
	matrix	ViewProj;

	float2  PixelSize;
}

Texture2D	g_DiffuseTexture	: register( t0 );	// Диффузная текстура объектов
Texture2D	g_DepthTexture		: register( t1 );	// Текстура буффера глубины

Texture2D	g_NormalTexture		: register( t1 );	// Текстура нормалей
Texture2D	g_MaterialTexture	: register( t2 );	// Текстура нормалей

// SMAA Textures
Texture2D	g_EdgesTexture		: register( t0 );	//
Texture2D	g_AreaTexture		: register( t1 );	//
Texture2D	g_SearchTexture		: register( t2 );	//
Texture2D	g_ColorTexture		: register( t0 );	//
Texture2D	g_BlendTexture		: register( t1 );	//
StructuredBuffer<float> HDRLum	: register( t2 );
Texture2D	g_BloomTexture		: register( t3 );	//

SamplerState Linear_Sampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
    //AddressW = WRAP;
    //ComparisonFunc = NEVER;
    //MaxAnisotropy = 16;
    //MinLOD = 0;
    //MaxLOD = 3.402823466e+38f;
};

SamplerState Point_Sampler
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Clamp;
	AddressV = Clamp;
};

SamplerState Anisotropic_Sampler
{
	Filter = ANISOTROPIC;
	AddressU = Clamp;
	AddressV = Clamp;
};

struct VS_INPUT_NOR_UV
{
	float3 Pos		: POSITION;
	float3 Nor		: NORMAL;
	float2 UV		: TEXCOORD;
};

struct PS_INPUT_NOR_UV
{
	float4 Pos		: SV_POSITION;
	float3 Nor		: NORMAL;
	float2 UV		: TEXCOORD0;
	float3 XYZ		: TEXCOORD1;

};

struct PS_INPUT_TBN_UV
{
	float4 Pos		: SV_POSITION;
	float3 Nor		: NORMAL;
	float3 Tan		: TANGENT;
	float3 Bin		: BINORMAL;
	float2 UV		: TEXCOORD;
};

struct VS_INPUT_TBN_UV
{
	float3 Pos		: POSITION;
	float3 Nor		: NORMAL;
	float3 Tan		: TANGENT;
	float3 Bin		: BINORMAL;
	float2 UV		: TEXCOORD;
};


/////////////////////////////////////
//
struct PS_OUT_RT
{
	float4	Diffuse		: SV_Target0;
	float4	Normal		: SV_Target1;
	float	Material	: SV_Target2;
};
//
/////////////////////////////////////

////////////////////////////////////////////////
// Структуры для рендринга цветных линий
struct VS_INPUT_LINE
{
    float3 Pos : POSITION;
    float4 Col : COLOR;	
};

struct PS_INPUT_LINE
{
    float4 Pos : SV_POSITION;
    float4 Col : COLOR;	
};
//
////////////////////////////////////////////////
// Структуры для рендринга интерфейса
struct VS_INPUT_INTERFACE
{
    float3 Pos : POSITION;
    float2 Tex : TEXCOORD;
};

struct PS_INPUT_INTERFACE
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD;
};

BlendState AlfaTexture
{
    BlendEnable[0] = TRUE;
    SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;
    BlendOp = ADD;
    SrcBlendAlpha = ZERO;
    DestBlendAlpha = ZERO;
    BlendOpAlpha = ADD;
    RenderTargetWriteMask[0] = 0x0F;
};

BlendState NoBlend
{ 
    BlendEnable[0] = False;
};
//
////////////////////////////////////////////////

struct VS_INPUT_QUAD
{
    float3 Pos : POSITION;
    float2 UV  : TEXCOORD;
};

struct PS_INPUT_QUAD
{
    float4 Pos : SV_POSITION;
    float2 UV  : TEXCOORD;
};

DepthStencilState DisableDepthReplaceStencil
{
    DepthEnable = FALSE;
    StencilEnable = TRUE;
    FrontFaceStencilPass = REPLACE;
};

DepthStencilState DisableDepthStencil
{
    DepthEnable = FALSE;
    StencilEnable = FALSE;
};

DepthStencilState DisableDepthUseStencil
{
    DepthEnable = FALSE;
    StencilEnable = TRUE;
    FrontFaceStencilFunc = EQUAL;
};

DepthStencilState DisableDepth
{
	DepthEnable		= FALSE;
	DepthWriteMask	= ZERO;
};

DepthStencilState EnableDepth
{
	DepthEnable		= TRUE;
	DepthWriteMask	= ALL;
	DepthFunc		= LESS;
};

BlendState NoBlending
{
    AlphaToCoverageEnable = FALSE;
    BlendEnable[0] = FALSE;
};

RasterizerState rsNoCulling
{
	CullMode = None;
};

RasterizerState rsCulling
{
	CullMode = Back;
};