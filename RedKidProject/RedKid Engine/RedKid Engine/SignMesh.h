#pragma once

#include "header.h"
#include "AABB.h"
#include "LogFile.h"

class CSignMesh
{
public:
	CSignMesh( void );
	~CSignMesh( void );
	void Release( void );

	HRESULT CreatePointLightMesh( D3DXVECTOR3 Pos, float Rad );
	HRESULT CreateAABBMesh( CAABB* AABB, D3DXVECTOR4 &Col );
	HRESULT CreateDirectoinLightMesh( D3DXMATRIX* WorldMat, float mSignScale );
	void Draw( void );

private:
	HRESULT ResizeBuffer( UINT NewCount );
	ID3D11Buffer* g_pVertexBuffer;

	LPVERTEX_LINE vb;	// Массив хранящий сетку ограничивающего бокса в памяти CPU
	UINT vbCount;

	UINT PointCount;
};

