#pragma once

#include "OBB.h"
#include "Scelet.h"
#include "ResMesh.h"

class CResPhysXSceletMesh :
	public CResMesh
{
	friend class CObjectPhysXSceletMesh;

public:
	CResPhysXSceletMesh( void );
	~CResPhysXSceletMesh( void );
	void Release( void );

	CScelet* GetSceleton( void ) { return &Scelet; };
	UINT GetBoneCount( void ) { return Scelet.GetBoneCount(); };

protected:
	HRESULT LoadSceletBlock( std::ifstream &file, TCHAR* FileName ) { return Scelet.ReadFromStream( file ); };
	HRESULT InitResIndividualData( VERTEX_TBN_UV_BONE* Vertex, UINT VertexCount, DWORD* Index, UINT IndexCount );

	CScelet Scelet;

	COBB*	LocalOBB;
};

