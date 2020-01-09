#include "ResPhysXDinamicConvexMesh.h"

CResPhysXDinamicConvexMesh::CResPhysXDinamicConvexMesh( void )
{
	Type = RES_PHYSX_CONVEX_DINAMIC_MESH;

	//PhysX
	PhysXMesh = NULL;
}

CResPhysXDinamicConvexMesh::~CResPhysXDinamicConvexMesh( void )
{
	Release();
}

void CResPhysXDinamicConvexMesh::Release( void )
{
	rELEASE( PhysXMesh );
}

HRESULT CResPhysXDinamicConvexMesh::InitResIndividualData( VERTEX_TBN_UV_BONE* Vertex, UINT VertexCount, DWORD* Index, UINT IndexCount )
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

		PxConvexMeshDesc convexDesc;
		convexDesc.points.count		= VertexCount;
		convexDesc.points.stride	= sizeof(PxVec3);
		convexDesc.points.data		= pVerts;
		convexDesc.triangles.count	= IndexCount / 3;
		convexDesc.triangles.stride = 3 * sizeof( PxU32 );
		convexDesc.triangles.data	= pIndex;
		convexDesc.flags			= PxConvexFlag::eCOMPUTE_CONVEX;

		MemoryWriteBuffer buf;
		if ( !SystemRedKid.GetPhysXCooking()->cookConvexMesh( convexDesc, buf ) )
		{
			LogFile.OutputLogFileErrorString( L"Error cookConvexMesh (PhysX).", __FILE__, __FUNCTION__, __LINE__ );
			throw 1;
		}

		PhysXMesh = SystemRedKid.GetPhysX()->createConvexMesh( MemoryReadBuffer( buf.data ) );
		if ( PhysXMesh == NULL )
		{
			LogFile.OutputLogFileErrorString( L"Error createConvexMesh (PhysX).", __FILE__, __FUNCTION__, __LINE__ );
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
