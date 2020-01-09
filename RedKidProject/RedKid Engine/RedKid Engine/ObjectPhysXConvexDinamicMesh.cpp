#include "System.h"
#include "ObjectPhysXConvexDinamicMesh.h"

CObjectPhysXConvexDinamicMesh::CObjectPhysXConvexDinamicMesh( TCHAR* name )
		: CObjectPhysX( name )
{
	PhysXMaterial	= NULL;
	PhysXShape		= NULL;
}

CObjectPhysXConvexDinamicMesh::~CObjectPhysXConvexDinamicMesh( void )
{
	Release();
}

void CObjectPhysXConvexDinamicMesh::Release( void )
{
	rELEASE( PhysXShape );
	rELEASE( PhysXMaterial );

}

HRESULT CObjectPhysXConvexDinamicMesh::AddMesh( CResMesh* mesh, PxScene* mPhysXScene )
{
	if ( mesh->GetType() != RES_PHYSX_CONVEX_DINAMIC_MESH )
		return S_FALSE;

	// PhysX
	if ( PhysXActor == NULL )
	{
		if ( mPhysXScene == NULL )
		{
			LogFile.OutputLogFileErrorString( L"mPhysXScene == NULL.", __FILE__, __FUNCTION__, __LINE__  );
			return S_FALSE;
		}

		PhysXScene = mPhysXScene;

		PxTransform pose;
		pose = PxTransform::createIdentity();

		PhysXActor = SystemRedKid.GetPhysX()->createRigidDynamic( pose );
		if ( PhysXActor == NULL )
		{
			LogFile.OutputLogFileErrorString( L"Error createRigidStatic.", __FILE__, __FUNCTION__, __LINE__  );
			return S_FALSE;
		}

		PhysXActor->userData = (void*)this;

		PhysXMaterial = SystemRedKid.GetPhysX()->createMaterial( 0.5f, 0.5f, 0.1f );     //static friction, dynamic friction, restitution
		if ( PhysXMaterial == NULL )
		{
			LogFile.OutputLogFileErrorString( L"Error createMaterial.", __FILE__, __FUNCTION__, __LINE__  );
			return S_FALSE;
		}
	}

	PhysXShape = ((PxRigidDynamic*)PhysXActor)->createShape( PxConvexMeshGeometry( ((CResPhysXDinamicConvexMesh*)mesh)->GetPhysXMesh() ), *PhysXMaterial );
	if ( PhysXShape == NULL )
	{
		LogFile.OutputLogFileErrorString( L"Error createShape.", __FILE__, __FUNCTION__, __LINE__  );
		return S_FALSE;
	}

	mPhysXScene->addActor( *PhysXActor );
	V_RETURN( CObjectMesh::AddMesh( mesh ) );

	return S_OK;
}

void CObjectPhysXConvexDinamicMesh::CalcAABB( D3DXMATRIX* m_GlobalWordMat )
{
	static PxBounds3 aabb;
	aabb = PhysXShape->getWorldBounds();
	AABB.Set( aabb.minimum.x, aabb.minimum.y, aabb.minimum.z, aabb.maximum.x, aabb.maximum.y, aabb.maximum.z );

	#if ( defined( DEBUG ) || defined( _DEBUG ) )
		if ( fRenderSign )
			V_( cSignMesh.CreateAABBMesh( &AABB, D3DXVECTOR4( 1.0f, 0.0f, 0.0f, 1.0f ) ) );
	#endif
}