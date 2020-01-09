#pragma once

#include "ResMesh.h"

class CResPhysXDinamicConvexMesh :
	public CResMesh
{
public:
	CResPhysXDinamicConvexMesh( void );
	~CResPhysXDinamicConvexMesh( void );
	void Release( void );

	PxConvexMesh* GetPhysXMesh() { return PhysXMesh; };

protected:
	HRESULT InitResIndividualData( VERTEX_TBN_UV_BONE* Vertex, UINT VertexCount, DWORD* Index, UINT IndexCount );

	PxConvexMesh* PhysXMesh;
};

