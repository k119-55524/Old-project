#include "System.h"
#include "TextureRT_SRV_UAV.h"

CTextureRT_SRV_UAV::CTextureRT_SRV_UAV( void )
{
	g_pTexture		= NULL;
	g_pTextureRT	= NULL;
	g_pTextureSRV	= NULL;
	g_pTextureUAV	= NULL;
}

CTextureRT_SRV_UAV::~CTextureRT_SRV_UAV( void )
{
	Release();
}

void CTextureRT_SRV_UAV::Release( void )
{
	RELEASE( g_pTextureUAV );
	RELEASE( g_pTextureSRV );
	RELEASE( g_pTextureRT );
	RELEASE( g_pTexture );
}

HRESULT CTextureRT_SRV_UAV::Resize( UINT Width, UINT Height, DXGI_FORMAT Format )
{
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory( &td, sizeof(D3D11_TEXTURE2D_DESC) );
	td.Width = Width;
	td.Height = Height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = Format;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	Release();
	V_RETURN( SystemRedKid.GetDevice()->CreateTexture2D( &td, NULL, &g_pTexture ) );

	static D3D11_RENDER_TARGET_VIEW_DESC rtd;
	ZeroMemory( &rtd, sizeof(D3D11_RENDER_TARGET_VIEW_DESC) );
	rtd.Format = Format;
	rtd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtd.Texture2D.MipSlice = 0;
	V_RETURN( SystemRedKid.GetDevice()->CreateRenderTargetView( g_pTexture, &rtd, &g_pTextureRT ) );

	static D3D11_SHADER_RESOURCE_VIEW_DESC DescSRV;
	ZeroMemory( &DescSRV, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC) );
	DescSRV.Format = Format;
	DescSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	DescSRV.Texture2D.MipLevels = 1;
	DescSRV.Texture2D.MostDetailedMip = 0;
	V_RETURN( SystemRedKid.GetDevice()->CreateShaderResourceView( g_pTexture, &DescSRV, &g_pTextureSRV ) );

	static D3D11_UNORDERED_ACCESS_VIEW_DESC	BufferDescUAV;
	ZeroMemory( &BufferDescUAV, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC) );
	BufferDescUAV.Format = Format;
	BufferDescUAV.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	V_RETURN( SystemRedKid.GetDevice()->CreateUnorderedAccessView( g_pTexture, &BufferDescUAV, &g_pTextureUAV ) );

	return S_OK;
}