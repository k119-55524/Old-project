#include "RedKid.h"

HRESULT res;
CLogFile LogFile;
CSystem SystemRedKid;
CRedKid RedKid;

D3DXQUATERNION operator * ( D3DXQUATERNION& qIn, D3DXQUATERNION& q )
{
	return D3DXQUATERNION( qIn.w * q.x + qIn.x * q.w + qIn.y * q.z - qIn.z * q.y,
					qIn.w * q.y + qIn.y * q.w + qIn.z * q.x - qIn.x * q.z,
					qIn.w * q.z + qIn.z * q.w + qIn.x * q.y - qIn.y * q.x,
					qIn.w * q.w - qIn.x * q.x - qIn.y * q.y - qIn.z * q.z );
};

float GetFloatRGB( UINT frgb )
{
	if ( frgb > 255 )
		return 1.0f;

	float res = (float)( ( 1.0f / 255.0f ) * frgb );
	return res;
};

CRedKid::CRedKid(void)
{
	ActivRenderPool = NULL;

	pFuncInicialise		= NULL;
	pFuncPreFrameRender	= NULL;
}

CRedKid::~CRedKid(void)
{
	aRenderPool.ReleaseLP();
	g_cDInput.Release();

	pFuncInicialise		= NULL;
	pFuncPreFrameRender	= NULL;
}

void CRedKid::Release( void )
{
	aRenderPool.ReleaseLP();
	g_cDInput.Release();

	pFuncInicialise		= NULL;
	pFuncPreFrameRender	= NULL;

	SystemRedKid.Release();
}

HRESULT CRedKid::Inicialise( HINSTANCE hInstance, PxDefaultAllocator &PhysXDefAlloc, PxErrorCallback &PhysXDefError, D3D_FEATURE_LEVEL cLev )
{
	if ( SystemRedKid.GetfInit() )
		return S_FALSE;

	V_RETURN( SystemRedKid.Inicialise( hInstance, PhysXDefAlloc, PhysXDefError, cLev ) );
	V_RETURN( g_cDInput.Inicialise( SystemRedKid.GetHWND(), SystemRedKid.GetHinstance() ) );

	try
	{
		CRenderPool* RenderPool = NULL;
		RenderPool = new CRenderPool();
		if ( RenderPool == NULL )
		{
			LogFile.OutputLogFileErrorString( L"Memory allocation failure.", __FILE__, __FUNCTION__, __LINE__  );
			return S_FALSE;
		}

		V_RETURN( RenderPool->Inicialise() );

		if ( S_OK != aRenderPool.Add( RenderPool ) )
		{
			delete RenderPool;
			LogFile.OutputLogFileErrorString( L"Error add RenderPool.", __FILE__, __FUNCTION__, __LINE__  );
			return S_FALSE;
		}

		ActivRenderPool = RenderPool;

		if ( pFuncInicialise != NULL )
		{
			LogFile.OutputLogFileString( L"User inicialise: START." );
			V_RETURN( pFuncInicialise() );
			LogFile.OutputLogFileString( L"User inicialise: OK." );
		}
	}
	catch( ... )
	{
		LogFile.OutputLogFileErrorString( L"Unknown error inicialise RedKid3D(catch ... RedKid class).", __FILE__, __FUNCTION__, __LINE__  );
		return S_FALSE;
	}

	return S_OK;
}

HRESULT CRedKid::Resize( UINT Width, UINT Height )
{
	LogFile.OutputLogFileString( L"Resize RedKid: START." );

	for ( UINT i = 0; i < aRenderPool.GetCount(); i++ )
		if ( aRenderPool[i] != NULL )
			V_RETURN( aRenderPool[i]->Resize( Width, Height ) );

	LogFile.OutputLogFileString( L"Resize RedKid: End." );

	return S_OK;
}

HRESULT CRedKid::FrameRender( float DeltaTime, float ElapsedTime )
{
	g_cDInput.FixedInputStatus();

	// Defferd render
	SystemRedKid.OMSetDefferedRTAndClear( ActivRenderPool->GetClearColor() );

	if ( pFuncPreFrameRender != NULL )
		pFuncPreFrameRender( DeltaTime );

	ActivRenderPool->Render( DeltaTime, ElapsedTime );
	SystemRedKid.RenderFinalSceneQuad();
	ActivRenderPool->RenderInterface();

	SystemRedKid.Present();
	
	return S_OK;
}

HRESULT CRedKid::AddObjectMesh( CObjectBase* &Obj, TCHAR* Name, CObjectBase* ParentObj, UINT MeshIndex, bool fRenderSign )
{
	Obj = NULL;

	if ( !SystemRedKid.GetfInit() )
		return S_FALSE;

	if ( MeshIndex >= cResManager.aMesh.GetCount() || cResManager.aMesh[MeshIndex] == NULL )
	{
		LogFile.OutputLogFileErrorString( _T( "Not a correct index of a grid." ), __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}

	Obj = new CObjectMesh( Name );
	if ( Obj == NULL )
	{
		LogFile.OutputLogFileErrorString( L"Memory allocation failure.", __FILE__, __FUNCTION__, __LINE__  );
		return S_FALSE;
	}

	if ( S_OK != ((CObjectMesh*)Obj)->AddMesh( cResManager.aMesh[MeshIndex] ) )
	{
		delete Obj;
		return S_FALSE;
	}

	ActivRenderPool->AddObject( Obj, ParentObj );
	Obj->SetRenderSign( fRenderSign );

	return S_OK;
}

HRESULT CRedKid::AddPointLight( TCHAR* Name, D3DXVECTOR3 &Pos, D3DXVECTOR4 &Color, float Radius, CObjectBase* ParentObj, bool fRenderSign )
{
	if ( !SystemRedKid.GetfInit() )
		return S_FALSE;

	CObjectBase* Light = NULL;
	Light = new CObjectLightPoint( Name, &Pos, &Color, Radius );
	if ( Light == NULL )
	{
		LogFile.OutputLogFileErrorString( L"Memory allocation failure.", __FILE__, __FUNCTION__, __LINE__  );
		return S_FALSE;
	}

	Light->SetRenderSign( fRenderSign );

	ActivRenderPool->AddObject( Light, ParentObj );

	return S_OK;
}

HRESULT CRedKid::AddDirectionLight( TCHAR* Name, CObjectBase* &Light, D3DXVECTOR4 &Color, bool fRenderSign, float SignScale )
{
	if ( !SystemRedKid.GetfInit() )
		return S_FALSE;

	Light = NULL;
	Light = new CObjectLightDirection( Name, &Color, SignScale );
	if ( Light == NULL )
	{
		LogFile.OutputLogFileErrorString( L"Memory allocation failure.", __FILE__, __FUNCTION__, __LINE__  );
		return S_FALSE;
	}

	Light->SetRenderSign( fRenderSign );
	Light->CalcAABB();

	if ( S_OK != ActivRenderPool->AddDirectionLight( (CObjectLightDirection*)Light ) )
	{
		delete Light;
		LogFile.OutputLogFileErrorString( L"Error add direction light in active render pool.", __FILE__, __FUNCTION__, __LINE__  );
		return S_FALSE;
	}

	return S_OK;
}

HRESULT CRedKid::AddInterfaceText( CGameInterface* &Text, UINT Line, TCHAR* TDFName, TCHAR* TextName )
{
	CResourceTexture* Texture;
	V_RETURN( cResManager.GetTexture( TextName, Texture ) );

	Text = NULL;
	Text = new CGameInterface( INTERFACE_TEXT );
	if ( Text == NULL )
	{
		LogFile.OutputLogFileErrorString( L"Memory allocation failure.", __FILE__, __FUNCTION__, __LINE__  );
		return S_FALSE;
	}

	if ( S_OK != Text->InitTextInterface( SystemRedKid.GetWidth(), SystemRedKid.GetHeight(), Line, TDFName, Texture ) )
	{
		delete Text;
		return S_FALSE;
	}

	if ( S_OK != ActivRenderPool->AddInterface( Text ) )
	{
		delete Text;
		return S_FALSE;
	}

	return S_OK;
}

HRESULT CRedKid::AddPhysXTriangleStaticMeshObject( CObjectBase* &Obj, TCHAR* Name, TCHAR* FileName, UINT IndexMat, CObjectBase* ParentObj, bool fRenderSign )
{
	if ( !SystemRedKid.GetfInit() )
		return S_FALSE;

	Obj = NULL;

	UINT IndexMesh;
	V_RETURN( cResManager.CreatePhysXStaticTriangleMeshOfTXTFile( FileName, IndexMat, IndexMesh ) );

	Obj = new CObjectPhysXTriangleStaticMesh( Name );
	if ( Obj == NULL )
	{
		LogFile.OutputLogFileErrorString( L"Memory allocation failure.", __FILE__, __FUNCTION__, __LINE__  );
		return S_FALSE;
	}

	Obj->SetRenderSign( fRenderSign );

	if ( S_OK != ((CObjectPhysXTriangleStaticMesh*)Obj)->AddMesh( cResManager.aMesh[IndexMesh], ActivRenderPool->GetPhysXScene() ) )
	{
		delete Obj;
		LogFile.OutputLogFileErrorString( L"Error AddMesh.", __FILE__, __FUNCTION__, __LINE__  );
		return S_FALSE;
	}

	V_RETURN( ActivRenderPool->AddObject( Obj, ParentObj ) );

	return S_OK;
}

HRESULT CRedKid::AddPhysXDinamicConvexMeshObject( CObjectBase* &Obj, TCHAR* Name, TCHAR* FileName, UINT IndexMat, CObjectBase* ParentObj, bool fRenderSign )
{
	if ( !SystemRedKid.GetfInit() )
		return S_FALSE;

	Obj = NULL;

	UINT IndexMesh;
	V_RETURN( cResManager.CreatePhysXDinamicConvexMeshOfTXTFile( FileName, IndexMat, IndexMesh ) );

	Obj = new CObjectPhysXConvexDinamicMesh( Name );
	if ( Obj == NULL )
	{
		LogFile.OutputLogFileErrorString( L"Memory allocation failure.", __FILE__, __FUNCTION__, __LINE__  );
		return S_FALSE;
	}

	Obj->SetRenderSign( fRenderSign );

	if ( S_OK != ((CObjectPhysXConvexDinamicMesh*)Obj)->AddMesh( cResManager.aMesh[IndexMesh], ActivRenderPool->GetPhysXScene() ) )
	{
		delete Obj;
		LogFile.OutputLogFileErrorString( L"Error AddMesh.", __FILE__, __FUNCTION__, __LINE__  );
		return S_FALSE;
	}

	V_RETURN( ActivRenderPool->AddObject( Obj, ParentObj ) );

	return S_OK;
}