#pragma once

#include "header.h"
#include "LogFile.h"

class CTextureRT_SRV_UAV
{
public:
	CTextureRT_SRV_UAV( void );
	~CTextureRT_SRV_UAV( void );
	void Release( void );

	HRESULT Resize( UINT Width, UINT Height, DXGI_FORMAT Format );
	inline ID3D11RenderTargetView* GetTextureRT( void ) { return g_pTextureRT; };
	inline ID3D11ShaderResourceView* GetTextureSRV( void ) { return g_pTextureSRV; };
	inline ID3D11UnorderedAccessView* GetTextureUAV( void ) { return g_pTextureUAV; };

private:
	ID3D11Texture2D*			g_pTexture;
	ID3D11RenderTargetView*		g_pTextureRT;
	ID3D11ShaderResourceView*   g_pTextureSRV;
	ID3D11UnorderedAccessView*  g_pTextureUAV;
};

