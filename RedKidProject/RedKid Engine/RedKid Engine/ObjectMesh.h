#pragma once

#include "ResMesh.h"
#include "SignMesh.h"
#include "ObjectBase.h"

class CObjectMesh :
	public CObjectBase
{

public:
	CObjectMesh( void );
	CObjectMesh( TCHAR* name );
	~CObjectMesh( void );
	void Release( void );

	virtual HRESULT AddMesh( CResMesh* mesh );
	virtual CResMesh* GetMesh( void ) { return Mesh; };

protected:
	CResMesh* Mesh;

	void CalcAABB( D3DXMATRIX* m_GlobalWordMat )
	{ 
		Mesh->CalcAABB( m_GlobalWordMat, AABB );

		#if ( defined( DEBUG ) || defined( _DEBUG ) )
			if ( fRenderSign )
				V_( cSignMesh.CreateAABBMesh( &AABB, D3DXVECTOR4( 1.0f, 0.0f, 0.0f, 1.0f ) ) );
		#endif
	};
};

