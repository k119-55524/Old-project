#include "ObjectPhysXSceletMesh.h"

CObjectPhysXSceletMesh::CObjectPhysXSceletMesh( TCHAR* name )
	: CObjectPhysX( name )
{
	ActivAnim		= 0;
	mDTimeScAnim	= 0.0f;

	PhysXActor		= NULL;
	PhysXMaterial	= NULL;
	PhysXShape		= NULL;
	GlobalSceletOBB	= NULL;
}

CObjectPhysXSceletMesh::~CObjectPhysXSceletMesh( void )
{
	Release();
}

void CObjectPhysXSceletMesh::Release( void )
{
	UINT scbc = Scelet.GetBoneCount();
	Scelet.Release();

	ActivAnim		= 0;
	mDTimeScAnim	= 0.0f;

	aSceletAnim.Release();
	DEL_ARRAY( GlobalSceletOBB );

	if ( PhysXActor != NULL )
	{
		for( UINT i = 0; i < scbc; i++ )
		{
			rELEASE( PhysXShape[i] );
			rELEASE( PhysXActor[i] );
		}

		DEL_ARRAY( PhysXShape );
		DEL_ARRAY( PhysXActor );
	}
	//rELEASE( PhysXShape );
	rELEASE( PhysXMaterial );
}

void CObjectPhysXSceletMesh::SetPhysXPosition( float x, float y, float z )
{

}

void CObjectPhysXSceletMesh::UpdatePhysXPose( void )
{

}

HRESULT CObjectPhysXSceletMesh::AddMesh( CResMesh* mesh, PxScene* mPhysXScene, CResourceAnimationScelet* scanim )
{
	if ( mesh->GetType() != RES_PHYSX_SCELET_MESH )
		return S_FALSE;

	V_RETURN( aSceletAnim.Add( scanim, ActivAnim ) );

	try
	{
		Scelet = *(((CResPhysXSceletMesh*)mesh)->GetSceleton());
	}
	catch( ... )
	{
		LogFile.OutputLogFileErrorString( L"Error Scelet class constructor copy.", __FILE__, __FUNCTION__, __LINE__  );
		return S_FALSE;
	}

	DEL_ARRAY( GlobalSceletOBB );

	UINT BoneCount = Scelet.GetBoneCount();

	GlobalSceletOBB = new COBB[BoneCount];
	if ( GlobalSceletOBB == NULL )
	{
		LogFile.OutputLogFileErrorString( L"Memory allocation failure.", __FILE__, __FUNCTION__, __LINE__  );
		return S_FALSE;
	}

	PhysXActor = new PxRigidDynamic*[BoneCount];
	if ( PhysXActor == NULL )
	{
		LogFile.OutputLogFileErrorString( L"Memory allocation failure.", __FILE__, __FUNCTION__, __LINE__  );
		return S_FALSE;
	}

	PhysXShape = new PxShape*[BoneCount];
	if ( PhysXShape == NULL )
	{
		LogFile.OutputLogFileErrorString( L"Memory allocation failure.", __FILE__, __FUNCTION__, __LINE__  );
		return S_FALSE;
	}

	for ( UINT i = 0; i < BoneCount; i++ )
	{
		PhysXActor[i] = NULL;
		PhysXShape[i] = NULL;
	}

	PhysXMaterial = SystemRedKid.GetPhysX()->createMaterial( 0.5f, 0.5f, 0.1f );     //static friction, dynamic friction, restitution
	if ( PhysXMaterial == NULL )
	{
		LogFile.OutputLogFileErrorString( L"Error createMaterial.", __FILE__, __FUNCTION__, __LINE__  );
		return S_FALSE;
	}

	UpdateScelet( 0.0f );
	Scelet.SetupBindPose();

	PhysXScene = mPhysXScene;

	PxTransform pose;
	pose = PxTransform::createIdentity();
	pose.isValid();

	for ( UINT i = 0; i < BoneCount; i++ )
	{
		pose.p.x = Scelet.m_BonesTransform[i].Offsets.x;
		pose.p.y = Scelet.m_BonesTransform[i].Offsets.y;
		pose.p.z = Scelet.m_BonesTransform[i].Offsets.z;

		pose.q.x = Scelet.m_BonesTransform[i].Rotations.x;
		pose.q.y = Scelet.m_BonesTransform[i].Rotations.y;
		pose.q.z = Scelet.m_BonesTransform[i].Rotations.z;
		pose.q.w = Scelet.m_BonesTransform[i].Rotations.w;

		PhysXActor[i] = SystemRedKid.GetPhysX()->createRigidDynamic( pose );
		if ( PhysXActor[i] == NULL )
		{
			LogFile.OutputLogFileErrorString( L"Error createRigidStatic.", __FILE__, __FUNCTION__, __LINE__  );
			return S_FALSE;
		}

		PhysXActor[i]->userData = (void*)this;
		PhysXActor[i]->setRigidDynamicFlag( PxRigidDynamicFlag::eKINEMATIC, true );	// Set kinematic oblect

		float dx = ((CResPhysXSceletMesh*)mesh)->LocalOBB[i].GetXSize() / 2.0f;
		float dy = ((CResPhysXSceletMesh*)mesh)->LocalOBB[i].GetYSize() / 2.0f;
		float dz = ((CResPhysXSceletMesh*)mesh)->LocalOBB[i].GetZSize() / 2.0f;

		PxBoxGeometry pbg( dx, dy, dz );

		PhysXShape[i] = PhysXActor[i]->createShape( pbg, *PhysXMaterial );
		if ( PhysXShape[i] == NULL )
		{
			LogFile.OutputLogFileErrorString( L"Error createShape.", __FILE__, __FUNCTION__, __LINE__  );
			return S_FALSE;
		}

		mPhysXScene->addActor( *PhysXActor[i] );
	}

	V_RETURN( CObjectMesh::AddMesh( mesh ) );

	//#if ( defined( DEBUG ) || defined( _DEBUG ) )
	//	//static D3DXMATRIX g_Position, g_Rotation, g_Orient;
	//	static UINT i, i1;

	//	for ( i = 0; i < BoneCount; i++ )
	//	{
	//	//	D3DXMatrixRotationQuaternion ( &g_Rotation, &Scelet.m_BonesTransform[i].Rotations );
	//	//	D3DXMatrixTranspose( &g_Rotation, &g_Rotation );
	//	//	D3DXMatrixTranslation( &g_Position, Scelet.m_BonesTransform[i].Offsets.x, Scelet.m_BonesTransform[i].Offsets.y, Scelet.m_BonesTransform[i].Offsets.z );
	//	//	g_Orient = g_Rotation * g_Position;

	//		for ( i1 = 0; i1 < 8; i1++ )
	//			GlobalSceletOBB[i].Box[i1] = ((CResPhysXSceletMesh*)Mesh)->LocalOBB[i].Box[i1];
	//	//		D3DXVec3TransformCoord( &GlobalSceletOBB[i].Box[i1], &(((CResPhysXSceletMesh*)Mesh)->LocalOBB[i].Box[i1]), &g_Orient );

	//		GlobalSceletOBB[i].CreateSignMesh();
	//	}
	//#endif

	return S_OK;
}

void CObjectPhysXSceletMesh::UpdateScelet( float dt )
{
	static float deltaTime;
	deltaTime = mDTimeScAnim + dt;

	static CResourceAnimationScelet* ActivAnimation;
	ActivAnimation = aSceletAnim[ActivAnim];

	// Колличество кадров при создании анимации в 3DMax
	static UINT fps;
	fps = ActivAnimation->GetPerFrame();

	static float st;
	st = deltaTime * fps;
	static int ist;
	ist = (int)floorf( st );
	static int f0;
	f0 = ist % ActivAnimation->GetFrameCount();
	static int f1;
	f1 = ( f0 + 1 ) % ActivAnimation->GetFrameCount();
	static float frameLerp;
	frameLerp = st - (float)ist;
	static UINT i, j;

	static CAnimationFrame* frame0;
	frame0 = ActivAnimation->GetFrame( f0 );
	static CAnimationFrame* frame1;
	frame1 = ActivAnimation->GetFrame( f1 );

	static D3DXVECTOR4 Offset;
	static D3DXQUATERNION Rotation;
	static D3DXMATRIX matres;
	static UINT mScCount;
	mScCount = Scelet.GetBoneCount();
	for ( i = 0; i < mScCount; ++i )
	{
		D3DXVec4Lerp( &Offset, &frame0->GetOffset( i ), &frame1->GetOffset( i ), frameLerp );
		D3DXQuaternionSlerp( &Rotation, &frame0->GetRotation( i ), &frame1->GetRotation( i ), frameLerp );
		D3DXQuaternionNormalize( &Rotation, &Rotation );

		Scelet.m_BonesTransform[i].Rotations = Rotation;
		Scelet.m_BonesTransform[i].Offsets = Offset;
	}

	Scelet.AnimateHierarhy();
}

HRESULT CObjectPhysXSceletMesh::AddSignMesh( CArrayStack<CSignMesh*> &SignMesh )
{
	static UINT i, BoneCount;
	BoneCount = Scelet.GetBoneCount();

	for ( i = 0; i < BoneCount; i++ )
		V_RETURN( SignMesh.Add( GlobalSceletOBB[i].GetSignMesh() ) );

	return S_OK;
};

void CObjectPhysXSceletMesh::CalcAABB( D3DXMATRIX* m_GlobalWordMat )
{
	static PxBounds3 aabb;
	//aabb = PhysXShape->getWorldBounds();
	//AABB.Set( aabb.minimum.x, aabb.minimum.y, aabb.minimum.z, aabb.maximum.x, aabb.maximum.y, aabb.maximum.z );
	AABB.Set( 10.0f, 10.0f, 10.0f, 100.0f, 100.0f, 100.0f );

	#if ( defined( DEBUG ) || defined( _DEBUG ) )
		if ( fRenderSign )
			V_( cSignMesh.CreateAABBMesh( &AABB, D3DXVECTOR4( 1.0f, 0.0f, 0.0f, 1.0f ) ) );
	#endif
}