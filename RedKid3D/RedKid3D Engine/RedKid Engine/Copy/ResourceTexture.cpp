#include "ResourceTexture.h"

CResourceTexture::CResourceTexture( void )
{
	Type = RES_TEXTURE;

	Width		= 0;
	Height		= 0;
	Depth		= 0;
	MipLevels	= 0;

	//g_pTexture		= NULL;
	//g_pTextureRT	= NULL;
	g_pTextureSRV	= NULL;
	//g_pTextureUAV	= NULL;
}

CResourceTexture::~CResourceTexture( void )
{
	Release();
}

void CResourceTexture::Release( void )
{
	//RELEASE( g_pTextureUAV );
	RELEASE( g_pTextureSRV );
	//RELEASE( g_pTextureRT );
	//RELEASE( g_pTexture );
}

HRESULT CResourceTexture::CreateTextureOfFile( TCHAR* FileName )
{
	TCHAR mESS[100];
	wsprintf( mESS, _T( "Start read texture file: %s." ), FileName );
	LogFile.OutputLogFileString( mESS );

	// Читаем информацию об изображении из файла
	static D3DX11_IMAGE_INFO InfoFromFile;
	V_RETURN( S_OK != D3DX11GetImageInfoFromFile( FileName, NULL, &InfoFromFile, NULL ) );

	Release();

	V_RETURN( D3DX11CreateShaderResourceViewFromFile( System.GetDevice(), FileName, NULL, NULL, &g_pTextureSRV, NULL ) );

	Width		= InfoFromFile.Width;
	Height		= InfoFromFile.Height;
	Depth		= InfoFromFile.Depth;
	MipLevels	= InfoFromFile.MipLevels;
	Name		= FileName;

	wsprintf( mESS, _T( "End read texture file: %s." ), FileName );
	LogFile.OutputLogFileString( mESS );

	return S_OK;
}