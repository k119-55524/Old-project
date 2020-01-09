#pragma once

#include "header.h"
#include "System.h"
#include "LogFile.h"
#include "ResBase.h"

class CResourceTexture :
	public CResBase
{
	friend class CManagerResource;

public:
	CResourceTexture( void );
	~CResourceTexture( void );
	void Release( void );

	HRESULT CreateTextureOfFile( TCHAR* FileName );
	inline ID3D11ShaderResourceView* GetSRV( void ) { return g_pTextureSRV; };

	inline UINT GetWidth( void ) { return Width; };
	inline UINT GetHeight( void ) { return Height; };

private:
	UINT		Width;
	UINT		Height;
	UINT		Depth;
	UINT		MipLevels;

	ID3D11ShaderResourceView*		g_pTextureSRV;
};

