#pragma once

#include "header.h"
#include "ResMesh.h"
#include "BufferSRV.h"

class CResPhysXStaticMesh :
	public CResMesh
{
public:
	CResPhysXStaticMesh( void );
	~CResPhysXStaticMesh( void );
	void Release( void );

	HRESULT CreateMeshOfTXTFile( TCHAR* FileName, CMeshFormat* mForm );

protected:
	HRESULT LoadMeshBlock( std::ifstream &file, CMeshFormat* mForm, TCHAR* FileName );

private:
	void CalcAABB( D3DXMATRIX* m_GlobalWordMat, CAABB &cAABB );

	// PhysX
	PxVec3* pVerts;
	PxU32*	pIndex;
	PxTriangleMesh* PhysXMesh;
};

