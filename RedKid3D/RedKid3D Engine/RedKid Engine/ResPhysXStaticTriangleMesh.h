#pragma once

#include "ResMesh.h"

class CResPhysXStaticTriangleMesh :
	public CResMesh
{
public:
	CResPhysXStaticTriangleMesh( void );
	~CResPhysXStaticTriangleMesh( void) ;
	void Release( void );

	PxTriangleMesh* GetPhysXMesh() { return PhysXMesh; };

protected:
	HRESULT InitResIndividualData( VERTEX_TBN_UV_BONE* Vertex, UINT VertexCount, DWORD* Index, UINT IndexCount );

	// PhysX
	PxVec3* pVerts;
	PxU32*	pIndex;
	PxTriangleMesh* PhysXMesh;
};

