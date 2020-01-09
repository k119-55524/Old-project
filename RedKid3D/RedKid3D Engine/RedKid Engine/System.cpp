#include "System.h"
#include "RedKid.h"

CSystem::CSystem(void)
{
	// PhysX 3.1
	mPhysX						= NULL;
	mCooking					= NULL;
	//mProfileZone				= NULL;

	fInit = false;
	fPause = false;
	mFps = 0;

	hWnd = NULL;
	hInst = NULL;

	g_pFactory					= NULL;
	g_pd3dDevice				= NULL;
	g_pImmediateContext			= NULL;
	g_pSwapChain				= NULL;
	g_pRenderScreenTargetView	= NULL;
	g_pDepthStencilView			= NULL;
	g_pDepthStencilSRV			= NULL;

	g_pEffect					= NULL;
	g_pSMAATechLumaEdgeDet		= NULL;
	g_pSMAATechBlendWeightCalc	= NULL;
	g_pSMAANeighborhoodBlending = NULL;
	g_pTechTexture				= NULL;
	g_pTechLine					= NULL;
	g_pTechInterface			= NULL;
	g_pTechTextureBone			= NULL;

	g_pTechRenderLight			= NULL;

	g_pInputLayoutLine			= NULL;
	g_pInputLayoutQuad			= NULL;
	g_pInpLayoutInterface		= NULL;
	g_pInputLayoutTexture		= NULL;
	g_pInputLayoutTextureBone	= NULL;

	g_pWorldVariable			= NULL;
	g_pViewVariable				= NULL;
	g_pProjectionVariable		= NULL;
	g_pViewProjVariable			= NULL;
	g_DiffuseTexture			= NULL;
	g_DepthTexture				= NULL;

	g_pCSSceletNormalCopy		= NULL;
	g_pCSSceletTBNCopy			= NULL;
	g_pCSSceletNormalAnim		= NULL;
	g_pCSSceletTBNAnim			= NULL;
	g_pCSCopyCalcAABB			= NULL;
	g_pCSCalcAABB				= NULL;
	g_pCSRenderLight			= NULL;
	g_pCSCalcSummLum			= NULL;
	g_pCSCalcAdaptedLum			= NULL;

	g_pVBQuad					= NULL;

	g_pAreaTexture				= NULL;
	g_pSearchTexture			= NULL;
	g_pAreaTextureSRV			= NULL;
	g_pSearchTextureSRV			= NULL;

	for( UINT i = 0; i < NUM_DEFFERED_CONTEXT; i++ )
		g_pDefferedContext[i] = NULL;
}

CSystem::~CSystem(void)
{
	Release();
}

void CSystem::Release( void )
{
	// Free PhysX
	//rELEASE( mProfileZone );
	rELEASE( mCooking );
	rELEASE( mPhysX );

	aAdapter.ReleaseLP();

	RELEASE( g_pAreaTexture );
	RELEASE( g_pSearchTexture );
	RELEASE( g_pAreaTextureSRV );
	RELEASE( g_pSearchTextureSRV );

	RELEASE( g_pCSCalcAdaptedLum );
	RELEASE( g_pCSCalcSummLum );
	RELEASE( g_pCSRenderLight );
	RELEASE( g_pCSCopyCalcAABB );
	RELEASE( g_pCSCalcAABB );
	RELEASE( g_pCSSceletTBNAnim );
	RELEASE( g_pCSSceletNormalAnim );
	RELEASE( g_pCSSceletTBNCopy );
	RELEASE( g_pCSSceletNormalCopy );

	RELEASE( g_pInputLayoutLine );
	RELEASE( g_pInputLayoutTextureBone );
	RELEASE( g_pInpLayoutInterface );
	RELEASE( g_pInputLayoutTexture );
	RELEASE( g_pInputLayoutQuad );

	g_pTechLine					= NULL;
	g_pSMAATechLumaEdgeDet		= NULL;
	g_pSMAATechBlendWeightCalc	= NULL;
	g_pSMAANeighborhoodBlending = NULL;
	g_pTechTexture				= NULL;
	g_pTechInterface			= NULL;
	g_pTechTextureBone			= NULL;
	g_pTechRenderLight			= NULL;
	RELEASE( g_pEffect );

	g_pWorldVariable			= NULL;
	g_pViewVariable				= NULL;
	g_pProjectionVariable		= NULL;
	g_pViewProjVariable			= NULL;
	g_DiffuseTexture			= NULL;
	g_DepthTexture				= NULL;

	RELEASE( g_pVBQuad );

	RELEASE( g_pDepthStencilSRV );
	RELEASE( g_pDepthStencilView );
	RELEASE( g_pRenderScreenTargetView );
	RELEASE( g_pSwapChain );

	for( UINT i = 0; i < NUM_DEFFERED_CONTEXT; i++ )
		RELEASE( g_pDefferedContext[i] );

	RELEASE( g_pImmediateContext );
	RELEASE( g_pd3dDevice );
	RELEASE( g_pFactory );

	fInit = false;
	fPause = false;

	hInst	= NULL;
	hWnd	= NULL;
}

HRESULT CSystem::CompartibleIni( void )
{
	UINT Index = 0;
	bool fFind = false;
	while( Index < aAdapter.GetCount() )
	{
		if ( Index == AppIni.GetNumAdapter() )
		{
			fFind = true;
			break;
		};

		Index++;
	}
	if ( fFind == false )
	{
		LogFile.OutputLogFileErrorString( L"Error find num adapters in INI file.", __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}
	LogFile.OutputLogFileString( L"Find compartible adapter in INI file: OK." );

	if ( AppIni.GetNumMonitor() > aAdapter[Index]->GetMonitorCount() )
	{
		LogFile.OutputLogFileErrorString( L"Error find num output monitor in INI file.", __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}
	LogFile.OutputLogFileString( L"Find compartible output monitor in INI file: OK." );

	CMonitorData* Monitor = aAdapter[AppIni.GetNumAdapter()]->GetMonitor( AppIni.GetNumMonitor() );
	if ( Monitor == NULL )
	{
		LogFile.OutputLogFileErrorString( L"Error NumMonitor.", __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}

	Index = Monitor->GetNumModes();
	if ( Index == 0 )
	{
		LogFile.OutputLogFileErrorString( L"Num mode desc = 0.", __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}

	if ( AppIni.GetWindowed() == false )
	{
		LogFile.OutputLogFileString( L"Windowed = false. Find compartible mode desc in INI file: START." );
		DXGI_MODE_DESC* ModeDesc = Monitor->GetModeDesc();
		fFind = false;
		while ( Index > 0 )
		{
			// Проверяем разрешение экрана
			if ( AppIni.GetWidth() == ModeDesc->Width && AppIni.GetHeight() == ModeDesc->Height )
			{
				fFind = true;
				break;
			}

			ModeDesc++;
			Index--;
		}
		if ( fFind == false )
		{
			LogFile.OutputLogFileErrorString( L"Error find mode desc in INI file.", __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}
		LogFile.OutputLogFileString( L"Find compartible mode desc in INI file: OK." );
	}
		
	return S_OK;
};

HRESULT CSystem::InicialiseForEditor( HWND Wnd, PxDefaultAllocator &PhysXDefAlloc, PxErrorCallback &PhysXDefError, D3D_FEATURE_LEVEL cLev )
{
	try
	{
		LogFile.ClearLogFile();
		LogFile.OutputLogFileString( L"Inicialise engine RedKid3Dfor editor: START." );

		hWnd = Wnd;

		V_RETURN( Inicialise3D( cLev ) );
		V_RETURN( InicialisePhysX( PhysXDefAlloc, PhysXDefError ) );
	}
	catch( ... )
	{
		LogFile.OutputLogFileErrorString( L"Unknown error inicialise RedKid3D(catch ... SystemRedKid class).", __FILE__, __FUNCTION__, __LINE__  );
		return S_FALSE;
	}

	return S_OK;
}

HRESULT CSystem::Inicialise( HINSTANCE hInstance, PxDefaultAllocator &PhysXDefAlloc, PxErrorCallback &PhysXDefError, D3D_FEATURE_LEVEL cLev )
{
	try
	{
		LogFile.ClearLogFile();
		LogFile.OutputLogFileString( L"Inicialise engine RedKid3D: START." );

		hInst = hInstance;

		// Регистрируем класс окна
		WNDCLASSEX wc;
		wc.cbSize			= sizeof( WNDCLASSEX );
		wc.style			= CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc		= WindowProc;
		wc.cbClsExtra		= 0;
		wc.cbWndExtra		= 0;
		wc.hInstance		= hInstance;
		//wc.hIcon			= LoadIcon( hInstance, MAKEINTRESOURCE( IDI_ICO_WND ) );
		wc.hIcon			= NULL;
		wc.hCursor			= LoadCursor( NULL, IDC_ARROW );
		wc.hbrBackground	= (HBRUSH)COLOR_WINDOW;
		wc.lpszMenuName		= NULL;
		wc.lpszClassName	= WndClassName;
		//wc.hIconSm			= LoadIcon( hInstance, MAKEINTRESOURCE( IDI_ICO_WND ) );
		wc.hIconSm			= NULL;

		if( !RegisterClassEx( &wc) )
		{
			LogFile.OutputLogFileErrorString( L"No create windows class.", __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}

		// Читаем INI файл
		if ( S_OK != AppIni.Load() )
		{
			LogFile.OutputLogFileString( L"Inicialise INI file: ERROR LOAD." );
			LogFile.OutputLogFileString( L"Inicialise INI: RESET." );
			AppIni.Reset();
		};

		// Создаём окно
		// Use MDICREATESTRUCT to support MDI child window
		MDICREATESTRUCT mdic;
		memset(&mdic, 0, sizeof(mdic));
		mdic.lParam = (LPARAM)this;

		hWnd = CreateWindowEx( 0, WndClassName, WndClassName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			AppIni.GetWidth(), AppIni.GetHeight(), NULL, NULL, hInstance, &mdic );

		if( !hWnd )
		{
			LogFile.OutputLogFileErrorString( L"No create window.", __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}
		LogFile.OutputLogFileString( L"Create windows: OK." );

		V_RETURN( Inicialise3D( cLev ) );
		V_RETURN( InicialisePhysX( PhysXDefAlloc, PhysXDefError ) );
	}
	catch( ... )
	{
		LogFile.OutputLogFileErrorString( L"Unknown error inicialise RedKid3D(catch ... SystemRedKid class).", __FILE__, __FUNCTION__, __LINE__  );
		return S_FALSE;
	}

	fInit = true;
	LogFile.OutputLogFileString( L"Inicialise engine RedKid3D: END." );

	return S_OK;
}

HRESULT CSystem::Inicialise3D( D3D_FEATURE_LEVEL cLev )
{
	LogFile.OutputLogFileString( _T( "Inicialise 3D graph system: End." ) );

	// Создаём фабрику
	V_RETURN( CreateDXGIFactory1( __uuidof(IDXGIFactory), (void**)(&g_pFactory) ) );
	LogFile.OutputLogFileString( _T( "Create IDXGIFactory1: OK." ) );

	V_RETURN( Scan3DSystem() );
	// Проверяем данные INI файла с возможностями системы
	V_RETURN( CompartibleIni() );

	LogFile.OutputLogFileString( _T( "Inicialise 3D graph system: End." ) );

	// Получаем адаптер соответствующий настройкам
	IDXGIAdapter1* g_pAdapter = aAdapter[AppIni.GetNumAdapter()]->GetAdapter();

	UINT flagCreate = 0;//D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_SINGLETHREADED;
	#if defined( DEBUG ) || defined( _DEBUG )
		flagCreate = flagCreate | D3D11_CREATE_DEVICE_DEBUG;
	#endif;

	D3D_FEATURE_LEVEL FeatureLevels[] = { D3D_FEATURE_LEVEL_11_0,
										  D3D_FEATURE_LEVEL_10_1,
										  D3D_FEATURE_LEVEL_10_0 };

    V_RETURN( D3D11CreateDevice(
		g_pAdapter,
		D3D_DRIVER_TYPE_UNKNOWN,
		NULL,
		flagCreate,
		FeatureLevels,
		3,
		D3D11_SDK_VERSION,
		&g_pd3dDevice,
		&FeatureLevel,
		&g_pImmediateContext ) );

	switch ( FeatureLevel )
	{
	case D3D_FEATURE_LEVEL_11_0:
		LogFile.OutputLogFileString( _T( "Create compartible DX11 device(D3D_FEATURE_LEVEL_11_0): OK." ) );
		break;
	case D3D_FEATURE_LEVEL_10_1:
		if ( cLev == D3D_FEATURE_LEVEL_11_0 )
		{
			LogFile.OutputLogFileErrorString( _T( "No create D3D_FEATURE_LEVEL_11_0 level." ), __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}
		LogFile.OutputLogFileString( _T( "Create compartible DX11 device(D3D_FEATURE_LEVEL_10_1): OK." ) );
		break;
	case D3D_FEATURE_LEVEL_10_0:
		if ( cLev == D3D_FEATURE_LEVEL_11_0 )
		{
			LogFile.OutputLogFileErrorString( _T( "No create D3D_FEATURE_LEVEL_11_0 level." ), __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}
		if ( cLev == D3D_FEATURE_LEVEL_10_1 )
		{
			LogFile.OutputLogFileErrorString( _T( "No create D3D_FEATURE_LEVEL_10_1 level." ), __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}
		LogFile.OutputLogFileString( _TEXT( "Create compartible DX11 device(D3D_FEATURE_LEVEL_10_0): OK." ) );
		break;
	}

	// Create deferred context
	for( UINT i = 0; i < NUM_DEFFERED_CONTEXT; i++ )
		V_RETURN( g_pd3dDevice->CreateDeferredContext( 0, &g_pDefferedContext[i] ) );

	DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory( &scd, sizeof( scd ) );
	scd.BufferDesc.Width = AppIni.GetWidth();
	scd.BufferDesc.Height = AppIni.GetHeight();
	scd.BufferDesc.Format = FORMAT_RENDER_BUFFER;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 1;
	scd.OutputWindow = hWnd;
	scd.Windowed = true;
	V_RETURN( g_pFactory->CreateSwapChain( g_pd3dDevice, &scd, &g_pSwapChain) );

	V_RETURN( InicialiseShader() );
	V_RETURN( CreateComputeShader( _T( "RedKid resource\\Shaders\\CSSceletNormalCopy.hlsl" ), "CSSceletNormalCopy", g_pCSSceletNormalCopy ) );
	V_RETURN( CreateComputeShader( _T( "RedKid resource\\Shaders\\CSSceletTBNCopy.hlsl" ), "CSSceletTBNCopy", g_pCSSceletTBNCopy ) );
	V_RETURN( CreateComputeShader( _T( "RedKid resource\\Shaders\\CSSceletNormalAnim.hlsl" ), "CSSceletNormalAnim", g_pCSSceletNormalAnim ) );
	V_RETURN( CreateComputeShader( _T( "RedKid resource\\Shaders\\CSSceletTBNAnim.hlsl" ), "CSSceletTBNAnim", g_pCSSceletTBNAnim ) );
	V_RETURN( CreateComputeShader( _T( "RedKid resource\\Shaders\\CS Calc AABB.hlsl" ), "CS_CopyCalcAABB", g_pCSCopyCalcAABB ) );
	V_RETURN( CreateComputeShader( _T( "RedKid resource\\Shaders\\CSSceletNormalAnim.hlsl" ), "CS_CopyCalcAABB_Scelet", g_pCSCopyCalcAABB_Scelet ) );
	V_RETURN( CreateComputeShader( _T( "RedKid resource\\Shaders\\CS Calc AABB.hlsl" ), "CS_CalcAABB", g_pCSCalcAABB ) );
	V_RETURN( CreateComputeShader( _T( "RedKid resource\\Shaders\\CS Deffered RenderLight.hlsl" ), "CS_RenderLight", g_pCSRenderLight ) );
	V_RETURN( CreateComputeShader( _T( "RedKid resource\\Shaders\\CS Calc SummLum.hlsl" ), "CS_CalcSummLum", g_pCSCalcSummLum ) );
	V_RETURN( CreateComputeShader( _T( "RedKid resource\\Shaders\\CS Calc AdaptedLum.hlsl" ), "CS_CalcAdaptedLum", g_pCSCalcAdaptedLum ) );
	V_RETURN( CreateComputeShader( _T( "RedKid resource\\Shaders\\CS BloomGaus.hlsl" ), "CS_BloomGaus", g_pCSBloomGaus ) );

	V_RETURN( c_CBRenderLight.Create( sizeof( sDefferedRenderData ) ) );

	V_RETURN( CreateAreaTexture() );
	V_RETURN( CreateSearchTexture() );

    VERTEX_QUAD vb[4];
	vb[0].Pos = D3DXVECTOR3( -1.0f, 1.0f, 0.5f );
	vb[0].Tex = D3DXVECTOR2( 0.0f, 0.0f );
	vb[1].Pos = D3DXVECTOR3( 1.0f, 1.0f, 0.5f );
	vb[1].Tex = D3DXVECTOR2( 1.0f, 0.0f );
	vb[2].Pos = D3DXVECTOR3( -1.0f, -1.0f, 0.5f );
	vb[2].Tex = D3DXVECTOR2( 0.0f, 1.0f );
	vb[3].Pos = D3DXVECTOR3( 1.0f, -1.0f, 0.5f );
	vb[3].Tex = D3DXVECTOR2( 1.0f, 1.0f );

	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.ByteWidth = sizeof( VERTEX_QUAD ) * 4;	// Размер буффера в байтах

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vb;
	V_RETURN( g_pd3dDevice->CreateBuffer( &bd, &InitData, &g_pVBQuad ) );

	V_RETURN( g_pBufferCalcAABB.Create( sizeof(float) * 3, 2 ) );
	V_RETURN( g_pBufferAABBData.Create( sizeof( D3DXMATRIX ) ) );
	V_RETURN( g_pBufferReadAABB.Create( sizeof(float) * 3, 2 ) );
	V_RETURN( g_pHDRData.Create( sizeof(sHDRCalcData) ) );
	V_RETURN( g_pHDRPrevLum.Create( sizeof(float), 1 ) );

	return S_OK;
}

HRESULT CSystem::InicialiseShader( void )
{
	TCHAR* ShaderFileName = _T( "RedKid resource\\Shaders\\HLSL Shaders.hlsl" );

	TCHAR mESS[1024]; 
	wsprintf( mESS, _T( "Start create shader: %s." ), ShaderFileName );
	LogFile.OutputLogFileString( mESS );

	ID3D10Blob* pErrorBlob = NULL;
	ID3D10Blob*	pShaderBlob= NULL;

	DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
	#if defined( DEBUG ) || defined( _DEBUG )
		dwShaderFlags |= D3D10_SHADER_DEBUG;
	#endif

	res = D3DX11CompileFromFile(
		ShaderFileName,		// Имя файла эффектов
		NULL,				// Макросы шейдера
		NULL,				// Указатель на интерфейс обрабатывающий файл
		NULL,				// Название функции с которой начинается выполнение шейдера
		"fx_5_0",			// Модель шейдера
		dwShaderFlags,		// Флаги компиляции шейдеров и типов данных
		NULL,				// Фдаги компиляции относящиеся к эффектам
		NULL,				// Указатель потока
		&pShaderBlob,		// Адрес указателя на текст 
		&pErrorBlob,		// Адрес указателя на текст в случае ошибки компиляции
		NULL);				// Код завершения функции

	if ( res != S_OK )
	{
		if ( pErrorBlob )
		{
			char* ErrString = NULL;
			ErrString = (char*)pErrorBlob->GetBufferPointer();
			wsprintf( mESS, _T( "Error cteate shader of file: %s." ), ShaderFileName );
			LogFile.OutputLogFileErrorString( mESS, __FILE__, __FUNCTION__, __LINE__ );
		}
		else
			LogFile.OutputLogFileErrorString( _T( "Error D3DX11CompileFromFile." ), __FILE__, __FUNCTION__, __LINE__ );

		RELEASE( pErrorBlob );
		RELEASE( pShaderBlob );
		return S_FALSE;
	}

	LogFile.OutputLogFileString( L"Compile shader: OK." );

	res = D3DX11CreateEffectFromMemory( pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), 0, g_pd3dDevice, &g_pEffect );
	RELEASE( pShaderBlob );
	if ( res != S_OK )
	{
		Release();
		LogFile.OutputLogFileErrorString( L"Error D3DX11CreateEffectFromMemory.", __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}
	RELEASE( pShaderBlob );
	LogFile.OutputLogFileString( L"Create Effect: OK." );

	g_pSMAATechLumaEdgeDet = g_pEffect->GetTechniqueByName( "SMAALumaEdgeDetection" );
	//g_pSMAATechLumaEdgeDet = g_pEffect->GetTechniqueByName( "SMAAColorEdgeDetection" );
	//g_pSMAATechLumaEdgeDet = g_pEffect->GetTechniqueByName( "SMAADepthEdgeDetection" );
	if ( !g_pSMAATechLumaEdgeDet->IsValid() )
	{
		Release();
		LogFile.OutputLogFileString( L"Error: GetTechniqueByName(SMAALumaEdgeDetection): No IsValid." );
		return S_FALSE;
	}
	LogFile.OutputLogFileString( L"GetTechniqueByName(SMAALumaEdgeDetection): OK." );

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	g_pTechRenderLight = g_pEffect->GetTechniqueByName( "RenderLight" );
	if ( !g_pTechRenderLight->IsValid() )
	{
		Release();
		LogFile.OutputLogFileString( L"Error: GetTechniqueByName(RenderLight): No IsValid." );
		return S_FALSE;
	}
	LogFile.OutputLogFileString( L"GetTechniqueByName(RenderLight): OK." );
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	g_pSMAATechBlendWeightCalc = g_pEffect->GetTechniqueByName( "SMAABlendingWeightCalculation" );
	if ( !g_pSMAATechBlendWeightCalc->IsValid() )
	{
		Release();
		LogFile.OutputLogFileString( L"Error: GetTechniqueByName(SMAABlendingWeightCalculation): No IsValid." );
		return S_FALSE;
	}
	LogFile.OutputLogFileString( L"GetTechniqueByName(SMAABlendingWeightCalculation): OK." );

	g_pSMAANeighborhoodBlending = g_pEffect->GetTechniqueByName( "SMAANeighborhoodBlending" );
	if ( !g_pSMAANeighborhoodBlending->IsValid() )
	{
		Release();
		LogFile.OutputLogFileString( L"Error: GetTechniqueByName(SMAANeighborhoodBlending): No IsValid." );
		return S_FALSE;
	}
	LogFile.OutputLogFileString( L"GetTechniqueByName(SMAANeighborhoodBlending): OK." );

	g_pTechTexture = g_pEffect->GetTechniqueByName( "RenderTexture" );
	if ( !g_pTechTexture->IsValid() )
	{
		Release();
		LogFile.OutputLogFileString( L"Error: GetTechniqueByName(RenderTexture): No IsValid." );
		return S_FALSE;
	}
	LogFile.OutputLogFileString( L"GetTechniqueByName(RenderTexture): OK." );

	g_pTechTextureBone = g_pEffect->GetTechniqueByName( "RenderTextureBone" );
	if ( !g_pTechTextureBone->IsValid() )
	{
		Release();
		LogFile.OutputLogFileString( L"Error: GetTechniqueByName(RenderTextureBone): No IsValid." );
		return S_FALSE;
	}
	LogFile.OutputLogFileString( L"GetTechniqueByName(RenderTextureBone): OK." );

	g_pTechLine = g_pEffect->GetTechniqueByName( "RenderLine" );
	if ( !g_pTechLine->IsValid() )
	{
		Release();
		LogFile.OutputLogFileString( L"Error: GetTechniqueByName(g_pTechLine): No IsValid." );
		return S_FALSE;
	}
	LogFile.OutputLogFileString( L"GetTechniqueByName(RenderLine): OK." );

	g_pTechInterface = g_pEffect->GetTechniqueByName( "RenderInterface" );
	if ( !g_pTechInterface->IsValid() )
	{
		Release();
		LogFile.OutputLogFileString( L"Error: GetTechniqueByName(RenderInterface): No IsValid." );
		return S_FALSE;
	}
	LogFile.OutputLogFileString( L"GetTechniqueByName(RenderInterface): OK." );

	// Получаем указатель на сигнатуру шейдера
	D3DX11_PASS_DESC PassDesc;
	res = g_pTechTexture->GetPassByIndex( 0 )->GetDesc( &PassDesc );
	if ( S_OK != res )
	{
		LogFile.OutputLogFileErrorString( L"Error GetDesc g_pTechnique.", __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}

	////////////////////////////////////////////////////////////////
	//
	D3D11_INPUT_ELEMENT_DESC layoutLine[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	 0, 0,							  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",	  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = sizeof(layoutLine)/sizeof(layoutLine[0]);

	V_RETURN( g_pTechLine->GetPassByIndex( 0 )->GetDesc( &PassDesc ) );
	V_RETURN( g_pd3dDevice->CreateInputLayout( layoutLine, numElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &g_pInputLayoutLine ) );

	LogFile.OutputLogFileString( L"Create input g_pInputLayoutLine: OK." );
	//
	////////////////////////////////////////////////////////////////
	//
	D3D11_INPUT_ELEMENT_DESC layoutInterface[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,							   D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	numElements = sizeof(layoutInterface)/sizeof(layoutInterface[0]);

	V_RETURN( g_pTechInterface->GetPassByIndex( 0 )->GetDesc( &PassDesc ) );
	V_RETURN( g_pd3dDevice->CreateInputLayout( layoutInterface, numElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize,	&g_pInpLayoutInterface ) );

	LogFile.OutputLogFileString( L"Create input layoutInterface: OK." );
	//
	////////////////////////////////////////////////////////////////

	D3D11_INPUT_ELEMENT_DESC layoutQuad[] =
	{
		{ "POSITION",	  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,								  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	  0, DXGI_FORMAT_R32G32_FLOAT,		 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	numElements = sizeof( layoutQuad ) / sizeof( layoutQuad[0] );

	// Получаем указатель на сигнатуру шейдера
	V_RETURN( g_pSMAATechLumaEdgeDet->GetPassByIndex( 0 )->GetDesc( &PassDesc ) );
	V_RETURN( g_pd3dDevice->CreateInputLayout(
		layoutQuad,						// Массив описания типов данных передаваемых на первую стадию графичесского конвеера
		numElements,					// Количество описаний типов данных
		PassDesc.pIAInputSignature,		// Указатель на сигнатуру входных данных шейдера
		PassDesc.IAInputSignatureSize,	// размер сигнатуры в байтах
		&g_pInputLayoutQuad ) );		// Адрес указателя в который будет помещён созданный объект
	LogFile.OutputLogFileString( L"Create input g_pInputLayoutQuad: OK." );

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,								  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	  0, DXGI_FORMAT_R32G32_FLOAT,		 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	numElements = sizeof( layout ) / sizeof( layout[0] );

	// Получаем указатель на сигнатуру шейдера
	V_RETURN( g_pTechTexture->GetPassByIndex( 0 )->GetDesc( &PassDesc ) );
	V_RETURN( g_pd3dDevice->CreateInputLayout(
		layout,							// Массив описания типов данных передаваемых на первую стадию графичесского конвеера
		numElements,					// Количество описаний типов данных
		PassDesc.pIAInputSignature,		// Указатель на сигнатуру входных данных шейдера
		PassDesc.IAInputSignatureSize,	// размер сигнатуры в байтах
		&g_pInputLayoutTexture ) );		// Адрес указателя в который будет помещён созданный объект
	LogFile.OutputLogFileString( L"Create input g_pInputLayoutTexture: OK." );

	// Получаем указатель на сигнатуру шейдера
	V_RETURN( g_pTechTextureBone->GetPassByIndex( 0 )->GetDesc( &PassDesc ) );
	V_RETURN( g_pd3dDevice->CreateInputLayout(
		layout,							// Массив описания типов данных передаваемых на первую стадию графичесского конвеера
		numElements,					// Количество описаний типов данных
		PassDesc.pIAInputSignature,		// Указатель на сигнатуру входных данных шейдера
		PassDesc.IAInputSignatureSize,	// размер сигнатуры в байтах
		&g_pInputLayoutTextureBone ) );	// Адрес указателя в который будет помещён созданный объект
	LogFile.OutputLogFileString( L"Create input g_pInputLayoutTextureBone: OK." );

	g_DiffuseTexture = g_pEffect->GetVariableByName( "g_DiffuseTexture" )->AsShaderResource();
	if ( !g_DiffuseTexture->IsValid() )
	{
		LogFile.OutputLogFileErrorString( L"Error GetVariableByName( \"g_DiffuseTexture\" ).", __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}

	g_DepthTexture = g_pEffect->GetVariableByName( "g_DepthTexture" )->AsShaderResource();
	if ( !g_DepthTexture->IsValid() )
	{
		LogFile.OutputLogFileErrorString( L"Error GetVariableByName( \"g_DepthTexture\" ).", __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}

	g_SearchTexture = g_pEffect->GetVariableByName( "g_SearchTexture" )->AsShaderResource();
	if ( !g_SearchTexture->IsValid() )
	{
		LogFile.OutputLogFileErrorString( L"Error GetVariableByName( \"g_SearchTexture\" ).", __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}

	g_pWorldVariable = g_pEffect->GetVariableByName( "World" )->AsMatrix();
	if ( !g_pWorldVariable->IsValid() )
	{
		LogFile.OutputLogFileErrorString( L"Error GetVariableByName( \"World\" ).", __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}

	g_pViewVariable = g_pEffect->GetVariableByName( "View" )->AsMatrix();
	if ( !g_pViewVariable->IsValid() )
	{
		LogFile.OutputLogFileErrorString( L"Error GetVariableByName( \"View\" ).", __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}

	g_pProjectionVariable = g_pEffect->GetVariableByName( "Projection" )->AsMatrix();
	if ( !g_pProjectionVariable->IsValid() )
	{
		LogFile.OutputLogFileErrorString( L"Error GetVariableByName( \"Projection\" ).", __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}

	g_pViewProjVariable = g_pEffect->GetVariableByName( "ViewProj" )->AsMatrix();
	if ( !g_pProjectionVariable->IsValid() )
	{
		LogFile.OutputLogFileErrorString( L"Error GetVariableByName( \"ViewProj\" ).", __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}

	g_pPixelSize = g_pEffect->GetVariableByName( "PixelSize" )->AsVector();
	if ( !g_pPixelSize->IsValid() )
	{
		LogFile.OutputLogFileErrorString( L"Error GetVariableByName( \"PixelSize\" ).", __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}

	wsprintf( mESS, _T( "End create shader: %s." ), ShaderFileName );
	LogFile.OutputLogFileString( mESS );

	return S_OK;
}

HRESULT CSystem::InicialisePhysX( PxDefaultAllocator &gAlloc, PxErrorCallback &PhysXDefError )
{
	// PhysX
	bool recordMemoryAllocations = true;

	//PxTolerancesScale pxts = PxTolerancesScale();
	//pxts.length = 0.1f;
	mPhysX = PxCreatePhysics( PX_PHYSICS_VERSION, gAlloc, PhysXDefError, PxTolerancesScale(), recordMemoryAllocations );
	if( mPhysX == NULL )
	{
		LogFile.OutputLogFileErrorString( _T( "Error PxCreatePhysics 3.1." ), __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}

	//PxFoundation& foundation = mPhysX->getFoundation();
	//#ifdef PHYSX_PROFILE_SDK
	//	mProfileZone = &PxProfileZone::createProfileZone( &foundation, "SampleProfileZone", gProfileNameProvider );
	//	if ( m_renderer != NULL )
	//		m_renderer->setProfileZone( mProfileZone );
	//#endif

	//if ( mProfileZone )
	//	mPhysX->getProfileZoneManager().addProfileZone( mProfileZone );

	if( !PxInitExtensions( *mPhysX ) )
	{
		LogFile.OutputLogFileErrorString( _T( "Error PxInitExtensions." ), __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}

	mCooking = PxCreateCooking( PX_PHYSICS_VERSION, &mPhysX->getFoundation(), PxCookingParams() );
	if ( !mCooking )
	{
		LogFile.OutputLogFileErrorString( _T( "Error PxCreateCooking." ), __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}

	return S_OK;
}

HRESULT CSystem::CreateComputeShader( TCHAR* ShaderFileName, LPCSTR FunctionName, ID3D11ComputeShader* &CS )
{
	TCHAR mESS[256];
	wsprintf( mESS, _T( "Start create compute shader: %s." ), ShaderFileName );
	LogFile.OutputLogFileString( mESS );

	DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
	#if defined( DEBUG ) || defined( _DEBUG )
		dwShaderFlags |= D3D10_SHADER_DEBUG;
	#endif

	LPCSTR pProfile = ( FeatureLevel >= D3D_FEATURE_LEVEL_11_0 ) ? "cs_5_0" : "cs_4_0";

    ID3DBlob* pErrorBlob = NULL;
    ID3DBlob* pBlob = NULL;
    res = D3DX11CompileFromFile( ShaderFileName, NULL, NULL, FunctionName, pProfile, dwShaderFlags, NULL, NULL, &pBlob, &pErrorBlob, NULL );
	if ( res != S_OK )
	{
		if ( pErrorBlob )
		{
			char* ErrString = NULL;
			ErrString = (char*)pErrorBlob->GetBufferPointer();
			wsprintf( mESS, _T( "Error compile compute shader of file: %s." ), ShaderFileName );
			LogFile.OutputLogFileErrorString( mESS, __FILE__, __FUNCTION__, __LINE__ );
		}
		else
			LogFile.OutputLogFileErrorString( _T( "Error D3DX11CompileFromFile(CS)." ), __FILE__, __FUNCTION__, __LINE__ );

		return S_FALSE;
	}

    res = g_pd3dDevice->CreateComputeShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &CS );
	if ( res != S_OK )
	{
		wsprintf( mESS, _T( "Error create compute shader of file: %s." ), ShaderFileName );
		LogFile.OutputLogFileErrorString( mESS, __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}

    RELEASE( pErrorBlob );
    RELEASE( pBlob );

	wsprintf( mESS, _T( "End create compute shader: %s." ), ShaderFileName );
	LogFile.OutputLogFileString( mESS );

	return S_OK;
}

HRESULT CSystem::Scan3DSystem( void )
{
	LogFile.OutputLogFileString( L"Scan 3D graph system: START." );

	// Find adapters(videocard)
	UINT Index = 0;
	IDXGIAdapter1* pIAdapter = NULL;
	CAdapterData* Adapter;
	while( S_OK == g_pFactory->EnumAdapters1( Index, &pIAdapter ) )
	{
		Adapter = new CAdapterData( pIAdapter, Index );

		if ( S_OK != Adapter->FindMonitor() )
			delete Adapter;
		else
			aAdapter.Add( Adapter );

		Index++;
	}

	if ( aAdapter.GetCount() == 0 )
	{
		LogFile.OutputLogFileErrorString( L"No find compartible application adapters.", __FILE__, __FUNCTION__, __LINE__  );
		return S_FALSE;
	}

	LogFile.OutputLogFileString( L"Scan 3D graph system: End." );

	return S_OK;
}

// Функция вызова функции обработки сообщений для текущего экземпляра класса
LRESULT CSystem::WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	CSystem* pWindow = NULL;
        
	if ( uMsg==WM_NCCREATE )
	{   
		assert( !IsBadReadPtr((void *) lParam, sizeof(CREATESTRUCT)) );
		MDICREATESTRUCT * pMDIC = (MDICREATESTRUCT *) ((LPCREATESTRUCT) lParam)->lpCreateParams;

		pWindow = (CSystem*)(pMDIC->lParam);

		assert( !IsBadReadPtr( pWindow, sizeof(CSystem) ) );
		SetWindowLong(hWnd, GWL_USERDATA, (LONG) pWindow);
	}
	else
		pWindow=(CSystem*)GetWindowLong( hWnd, GWL_USERDATA );

	if ( pWindow )
		return pWindow->WndProc( hWnd, uMsg, wParam, lParam );
	else
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

//  обработки сообщений
LRESULT CSystem::WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
		case WM_ACTIVATE:
		{
			if ( LOWORD(wParam) == WA_ACTIVE )
			{
				if ( HIWORD ( wParam ) == 0 )
				{
					fPause = false;
					RedKid.Acquire();
					RedKid.g_cDInput.FixedInputStatus();
					return 0;
				}
				else
				{
					fPause = true;
					return 0;
				}
			}
		};
		case WM_KILLFOCUS:
			{
				fPause = true;
				return 0;
			}
		case WM_SETFOCUS:
			{
				fPause = false;
				RedKid.Acquire();
				RedKid.g_cDInput.FixedInputStatus();
				return 0;
			};
		case WM_SIZE:
		{
			if ( wParam == SIZE_MINIMIZED )
				return 0;

			if ( fInit )
				if ( S_OK != Resize() )
				{
					LogFile.OutputLogFileErrorString( L"Error resize.", __FILE__, __FUNCTION__, __LINE__ );
					DestroyWindow( hWnd );
				}

			return 0;
		};
		case WM_CLOSE:
		{
			DestroyWindow( hWnd );
			return 0;
		}
		case WM_DESTROY:
		{
			if ( AppIni.GetWindowed() == true )
			{
				RECT rc;
				GetWindowRect( hWnd, &rc );
				AppIni.SetWidth( rc.right - rc.left );
				AppIni.SetHeight( rc.bottom - rc.top );
			}

			g_pImmediateContext->ClearState();
			g_pSwapChain->SetFullscreenState( false, NULL );
			PostQuitMessage(0);
			return 0;
		}
	}

   return DefWindowProc(hWnd, msg, wParam, lParam);
}

int CSystem::Run( void )
{
	if ( !fInit )
	{
		LogFile.OutputLogFileErrorString( _T( "No inicialise application." ), __FILE__, __FUNCTION__, __LINE__  );
		return -1;
	}

	V_RETURN( g_pFactory->MakeWindowAssociation( hWnd, DXGI_MWA_NO_ALT_ENTER ) );

	RedKid.Acquire();
	bool fW = AppIni.GetWindowed();
	LogFile.OutputLogFileString( _T( "Show window." ) );
	::ShowWindow( hWnd, true );
	::UpdateWindow( hWnd );

	AppIni.SetWindowed( fW );
	if ( AppIni.GetWindowed() == false )
		g_pSwapChain->SetFullscreenState( true, NULL );

	// Цикл обработки сообщений
	MSG msg	= { 0 };
	LogFile.OutputLogFileString( _T( "Message loop: START." ) );
	while ( msg.message != WM_QUIT )
	{
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
			if ( S_OK != FrameRender() )
				DestroyWindow( hWnd );
	}

	LogFile.OutputLogFileString( _T( "Message loop: EXIT.") );

	V_( S_OK != AppIni.Save() );

	LogFile.OutputLogFileString( _T( "Exit application: OK." ) );

	return 0;
}

HRESULT CSystem::Resize( void )
{
	LogFile.OutputLogFileString( L"Resize: START." );

	BOOL InFullScreen;
	UINT width = 0;
	UINT height = 0;
	std::wstringstream ss;

	// Определим какой режим сейчас установлен
	g_pSwapChain->GetFullscreenState( &InFullScreen, NULL );
	if( !InFullScreen )
	{
		RECT rc;
		GetClientRect( SystemRedKid.GetHWND(), &rc );
		width = rc.right - rc.left;
		height = rc.bottom - rc.top;
		ss << L"+++ Windowed: " << width << L"x" << height << L".";
		LogFile.OutputLogFileString( ss.str() );
	}
	else
	{
		width = GetSystemMetrics( SM_CXSCREEN );
		height = GetSystemMetrics( SM_CYSCREEN );
		ss << L"+++FullScreen: " << width << L"x" << height << L".";
		LogFile.OutputLogFileString( ss.str() );
	};

	if ( width == 0 || height == 0 )
	{
		std::wstringstream ss;
		ss << L"ERROR: width " << width << ", height " << height << L".";
		LogFile.OutputLogFileString( ss.str() );
		return S_FALSE;
	}

	// Освобождаем ссылку на представление данных буффера визуализации
	RELEASE( g_pRenderScreenTargetView );

	V_RETURN( g_pSwapChain->ResizeBuffers(
			2,							// Общее количество буферов в цепочке переключений
			0,							// Новая ширина
			0,							// Новая высота
			FORMAT_RENDER_BUFFER,		// Новый формат вторичного буфера
			0 ) );						// Дополнительные флаги

	// Связывание устройства со вторичным буффером в цепочке переключений
	ID3D11Texture2D*	g_pBackBuffer;
	V_RETURN( g_pSwapChain->GetBuffer( 0,			// Индекс вторичного буффера к которому нужно получить доступ
					__uuidof( ID3D11Texture2D ),	// Идентификатор интерфейса который используется для выполнения дейстрвий
					( LPVOID*)&g_pBackBuffer ) );	// Адрес указателя наинтерфейс вторичного буффера

	// Создаём представление данных как вторичного буффера визуализации
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory( &descDepth, sizeof( descDepth ) );
	g_pBackBuffer->GetDesc( &descDepth );

	D3D11_RENDER_TARGET_VIEW_DESC rt;
	ZeroMemory( &rt, sizeof( D3D11_RENDER_TARGET_VIEW_DESC ) );
	rt.Format = descDepth.Format;
	rt.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rt.Texture2D.MipSlice = 0;
	res = g_pd3dDevice->CreateRenderTargetView(
			g_pBackBuffer,					// Указатель на ресурс содержащий данные
			&rt,							// Указатель на структуру содержащую параметры визуализации
			&g_pRenderScreenTargetView );	// Адрес указателя на созданное представление данных
	RELEASE( g_pBackBuffer );
	if ( res != S_OK )
	{
		LogFile.OutputLogFileErrorString( L"Error create RenderTargetView.", __FILE__, __FUNCTION__, __LINE__ );
		return res;
	}

	// Создаём текстуру буфера глубины
	ID3D11Texture2D* g_pDepthStencil = NULL;
	ZeroMemory( &descDepth, sizeof( descDepth ) );
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	//descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.Format = FORMAT_DEPTH_BUFFER;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	V_RETURN( g_pd3dDevice->CreateTexture2D( &descDepth, NULL, &g_pDepthStencil ) );

	// Создаём SRV на буффер глубины
	RELEASE( g_pDepthStencilSRV );
	D3D11_SHADER_RESOURCE_VIEW_DESC DescSRV;
	ZeroMemory( &DescSRV, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC) );
	DescSRV.Format = DXGI_FORMAT_R32_FLOAT;
	DescSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	DescSRV.Texture2D.MipLevels = 1;
	DescSRV.Texture2D.MostDetailedMip = 0;
	V_RETURN( g_pd3dDevice->CreateShaderResourceView( g_pDepthStencil, &DescSRV, &g_pDepthStencilSRV ) );

	// Теперь создаём представление данных как шаблонного буфера глубины
	RELEASE( g_pDepthStencilView );
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory( &descDSV, sizeof( descDSV ) );
    descDSV.Format = DXGI_FORMAT_D32_FLOAT;
    descDSV.Flags = 0;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	res = g_pd3dDevice->CreateDepthStencilView( g_pDepthStencil, &descDSV, &g_pDepthStencilView );
	RELEASE( g_pDepthStencil );
	if ( res != S_OK )
	{
		LogFile.OutputLogFileErrorString( L"Error create DepthStencilView.", __FILE__, __FUNCTION__, __LINE__ );
		return res;
	}
	LogFile.OutputLogFileString( L"Create DepthStencilView: OK." );

	LogFile.OutputLogFileString( L"Resize different render texture: START." );
	V_RETURN( cTextDiff.Resize( width, height, FORMAT_HDR_RENDER_BUFFER ) );
	V_RETURN( cTextGBuffer.Resize( width, height, FORMAT_HDR_RENDER_BUFFER ) );
	V_RETURN( cTextMaterial.Resize( width, height, DXGI_FORMAT_R32_FLOAT ) );
	V_RETURN( cTextLightRender.Resize( width, height, FORMAT_HDR_RENDER_BUFFER ) );
	V_RETURN( cTextEdges.Resize( width, height, FORMAT_RENDER_BUFFER ) );
	V_RETURN( cTextWeight.Resize( width, height, FORMAT_RENDER_BUFFER ) );
	V_RETURN( cTextBloom.Resize( width / 8, height / 8, FORMAT_HDR_RENDER_BUFFER ) );
	V_RETURN( cTextBloomGaus.Resize( width / 8, height / 8, FORMAT_RENDER_BUFFER ) );

	static UINT SC;
	XScBl = UINT( ceil( (float)width / 32.0f ) );
	YScBl = UINT( ceil( (float)height / 32.0f ) );
	SC = XScBl * YScBl;
	V_RETURN( g_pHDRCalc.Resize( sizeof(float), SC ) );
	V_RETURN( g_pHDRCalcLum.Resize( sizeof(float), UINT( ceil( (float)SC / 1024.0f ) ) ) )
	LogFile.OutputLogFileString( L"Resize different render texture: OK." );

	//Устанавливаем новые ViewPort для просчитываемой текстуры
	mViewPort.Width = (float)width;
    mViewPort.Height = (float)height;
    mViewPort.MinDepth = 0.0f;
    mViewPort.MaxDepth = 1.0f;
    mViewPort.TopLeftX = 0;
    mViewPort.TopLeftY = 0;
    g_pImmediateContext->RSSetViewports( 1, &mViewPort );

	AppIni.SetWidth( width );
	AppIni.SetHeight( height );
	AppIni.SetWindowed( !InFullScreen );

	RedKid.Resize( width, height );

	LogFile.OutputLogFileString( L"Resize: END." );

	return S_OK;
}

HRESULT CSystem::FrameRender( void )
{
	static DWORD TimerStart = 0;
	static DWORD TimerCur;
	static DWORD TimerFPSCur;
	static float DeltaTime;
	static DWORD TimerStopCur;
	static DWORD DeltaStopTime;
	static float dTFPS;
	static UINT	 mNumFrame = 0;

	TimerCur = GetTickCount();

	if ( TimerStart == 0 )
	{
		TimerStart = TimerCur;
		TimerFPSCur = TimerCur;
	}

	if ( !fPause )
	{
		TimerStopCur = TimerCur;

		mNumFrame++;
		dTFPS = (float)TimerCur - (float)TimerFPSCur;
		if ( mNumFrame == 100 )
		{
			mFps = 1000.0f / ( ( dTFPS ) / 100.0f );
			mNumFrame = 0;
			TimerFPSCur = TimerCur;
		}
		else
			if ( ( dTFPS ) > 1000.0f )
			{
				mFps = (float)mNumFrame;
				mNumFrame = 0;
				TimerFPSCur = TimerCur;
			}

		DeltaTime = ( (float)TimerCur - (float)TimerStart ) / 1000.0f;

		static float ElapsedTime = (float)TimerCur;
		ElapsedTime = ( (float)TimerCur - ElapsedTime ) / 1000.0f;
		V_RETURN( RedKid.FrameRender( DeltaTime, ElapsedTime ) );
		ElapsedTime = (float)TimerCur;
		return S_OK;
	}
	else
	{
		DeltaStopTime = TimerCur - TimerStopCur;
		TimerStopCur = TimerCur;

		TimerStart += DeltaStopTime;
	}

	return S_OK;
}

void CSystem::RenderFinalSceneQuad( void )
{
	static ID3D11RenderTargetView*		rTV[3];
	static ID3D11ShaderResourceView*	aRViewsSRV[4];
	static ID3D11ShaderResourceView*	ppSRVNULL[4] = { NULL, NULL, NULL, NULL };

	////////////////////////////////////////////////////////////////////////////////
	// Render QuadScreen(SMAA)
	rTV[0] = cTextEdges.GetTextureRT();
	rTV[1] = NULL;
	rTV[2] = NULL;

	g_pDefferedContext[0]->OMSetRenderTargets( 3, rTV, NULL );
	g_pDefferedContext[0]->RSSetViewports( 1, &mViewPort );

	static float PixelSize[2];
	PixelSize[0] = 1.0f / AppIni.GetWidth();
	PixelSize[1] = 1.0f / AppIni.GetHeight();

	g_pPixelSize->SetRawValue( PixelSize, 0, sizeof(float) * 2 );
	g_DiffuseTexture->SetResource( cTextLightRender.GetTextureSRV() );
	g_DepthTexture->SetResource( g_pDepthStencilSRV );

	g_pDefferedContext[0]->IASetInputLayout( g_pInputLayoutQuad );

	static UINT offset = 0;
	static UINT StructSize = sizeof( VERTEX_QUAD );
	g_pDefferedContext[0]->IASetVertexBuffers( 0, 1, &g_pVBQuad, &StructSize, &offset );

	g_pDefferedContext[0]->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	g_pSMAATechLumaEdgeDet->GetPassByIndex( 0 )->Apply( 0, g_pDefferedContext[0] );

	g_pDefferedContext[0]->Draw( 4, 0 );

	g_pDefferedContext[0]->PSSetShaderResources( 0, 3, ppSRVNULL );

	////////////////////////////////////////////////////////////////////////////////
	rTV[0] = cTextWeight.GetTextureRT();
	g_pDefferedContext[0]->OMSetRenderTargets( 1, rTV, NULL );

	g_pSMAATechBlendWeightCalc->GetPassByIndex( 0 )->Apply( 0, g_pDefferedContext[0] );

	aRViewsSRV[0] = cTextEdges.GetTextureSRV();
	aRViewsSRV[1] = g_pAreaTextureSRV;
	aRViewsSRV[2] = g_pSearchTextureSRV;
	g_pDefferedContext[0]->PSSetShaderResources( 0, 3, aRViewsSRV );

	g_pDefferedContext[0]->Draw( 4, 0 );

	////////////////////////////////////////////////////////////////////////////////
	rTV[0] = g_pRenderScreenTargetView;
	g_pDefferedContext[0]->OMSetRenderTargets( 1, rTV, NULL );

	g_pSMAANeighborhoodBlending->GetPassByIndex( 0 )->Apply( 0, g_pDefferedContext[0] );

	aRViewsSRV[0] = cTextLightRender.GetTextureSRV();
	aRViewsSRV[1] = cTextWeight.GetTextureSRV();
	aRViewsSRV[2] = g_pHDRCalcLum.GetBufferSRV();
	aRViewsSRV[3] = cTextBloomGaus.GetTextureSRV();
	g_pDefferedContext[0]->PSSetShaderResources( 0, 4, aRViewsSRV );

	g_pDefferedContext[0]->Draw( 4, 0 );

	static ID3D11CommandList* g_pCommandList;
	g_pCommandList = NULL;
	V_( g_pDefferedContext[0]->FinishCommandList( false, &g_pCommandList ) );

	g_pImmediateContext->ExecuteCommandList( g_pCommandList, true );
	RELEASE( g_pCommandList );

	g_pDefferedContext[0]->PSSetShaderResources( 0, 4, ppSRVNULL );
	g_pImmediateContext->OMSetRenderTargets( 1, rTV, g_pDepthStencilView );
}

void CSystem::DefferedRenderLight( float ElapsedTime, D3DXMATRIX* mWVInv, CArrayBufferStruct<LIGHT_POINT> *aLightPoint, CArrayBufferStruct<LIGHT_DIRECTION> *aLightDirection )
{
	static ID3D11ShaderResourceView*	aRViewsSRV[3];

	static ID3D11ShaderResourceView*	ppSRVNULL[3] = { NULL, NULL, NULL };
	static ID3D11UnorderedAccessView*	ppUAVNULL[3] = { NULL, NULL, NULL };
	static ID3D11Buffer*				ppBufferNULL[3] = { NULL, NULL, NULL };

	static ID3D11RenderTargetView*		ppRTCopy[1];
	static ID3D11UnorderedAccessView*	aRViewsUAV[3];

	static ID3D11RenderTargetView*		ppRTNULL[3] = { NULL, NULL, NULL };
	g_pImmediateContext->OMSetRenderTargets( 3, ppRTNULL, NULL );

	// Render Light
	aRViewsSRV[0] = cTextDiff.GetTextureSRV();
	aRViewsSRV[1] = cTextGBuffer.GetTextureSRV();
	aRViewsSRV[2] = cTextMaterial.GetTextureSRV();
	g_pImmediateContext->CSSetShaderResources( 0, 3, aRViewsSRV );

	aRViewsUAV[0] = cTextLightRender.GetTextureUAV();
	aRViewsUAV[1] = g_pHDRCalc.GetBufferUAV();
	aRViewsUAV[2] = cTextBloom.GetTextureUAV();
	g_pImmediateContext->CSSetUnorderedAccessViews( 0, 3, aRViewsUAV, NULL );

	static sDefferedRenderData sDeffDat;
	sDeffDat.mVPInv = *mWVInv;
	sDeffDat.Width = (float)AppIni.GetWidth();
	sDeffDat.Height = (float)AppIni.GetHeight();
	sDeffDat.LightPointCount = aLightPoint->GetCount();
	sDeffDat.LightDirectionCount = aLightDirection->GetCount();
	sDeffDat.XBlockCount = (UINT)ceil( (float)AppIni.GetWidth() / 32.0f );
	c_CBRenderLight.of_CPU_in_GPU_Copy( &sDeffDat );
	c_CBRenderLight.CSSetConstantBuffers( 0 );

	V_( aLightPoint->CopyDataOfCPUInGPU() );
	aLightPoint->CSSetConstantBuffers( 1 );

	V_( aLightDirection->CopyDataOfCPUInGPU() );
	aLightDirection->CSSetConstantBuffers( 2 );

	g_pImmediateContext->CSSetShader( g_pCSRenderLight, NULL, 0 );
	g_pImmediateContext->Dispatch( XScBl, YScBl, 1 );

	// Calc Summ Lum
	aRViewsUAV[0] = g_pHDRCalcLum.GetBufferUAV();
	aRViewsUAV[1] = NULL;
	aRViewsUAV[2] = NULL;
	g_pImmediateContext->CSSetUnorderedAccessViews( 0, 3, aRViewsUAV, NULL );

	aRViewsSRV[0] = g_pHDRCalc.GetBufferSRV();
	aRViewsSRV[1] = NULL;
	aRViewsSRV[2] = NULL;
	g_pImmediateContext->CSSetShaderResources( 0, 3, aRViewsSRV );

	static sHDRCalcData SummConst;
	static UINT SC;
	SC = XScBl * YScBl;
	SummConst.SummSize = SC;
	SummConst.ElapsedTime = ElapsedTime;
	SummConst.NumPoint = float( AppIni.GetWidth() * AppIni.GetHeight() );
	V_( g_pHDRData.of_CPU_in_GPU_Copy( &SummConst ) );
	g_pHDRData.CSSetConstantBuffers( 0 );

	UINT SCMod;
	SCMod = UINT( ceil( (float)SC / 1024.0f ) );
	g_pImmediateContext->CSSetShader( g_pCSCalcSummLum, NULL, 0 );
	g_pImmediateContext->Dispatch( SCMod, 1, 1 );

	// Calc adapted lum
	aRViewsUAV[1] = g_pHDRPrevLum.GetBufferUAV();
	g_pImmediateContext->CSSetUnorderedAccessViews( 1, 1, &aRViewsUAV[1], NULL );

	g_pImmediateContext->CSSetShader( g_pCSCalcAdaptedLum, NULL, 0 );
	g_pImmediateContext->Dispatch( SCMod, 1, 1 );

	// Bloor Bloom Gaus x3 pass
	aRViewsSRV[0] = cTextBloom.GetTextureSRV();
	g_pImmediateContext->CSSetShaderResources( 0, 1, aRViewsSRV );

	aRViewsUAV[0] = cTextBloomGaus.GetTextureUAV();
	aRViewsUAV[1] = NULL;
	g_pImmediateContext->CSSetUnorderedAccessViews( 0, 2, aRViewsUAV, NULL );

	g_pImmediateContext->CSSetShader( g_pCSBloomGaus, NULL, 0 );
	g_pImmediateContext->Dispatch( (UINT)( (float)AppIni.GetWidth() / 8.0f ), (UINT)( (float)AppIni.GetHeight() / 8.0f ), 1 );

	// Pass #1
	aRViewsUAV[0] = NULL;
	g_pImmediateContext->CSSetUnorderedAccessViews( 0, 1, aRViewsUAV, NULL );

	aRViewsSRV[0] = cTextBloomGaus.GetTextureSRV();
	g_pImmediateContext->CSSetShaderResources( 0, 1, aRViewsSRV );

	aRViewsUAV[0] = cTextBloom.GetTextureUAV();
	g_pImmediateContext->CSSetUnorderedAccessViews( 0, 1, aRViewsUAV, NULL );

	g_pImmediateContext->Dispatch( (UINT)( (float)AppIni.GetWidth() / 8.0f ), (UINT)( (float)AppIni.GetHeight() / 8.0f ), 1 );

	// Pass #2
	aRViewsUAV[0] = NULL;
	g_pImmediateContext->CSSetUnorderedAccessViews( 0, 1, aRViewsUAV, NULL );

	aRViewsSRV[0] = cTextBloom.GetTextureSRV();
	g_pImmediateContext->CSSetShaderResources( 0, 1, aRViewsSRV );

	aRViewsUAV[0] = cTextBloomGaus.GetTextureUAV();
	g_pImmediateContext->CSSetUnorderedAccessViews( 0, 1, aRViewsUAV, NULL );

	g_pImmediateContext->Dispatch( (UINT)( (float)AppIni.GetWidth() / 8.0f ), (UINT)( (float)AppIni.GetHeight() / 8.0f ), 1 );

	// End
	g_pImmediateContext->CSSetShaderResources( 0, 3, ppSRVNULL );
	g_pImmediateContext->CSSetUnorderedAccessViews( 0, 2, ppUAVNULL, NULL );
	g_pImmediateContext->CSSetConstantBuffers( 0, 3, ppBufferNULL );
	ppRTCopy[0] = cTextLightRender.GetTextureRT();
	g_pImmediateContext->OMSetRenderTargets( 1, ppRTCopy, g_pDepthStencilView );
};

HRESULT CSystem::CreateAreaTexture( void )
{
	LogFile.OutputLogFileString( _TEXT( "Create AreaTexture: START." ) );

    D3D11_SUBRESOURCE_DATA data;
    data.pSysMem = areaTexBytes;
    data.SysMemPitch = AREATEX_PITCH;
    data.SysMemSlicePitch = 0;

    D3D11_TEXTURE2D_DESC descTex;
    descTex.Width = AREATEX_WIDTH;
    descTex.Height = AREATEX_HEIGHT;
    descTex.MipLevels = descTex.ArraySize = 1;
    descTex.Format = DXGI_FORMAT_R8G8_UNORM;
    descTex.SampleDesc.Count = 1;
    descTex.SampleDesc.Quality = 0;
    descTex.Usage = D3D11_USAGE_DEFAULT;
    descTex.BindFlags = D3D10_BIND_SHADER_RESOURCE;
    descTex.CPUAccessFlags = 0;
    descTex.MiscFlags = 0;
    V_RETURN( g_pd3dDevice->CreateTexture2D( &descTex, &data, &g_pAreaTexture ) );

    D3D11_SHADER_RESOURCE_VIEW_DESC descSRV;
    descSRV.Format = descTex.Format;
    descSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    descSRV.Texture2D.MostDetailedMip = 0;
    descSRV.Texture2D.MipLevels = 1;
    V_RETURN( g_pd3dDevice->CreateShaderResourceView( g_pAreaTexture, &descSRV, &g_pAreaTextureSRV ) );

	return S_OK;

	LogFile.OutputLogFileString( _TEXT( "Create AreaTexture: END." ) );
}

HRESULT CSystem::CreateSearchTexture( void )
{
	LogFile.OutputLogFileString( _TEXT( "Create SearchTexture: START." ) );

    D3D11_SUBRESOURCE_DATA data;
    data.pSysMem = searchTexBytes;
    data.SysMemPitch = SEARCHTEX_PITCH;
    data.SysMemSlicePitch = 0;

    D3D11_TEXTURE2D_DESC descTex;
    descTex.Width = SEARCHTEX_WIDTH;
    descTex.Height = SEARCHTEX_HEIGHT;
    descTex.MipLevels = descTex.ArraySize = 1;
    descTex.Format = DXGI_FORMAT_R8_UNORM;
    descTex.SampleDesc.Count = 1;
    descTex.SampleDesc.Quality = 0;
    descTex.Usage = D3D11_USAGE_DEFAULT;
    descTex.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    descTex.CPUAccessFlags = 0;
    descTex.MiscFlags = 0;
    V_RETURN( g_pd3dDevice->CreateTexture2D( &descTex, &data, &g_pSearchTexture ) );

    D3D11_SHADER_RESOURCE_VIEW_DESC descSRV;
    descSRV.Format = descTex.Format;
    descSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    descSRV.Texture2D.MostDetailedMip = 0;
    descSRV.Texture2D.MipLevels = 1;
    V_RETURN( g_pd3dDevice->CreateShaderResourceView( g_pSearchTexture, &descSRV, &g_pSearchTextureSRV ) );

	return S_OK;

	LogFile.OutputLogFileString( _TEXT( "Create SearchTexture: END." ) );
}