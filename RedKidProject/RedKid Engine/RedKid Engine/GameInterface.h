#pragma once

#include "header.h"
#include "System.h"
#include "ResourceTexture.h"

class CGameInterface
{
public:
	CGameInterface( eTypeInterface t );
	~CGameInterface( void );
	void Release( void );

	HRESULT Resize( UINT Width, UINT Height );

	HRESULT InitTextInterface( UINT Width, UINT Height, UINT line, TCHAR* TDFName, CResourceTexture* Text );
	HRESULT SetText( char* t );
	HRESULT SetTextLine( UINT line );
	void Draw( void );

protected:
	HRESULT PrintText( void );

	bool fInit;
	eTypeInterface Type;
	CResourceTexture* Texture;

	char* Text;
	size_t mTextSize;

	// ������ � ������
	UINT mArraySize;
	LPVERTEX_INTERFACE aMeshArray;
	ID3D11Buffer* g_pVertexBuffer;

	UINT Line;						// ����������� ����� � ����
	float xPointSize, yPointSize;	// ������ 1 ����� �� x � y ��� ������
	float ySymbSize;				// ������ ������� ��� ������ �� y( � ����������� �� ������� ������, ������� ����� ������ � ����������� ����� �� ������)
	UINT FontData[95];

	D3DXVECTOR3 Pos;
};

