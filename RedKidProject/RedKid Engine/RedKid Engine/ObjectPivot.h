#pragma once

#include "ObjectBase.h"

class CObjectPivot :
	public CObjectBase
{
	friend class CRenderPool;

public:
	CObjectPivot( TCHAR* Name );
	~CObjectPivot(void);

	void UpdateObjAnimation( float DeltaTime ) {};
	void CalcAABB( D3DXMATRIX* m_GlobalWordMat ) { AABB.Set( D3DXVECTOR3( Position.x, Position.y, Position.z ), D3DXVECTOR3( Position.x, Position.y, Position.z ) ); };

protected:
	void Release( void  );
};

