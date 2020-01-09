#pragma once

#include "header.h"
#include "LogFile.h"

class CTextureRTAndSRV
{
public:
	CTextureRTAndSRV( void );
	~CTextureRTAndSRV( void );
	void Release( void );
	HRESULT Resize( UINT Width, UINT Height, DXGI_FORMAT Format );

	inline ID3D11Texture2D*	GetTexture( void ) { return	g_pTexture; };
	inline ID3D11RenderTargetView* GetTextureRT( void ) { return g_pTextureRT; };
	inline ID3D11ShaderResourceView* GetTextureSRV( void ) { return g_pTextureSRV; };

protected:
	ID3D11Texture2D*			g_pTexture;
	ID3D11RenderTargetView*		g_pTextureRT;
	ID3D11ShaderResourceView*	g_pTextureSRV;
};

