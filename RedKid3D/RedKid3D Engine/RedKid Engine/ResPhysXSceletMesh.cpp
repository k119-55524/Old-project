#include "ResPhysXSceletMesh.h"

CResPhysXSceletMesh::CResPhysXSceletMesh( void )
{
	Type			= RES_PHYSX_SCELET_MESH;
	LocalOBB		= NULL;
}

CResPhysXSceletMesh::~CResPhysXSceletMesh( void )
{
	Release();
}

void CResPhysXSceletMesh::Release( void )
{
	Scelet.Release();
	DEL_ARRAY( LocalOBB );
}

HRESULT CResPhysXSceletMesh::InitResIndividualData( VERTEX_TBN_UV_BONE* Vertex, UINT VertexCount, DWORD* Index, UINT IndexCount )
{
	UINT BoneCount = Scelet.GetBoneCount();

	DEL_ARRAY( LocalOBB );

	LocalOBB = new COBB[BoneCount];
	if ( LocalOBB == NULL )
	{
		LogFile.OutputLogFileErrorString( L"Memory allocation failure.", __FILE__, __FUNCTION__, __LINE__  );
		return S_FALSE;
	}

	for ( UINT i = 0; i < VertexCount; i++ )
		for ( UINT ib = 0; ib < Vertex[i].NumWeigth; ib++ )
			LocalOBB[Vertex[i].Weigth[ib].BoneID].CheckMinMaxPoint( Vertex[i].Pos );

	for ( UINT i = 0; i < BoneCount; i++ )
		LocalOBB[i].EndCheck();

	return S_OK;
}