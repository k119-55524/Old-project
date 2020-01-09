#include "ObjectLightDirection.h"

CObjectLightDirection::CObjectLightDirection( TCHAR* Name, D3DXVECTOR4* Col, float SignScale )
	: CObjectBase( Name )
{
	Color = *Col;
	Direction = D3DXVECTOR4( 0.0f, 0.0f, -1.0f, 1.0f );
	mSignScale = SignScale;
	Type = OBJ_LIGHTDIRECTION;
}

CObjectLightDirection::~CObjectLightDirection( void )
{
	Release();
	CObjectBase::~CObjectBase();
}

void CObjectLightDirection::Release( void  )
{

}