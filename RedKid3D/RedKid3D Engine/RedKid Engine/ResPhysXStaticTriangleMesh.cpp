#include "System.h"
#include "ResPhysXStaticTriangleMesh.h"

CResPhysXStaticTriangleMesh::CResPhysXStaticTriangleMesh(void)
	: CResMesh()
{
	Type = RES_PHYSX_TRIANGLE_STATIC_MESH;

	// PhysX
	pVerts			= NULL;
	pIndex			= NULL;
	PhysXMesh		= NULL;
}

CResPhysXStaticTriangleMesh::~CResPhysXStaticTriangleMesh(void)
{
	Release();
}

void CResPhysXStaticTriangleMesh::Release(void)
{
	// PhysX
	DEL_ARRAY( pVerts );
	DEL_ARRAY( pIndex );
	rELEASE( PhysXMesh );
}

HRESULT CResPhysXStaticTriangleMesh::InitResIndividualData( VERTEX_TBN_UV_BONE* Vertex, UINT VertexCount, DWORD* Index, UINT IndexCount )
{
	PxVec3* pVerts = NULL;
	PxU32*	pIndex = NULL;

	try
	{
		pVerts = new _declspec(align(16)) PxVec3[VertexCount];
		if ( pVerts == NULL )
		{
			LogFile.OutputLogFileErrorString( L"Error of allocation of memory(PhysX).", __FILE__, __FUNCTION__, __LINE__ );
			throw 1;
		}

		for ( UINT i = 0; i < VertexCount; i++ )
		{
			pVerts[i].x = Vertex[i].Pos.x;
			pVerts[i].y = Vertex[i].Pos.y;
			pVerts[i].z = Vertex[i].Pos.z;
		}

		pIndex = new PxU32[IndexCount * 3];
		if ( pIndex == NULL )
		{
			LogFile.OutputLogFileErrorString( L"Error of allocation of memory(PhysX).", __FILE__, __FUNCTION__, __LINE__ );
			throw 1;
		}

		for ( UINT i = 0; i < IndexCount; i++ )
			pIndex[i] = Index[i];

		PxTriangleMeshDesc meshDesc;
		meshDesc.points.count			= mVertexCount;			// Количесство вершин
		meshDesc.triangles.count		= mIndexCount / 3;		// Количесство индексов
		meshDesc.points.stride			= sizeof( PxVec3 );		// Размер 1 структуры буффера вершин
		meshDesc.triangles.stride		= 3 * sizeof( PxU32 );	// Размер 1 структуры буффера индексов
		meshDesc.points.data            = pVerts;				// Указатель на буффер вершин
		meshDesc.triangles.data         = pIndex;				// Указатель на буффер индексов

		MemoryWriteBuffer buf;
		if ( !SystemRedKid.GetPhysXCooking()->cookTriangleMesh( meshDesc, buf ) )
		{
			Release();
			LogFile.OutputLogFileErrorString( L"Error cookTriangleMesh (PhysX).", __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}

		PhysXMesh = SystemRedKid.GetPhysX()->createTriangleMesh( MemoryReadBuffer( buf.data ) );
		if ( PhysXMesh == NULL )
		{
			Release();
			LogFile.OutputLogFileErrorString( L"Error createTriangleMesh (PhysX).", __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}

		DEL_ARRAY( pVerts );
		DEL_ARRAY( pIndex );
	}
	catch( ... )
	{
		DEL_ARRAY( pVerts );
		DEL_ARRAY( pIndex );
	}

	return S_OK;
}
