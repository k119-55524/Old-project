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
	HRESULT LoadMeshBlock( std::ifstream &file, CMeshFormat* mForm, TCHAR* FileName );

	// PhysX
	PxVec3* pVerts;
	PxU32*	pIndex;
	PxConvexMesh* PhysXMesh;
};

