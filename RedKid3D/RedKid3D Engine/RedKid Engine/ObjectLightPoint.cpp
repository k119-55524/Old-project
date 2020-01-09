#include "ObjectLightPoint.h"

CObjectLightPoint::CObjectLightPoint( TCHAR* Name, D3DXVECTOR3* Pos, D3DXVECTOR4* Col, float rad )
	: CObjectBase( Name )
{
	Type = OBJ_LIGHTPOINT;

	Position = *Pos;
	Color = *Col;
	Radius = rad;
}

CObjectLightPoint::~CObjectLightPoint( void )
{
	Release();
	CObjectBase::~CObjectBase();
}

void CObjectLightPoint::Release( void )
{
}

void CObjectLightPoint::UpdateObjAnimation( float DeltaTime )
{

}

void CObjectLightPoint::CalcAABB( D3DXMATRIX* m_GlobalWordMat )
{
	AABB.Set( D3DXVECTOR3( Position.x - Radius, Position.y - Radius, Position.z - Radius ), D3DXVECTOR3( Position.x + Radius, Position.y + Radius, Position.z + Radius ) );

	#if ( defined( DEBUG ) || defined( _DEBUG ) )
		if ( fRenderSign )
			V_( cSignMesh.CreatePointLightMesh( Position, Radius ) );
	#endif
}