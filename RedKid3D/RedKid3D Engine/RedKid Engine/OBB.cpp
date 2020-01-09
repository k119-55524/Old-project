#include "OBB.h"

COBB::COBB(void)
{
	fMMStart = true;

	Center = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	for ( UINT i = 0; i < 8; i++ )
		Box[i] = Center;
}

COBB::~COBB(void)
{
	Release();
}

void COBB::Release( void )
{
	fMMStart = true;

	Center = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	for ( UINT i = 0; i < 8; i++ )
		Box[i] = Center;
}
