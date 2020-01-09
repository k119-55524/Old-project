#include "ObjectBase.h"

CObjectBase::CObjectBase( void )
{
	Name = NULL;
	fRenderSign = false;
	fCalcLocalMat = true;
	fCalcGlobalMat = true;
	fCalcAABB = true;
	Position  = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	Angle  = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	Position.x = Position.y = Position.z = 0.0f;
	Angle.x = Angle.y = Angle.z = 0.0f;
	D3DXMatrixIdentity( &m_LocalWordMat );
	D3DXMatrixIdentity( &m_GlobalWordMat );

	ParentObj	= NULL;
	ChildObj	= NULL;
	PrevObj		= NULL;
	NextObj		= NULL;

	Frustum		= NULL;
}

CObjectBase::CObjectBase( TCHAR* name )
{
	Name = name;
	fRenderSign = false;
	fCalcAABB = true;
	Position  = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	Angle  = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	ParentObj	= NULL;
	ChildObj	= NULL;
	PrevObj		= NULL;
	NextObj		= NULL;

	Frustum		= NULL;
}

CObjectBase::~CObjectBase( void )
{
	Release();
}

void CObjectBase::Release( void )
{
	DEL( ChildObj );
	DEL( NextObj );

	Position  = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
}