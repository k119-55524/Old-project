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
	HRESULT LoadMeshBlock( std::ifstream &file, CMeshFormat* mForm, TCHAR* FileName );

	// PhysX
	PxVec3* pVerts;
	PxU32*	pIndex;
	PxTriangleMesh* PhysXMesh;
};

