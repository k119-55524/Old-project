#include "RenderPool.h"

CRenderPool::CRenderPool(void)
	: Scene( _T("Base pivot all world.") ), aLightPoint( 10 ), Frustum( 5000000.0f, 20 ) // 1.0f = 1mm. итого - 100km.
{
	fInit = false;

	D3DXMatrixIdentity( &g_View );
	D3DXMatrixIdentity( &g_Projection );
	D3DXMatrixIdentity( &g_ViewProj );
	zNear				= 0.5f;
	zFar				= 5000.0f;

	g_pd3dDevice		= SystemRedKid.GetDevice();
	g_pImmediateContext	= SystemRedKid.GetDeviceContext();

	D3DXMatrixIdentity( &g_ViewProj );
	DeltaTime = 0.0f;

	RendObjCount = RendLightCount = RendTrianglCount = 0;
	//ClearColor = D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f );
	//ClearColor = D3DXVECTOR4( 0.0f, 0.0f, 0.0f, 1.0f );
	ClearColor = D3DXVECTOR4( 0.0f, 0.490196f, 0.772549f, 1.0f );

	//V_( tBuildRendL.Create( BuildRenderData, this ) );

	// PhysX
	PhysXScene			= NULL;
	mCudaContextManager	= NULL;
	mCpuDispatcher		= NULL;
}

CRenderPool::~CRenderPool(void)
{
	Release();
}

void CRenderPool::Release(void)
{
	fInit = false;

	D3DXMatrixIdentity( &g_ViewProj );
	DeltaTime = 0.0f;
	RendObjCount = RendLightCount = RendTrianglCount = 0;

	Interface.ReleaseLP();
	SceneSortForRendring.ReleaseLP();
	m_lLightDirection.ReleaseLP();

	// PhysX
	rELEASE( mCpuDispatcher );
	rELEASE( mCudaContextManager );
	rELEASE( PhysXScene );

	g_pd3dDevice		= NULL;
	g_pImmediateContext	= NULL;

	// PhysX
	rELEASE( mCpuDispatcher );
	rELEASE( mCudaContextManager );
	rELEASE( PhysXScene );
}

HRESULT CRenderPool::Inicialise( void )
{
	if ( fInit )
		return S_FALSE;

	// PhysX
	CudaContextManagerDesc cudaContextManagerDesc;
	mCudaContextManager = createCudaContextManager( cudaContextManagerDesc, &SystemRedKid.GetPhysX()->getProfileZoneManager() );
	if ( mCudaContextManager == NULL )
	{
		LogFile.OutputLogFileErrorString( L"Error mCudaContextManager.", __FILE__, __FUNCTION__, __LINE__  );
		return S_FALSE;
	}

	PxSceneDesc sceneDesc( SystemRedKid.GetPhysX()->getTolerancesScale() );
	sceneDesc.gravity					= PxVec3( 0.0f, -9.81f, 0.0f );
	//sceneDesc.filterShader				= PxFilterFlags();
	//sceneDesc.simulationEventCallback	= this;
	sceneDesc.flags						|= PxSceneFlag::eENABLE_SWEPT_INTEGRATION;
	sceneDesc.flags						|= PxSceneFlag::eDISABLE_SSE  
;

	PxU32 mNbThreads = 4;
	if ( !sceneDesc.cpuDispatcher )
	{
		mCpuDispatcher = PxDefaultCpuDispatcherCreate( mNbThreads );
		if( !mCpuDispatcher )
		{
			LogFile.OutputLogFileErrorString( L"Error PxDefaultCpuDispatcherCreate.", __FILE__, __FUNCTION__, __LINE__  );
			return S_FALSE;
		}
		sceneDesc.cpuDispatcher	= mCpuDispatcher;
	}

	if ( !sceneDesc.filterShader )
		sceneDesc.filterShader = GetSampleFilterShader();

	//if ( mCudaContextManager )
	//	sceneDesc.gpuDispatcher = mCudaContextManager->getGpuDispatcher();

	sceneDesc.flags |= PxSceneFlag::eENABLE_ACTIVETRANSFORMS;
	PhysXScene = SystemRedKid.GetPhysX()->createScene( sceneDesc );
	if ( !PhysXScene )
	{
		LogFile.OutputLogFileErrorString( L"Error createScene.", __FILE__, __FUNCTION__, __LINE__  );
		return S_FALSE;
	}

	fInit = true;
	
	return S_OK;
}

// Расчёт физики
void CRenderPool::FrameCalcPhysics( void )
{
	static float mAccumulator = 0.0f;
	static float mStepSize = 1.0f / 25.0f;
	static PxActiveTransform* sActTr;
	static PxU32 cAcTr;
	static PxRigidDynamic* Actor;

	mAccumulator += DeltaTime;
	if( mAccumulator >= mStepSize )
	{
		mAccumulator -= mStepSize;
		PhysXScene->simulate( mStepSize );
		PhysXScene->fetchResults( true );

		sActTr = PhysXScene->getActiveTransforms( cAcTr );
		for( UINT i = 0; i < cAcTr; i++ )
		{
			Actor = (PxRigidDynamic*)sActTr[i].actor;
			((CObjectPhysX*)(Actor->userData))->UpdatePhysXPose();
		}
	}
}

DWORD CRenderPool::BuildRenderData( void )
{
	g_View = *Camera.GetViewMatrix();
	g_ViewProj = g_View * g_Projection;

	Camera.InicialiseFrustumPlane( &g_Projection );

	#if ( defined( DEBUG ) || defined( _DEBUG ) )
		SignMesh.ResetAdd();
	#endif

	RendObjCount = RendLightCount = RendTrianglCount = 0;
	aLightPoint.ResetAdd();
	FindRenderObject( &Frustum );

	// Direction light
	if ( m_lLightDirection.FindStart() )
	{
		static LIGHT_DIRECTION LightData;
		static CObjectLightDirection* Light;
		aLightDirection.ResetAdd();
		do
		{
			RendLightCount++;

			m_lLightDirection.GetFindObject( Light );
			aLightDirection.Add( LightData );
			LightData.Color = *(Light->GetColor());
			LightData.Direction = *(Light->GetDirection());

			#if ( defined( DEBUG ) || defined( _DEBUG ) )
				if ( Light->GetRenderSign() )
					V_RETURN( Light->AddSignMesh( SignMesh ) );
			#endif
		} while ( m_lLightDirection.FindNext() );
	}

	return 0;
}

void CRenderPool::Render( float DTime, float ElapsedTime )
{
	DeltaTime = DTime;

	FrameCalcPhysics();
	BuildRenderData();

	SystemRedKid.GetViewVariable()->SetMatrix( (float*)g_View );
	SystemRedKid.GetProjectionVariable()->SetMatrix( (float*)g_Projection );
	SystemRedKid.GetViewProjVariable()->SetMatrix( (float*)&g_ViewProj );

	RenderSortList();

	static D3DXMATRIX mVPInv;
	D3DXMatrixInverse( &mVPInv, NULL, &g_ViewProj );
	SystemRedKid.DefferedRenderLight( ElapsedTime, &mVPInv, &aLightPoint, &aLightDirection );
}

void CRenderPool::RenderInterface( void )
{
	#if ( defined( DEBUG ) || defined( _DEBUG ) )
		if ( SignMesh.ResetGet() )
		{
			SystemRedKid.GetDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_LINELIST );
			SystemRedKid.GetDeviceContext()->IASetInputLayout( SystemRedKid.GetLayoutLine() );

			CSignMesh* Sign;
			SystemRedKid.GetTechLine()->GetPassByIndex( 0 )->Apply( 0, SystemRedKid.GetDeviceContext() );
			while ( SignMesh.Get( Sign ) )
				Sign->Draw();
		}
	#endif

	if ( Interface.FindStart() )
	{
		SystemRedKid.GetDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
		SystemRedKid.GetDeviceContext()->IASetInputLayout( SystemRedKid.GetLayoutInterface() );

		static CGameInterface* interf;
		do
		{
			Interface.GetFindObject( interf );
			interf->Draw();
		} while ( Interface.FindNext() );
	}
}

void CRenderPool::RenderSortList( void )
{
	SystemRedKid.GetDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	// Отрисовка сеток
	static UINT MatCount;
	MatCount = SceneSortForRendring.GetCount();
	static UINT MatConcrCount, MeshCount, i1, i2, i3;
	static CMaterialList* Material;
	static CMaterialConcrete* MaterialConcrete;
	static CObjList* ObjList;
	static bool fSetMat, fSetIndivMat;
	static CObjectBase* Obj;
	static CResMesh* Mesh;

	// Обходим все известные материалы
	for ( i1 = 0; i1 < MatCount; i1++ )
	{
		Material = SceneSortForRendring[i1];
		if ( Material != NULL )
		{
			// Обходим все "конкретные материалы"
			fSetMat = false;
			MatConcrCount = Material->ConcreteMaterialList.GetCount();
			for ( i2 = 0; i2 < MatConcrCount; i2++ )
			{
				MaterialConcrete = Material->ConcreteMaterialList[i2];
				if ( MaterialConcrete != NULL )
				{
					fSetIndivMat = false;
					MeshCount = MaterialConcrete->ObjList.GetCount();
					for ( i3 = 0; i3 < MeshCount; i3++ )
					{
						ObjList = MaterialConcrete->ObjList[i3];
						if ( ObjList->Obj.ResetGet() )
						{
							ObjList->Obj.Get( Obj );
							Mesh = ((CObjectMesh*)Obj)->GetMesh();
							Mesh->SetIndexBuffer();

							if ( !fSetMat )
							{
								// Установка основного материала
								Material->Material->SetMaterial();
								fSetMat = true;
							}

							if ( !fSetIndivMat )
							{
								// Установка индивидуальных параметров материала
								CResMat*  mmat;
								mmat = Mesh->GetMaterial();
								mmat->SetMaterialIndividual();
								fSetIndivMat = true;
							}

							// Если есть скелет
							if ( Mesh->GetScelet() )
								do
								{
									Mesh = ((CObjectMesh*)Obj)->GetMesh();
									RendTrianglCount += Mesh->GetTriangleCount();
									Mesh->SetVertexBuffer();
									Mesh->Render();
								}
								while ( ObjList->Obj.Get( Obj ) );
							else
							{
								Mesh->SetVertexBuffer();
								do
								{
									Mesh = ((CObjectMesh*)Obj)->GetMesh();
									RendTrianglCount += Mesh->GetTriangleCount();
									SystemRedKid.GetWorldVariable()->SetMatrix( (float*)Obj->m_GlobalWordMat );
									Mesh->Render();
								}
								while ( ObjList->Obj.Get( Obj ) );
							}

							ObjList->Obj.ResetAdd();
						}
					}
				}
			}
		}
	}
}

HRESULT CRenderPool::Resize( UINT Width, UINT Height )
{
	CGameInterface* Interf;

	D3DXMatrixPerspectiveFovLH( &g_Projection, (float)D3DX_PI/4, (float)Width/(float)Height, zNear, zFar );

	if ( Interface.FindStart() )
		do
		{
			Interface.GetFindObject( Interf );
			Interf->Resize( Width, Height );
		} while( Interface.FindNext() );

	return S_OK;
}

void CRenderPool::FindRenderObject( CFrustumBox* Frustum )
{
	UINT countObj, i;
	static CObjectBase* Obj;

	// Если бокс полностью попадает в фрустум
	if ( Camera.TestAllBoxInsideFrustum( Frustum ) )
		AddAllRenderObject( Frustum );
	else
	{
		// Если камера в боксе или частичное попадание бокса в фрустум
		if ( Camera.TestCameraInsideBox( Frustum ) || Camera.TestBoxInsideFrustum( Frustum ) )
		{
			countObj = Frustum->GetObjectCount();

			#if ( defined( DEBUG ) || defined( _DEBUG ) )
				if ( countObj == 0 )
					SignMesh.Add( Frustum->GetSignGreenMesh() );
				else
					SignMesh.Add( Frustum->GetSignFillMesh() );
			#endif

			Frustum->FindObjStart();
			for ( i = 0; i < countObj; i++ )
			{
				Obj = Frustum->GetFindObject();
				// Если бокс объекта попадает в фрустум
				if ( Camera.TestBoxInsideFrustum( Obj->GetAABB() ) )
					AddOblectForRendring( Obj );

				Frustum->FindNext();
			}

			for ( i = 0; i < 8; i++ )
				if ( Frustum->GetChildFrustum( i ) != NULL )
					FindRenderObject( Frustum->GetChildFrustum( i ) );
		}
	}
}

void CRenderPool::AddAllRenderObject( CFrustumBox* Frustum )
{
	UINT countObj, i;
	static CObjectBase* Obj;

	countObj = Frustum->GetObjectCount();

	#if ( defined( DEBUG ) || defined( _DEBUG ) )
		if ( countObj == 0 )
			SignMesh.Add( Frustum->GetSignGreenMesh() );
		else
			SignMesh.Add( Frustum->GetSignFillMesh() );
	#endif

	Frustum->FindObjStart();
	for ( i = 0; i < countObj; i++ )
	{
		Obj = Frustum->GetFindObject();
		AddOblectForRendring( Obj );
		Frustum->FindNext();
	}

	for ( i = 0; i < 8; i++ )
		if ( Frustum->GetChildFrustum( i ) != NULL )
			AddAllRenderObject( Frustum->GetChildFrustum( i ) );
}

void CRenderPool::AddOblectForRendring( CObjectBase* Obj )
{
	static LPLIGHT_POINT sLightPoint;

	switch ( Obj->GetType() )
	{
	case OBJ_MESH:
		{
			static CResMat* Mat;
			static CResMesh* Mesh;
			Mesh = ((CObjectMesh*)Obj)->GetMesh();
			Mat = Mesh->GetMaterial();
			RendObjCount++;

			// Добавляем новый тип материала
			if ( Mat->GetIndexRenderList() == -1 )
			{
				UINT IndexMaterialList;
				CMaterialList* cMatList = NULL;
				cMatList = new CMaterialList();
				if ( cMatList == NULL )
				{
					LogFile.OutputLogFileErrorString( _T( "Memory allocation failure." ), __FILE__, __FUNCTION__, __LINE__ );
					return;
				}

				if ( S_OK != SceneSortForRendring.Add( cMatList, IndexMaterialList ) )
				{
					delete cMatList;
					LogFile.OutputLogFileErrorString( _T( "Error add in MaterialList." ), __FILE__, __FUNCTION__, __LINE__ );
					return;
				}

				SceneSortForRendring[IndexMaterialList]->Material = Mat;
				Mat->SetIndexRenderList( IndexMaterialList );
			}

			if ( Mat->GetIndexConcrRenderList() == -1 )
			{
				UINT IndexMaterialConcrete;

				CMaterialConcrete* cMaterialConcrete = NULL;
				cMaterialConcrete = new CMaterialConcrete();
				if ( cMaterialConcrete == NULL )
				{
					LogFile.OutputLogFileErrorString( _T( "Memory allocation failure." ), __FILE__, __FUNCTION__, __LINE__ );
					return;
				}

				if ( S_OK != SceneSortForRendring[Mat->GetIndexRenderList()]->ConcreteMaterialList.Add( cMaterialConcrete, IndexMaterialConcrete ) )
				{
					delete cMaterialConcrete;
					LogFile.OutputLogFileErrorString( _T( "Error add in ConcreteMat." ), __FILE__, __FUNCTION__, __LINE__ );
					return;
				}

				Mat->SetIndexConcrRenderList( IndexMaterialConcrete );
			}

			// Добавляем сетку
			static UINT IndexMesh;
			static CObjList* cObjList;
			if ( Mesh->IndexRenderMeshArray == -1 )
			{
				cObjList = NULL;
				cObjList = new CObjList();
				if ( cObjList == NULL )
				{
					LogFile.OutputLogFileErrorString( _T( "Memory allocation failure." ), __FILE__, __FUNCTION__, __LINE__ );
					return;
				}

				if ( S_OK != SceneSortForRendring[Mat->GetIndexRenderList()]->ConcreteMaterialList[Mat->GetIndexConcrRenderList()]->ObjList.Add( cObjList, IndexMesh ) )
				{
					delete cObjList;
					LogFile.OutputLogFileErrorString( _T( "Error add in MeshRenderList." ), __FILE__, __FUNCTION__, __LINE__ );
					return;
				}

				Mesh->IndexRenderMeshArray = IndexMesh;
			}

			SceneSortForRendring[Mat->GetIndexRenderList()]->ConcreteMaterialList[Mat->GetIndexConcrRenderList()]->ObjList[Mesh->IndexRenderMeshArray]->Obj.Add( Obj );
			break;
		}
	case OBJ_LIGHTPOINT:
		{
			RendLightCount++;
			V_( aLightPoint.Add( sLightPoint ) );
			sLightPoint->Position = *Obj->GetPosition();
			sLightPoint->Radius = ((CObjectLightPoint*)Obj)->GetRadius();
			sLightPoint->Color = *(((CObjectLightPoint*)Obj)->GetColor());
			break;
		}
	}

	#if ( defined( DEBUG ) || defined( _DEBUG ) )
		if ( Obj->GetRenderSign() )
			Obj->AddSignMesh( SignMesh );
	#endif
}
