#include "System.h"
#include "TextureSRV_UAV.h"

CTextureSRV_UAV::CTextureSRV_UAV( void )
{
	g_pTexture		= NULL;
	g_pTextureSRV	= NULL;
	g_pTextureUAV	= NULL;
}

CTextureSRV_UAV::~CTextureSRV_UAV( void )
{
	Release();
}

void CTextureSRV_UAV::Release( void )
{
	RELEASE( g_pTextureUAV );
	RELEASE( g_pTextureSRV );
	RELEASE( g_pTexture );
}

HRESULT CTextureSRV_UAV::Resize( UINT Width, UINT Height, DXGI_FORMAT Format )
{
	static D3D11_TEXTURE2D_DESC td;
	ZeroMemory( &td, sizeof(D3D11_TEXTURE2D_DESC) );
	td.Width = Width;
	td.Height = Height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = Format;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	Release();

	V_RETURN( SystemRedKid.GetDevice()->CreateTexture2D( &td, NULL, &g_pTexture ) );

	static D3D11_SHADER_RESOURCE_VIEW_DESC DescSRV;
	ZeroMemory( &DescSRV, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC) );
	DescSRV.Format = Format;
	DescSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	DescSRV.Texture2D.MipLevels = 1;
	DescSRV.Texture2D.MostDetailedMip = 0;
	V_RETURN( SystemRedKid.GetDevice()->CreateShaderResourceView( g_pTexture, &DescSRV, &g_pTextureSRV ) );

	static D3D11_UNORDERED_ACCESS_VIEW_DESC DescUAV;
	ZeroMemory( &DescUAV, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC) );
	DescUAV.Format = DXGI_FORMAT_UNKNOWN;
	DescUAV.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	DescUAV.Texture2D.MipSlice = 0;
	V_RETURN( SystemRedKid.GetDevice()->CreateUnorderedAccessView( g_pTexture, &DescUAV, &g_pTextureUAV ) );

	return S_OK;
}