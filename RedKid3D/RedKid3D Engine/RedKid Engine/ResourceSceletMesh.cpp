#include "ResourceSceletMesh.h"

CResourceSceletMesh::CResourceSceletMesh(void)
	: CResMesh(), aSceletAnimation( 10 )
{
	Type = RES_SCELET_MESH;

	g_pRenderVertexBuffer = &c_UAVOut.GetBuffer();

	g_pCSCopy = NULL;
	g_pCSAnim = NULL;
	ActivAnimation = NULL;
	mDTimeScAnim = 0.0f;
}

CResourceSceletMesh::~CResourceSceletMesh(void)
{
	Release();
}

void CResourceSceletMesh::Release( void )
{
	CResMesh::Release();
	c_UAVOut.Release();
	c_SRVIn.Release();
	c_CBMatr.Release();
	Scelet.Release();

	g_pCSCopy = NULL;
	g_pCSAnim = NULL;

	ActivAnimation = NULL;
	mDTimeScAnim = 0.0f;
}

HRESULT CResourceSceletMesh::CreateMeshOfTXTFile( TCHAR* FileName, CMeshFormat* mForm )
{
	std::ifstream file;

	if ( !mForm->GetfSceletData() )
	{
		TCHAR __eRR[1024];
		wsprintf( __eRR, L"This material does not support skeletal animation.\nNo open mesh txt file: %s.", FileName );
		LogFile.OutputLogFileErrorString( __eRR, __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}

	try
	{
		file.open( FileName, std::ios::in );
		if ( !file )
		{
			TCHAR __eRR[1024];
			wsprintf( __eRR, L"No open mesh txt file: %s.", FileName );
			LogFile.OutputLogFileErrorString( __eRR, __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}
	}
	catch ( ... )
	{
		TCHAR __eRR[1024];
		wsprintf( __eRR, L"Error I/O(open txt file: %s).", FileName );
		LogFile.OutputLogFileErrorString( __eRR, __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}

	try
	{
		std::string Line;

		file >> Line;
		if ( Line != "[RedMax3D.com._RedKid_PlugIn_V4.0.0.]" )
		{
			TCHAR __eRR[1024];
			wsprintf( __eRR, L"Unknow format file(open txt file: %s).", FileName );
			LogFile.OutputLogFileErrorString( __eRR, __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}

		file >> Line;
		if ( Line != "[Eport_mesh,_sceleton_and_sceleton_animation_of_3Ds_Max,_in_text_format_for_DirectX_dimension.]" )
		{
			TCHAR __eRR[1024];
			wsprintf( __eRR, L"Unknow format file(open txt file: %s).", FileName );
			LogFile.OutputLogFileErrorString( __eRR, __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}

		Release();

		// Читаем блок с костями
		file >> Line;
		if ( Line != "[Start_Scelet_Block]" )
		{
			TCHAR __eRR[1024];
			wsprintf( __eRR, L"Unknow format file(open txt file: %s).", FileName );
			LogFile.OutputLogFileErrorString( __eRR, __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}

		LogFile.OutputLogFileString( _T( "Start read scelet." ) );

		if ( S_OK != Scelet.ReadFromStream( file ) )
		{
			Release();
			return S_FALSE;
		}

		file >> Line;
		if ( Line != "[End_Scelet_Block]" )
		{
			Scelet.Release();
			TCHAR __eRR[1024];
			wsprintf( __eRR, L"Unknow format file(open txt file: %s).", FileName );
			LogFile.OutputLogFileErrorString( __eRR, __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}

		LogFile.OutputLogFileString( _T( "End read scelet." ) );

		file >> Line;
		if ( Line != "[Start_Mesh_block]" )
		{
			TCHAR __eRR[1024];
			wsprintf( __eRR, L"Unknow format file(open txt file: %s).", FileName );
			LogFile.OutputLogFileErrorString( __eRR, __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}


		if ( S_OK != LoadMeshBlock( file, mForm, FileName ) )
		{
			Release();
			return S_FALSE;
		}

		file >> Line;
		if ( Line != "[End_Mesh_block]" )
		{
			Release();
			TCHAR __eRR[1024];
			wsprintf( __eRR, L"Unknow format file(open txt file: %s).", FileName );
			LogFile.OutputLogFileErrorString( __eRR, __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}

		file.close();
	}
	catch ( ... )
	{
		Release();
		TCHAR __eRR[1024];
		wsprintf( __eRR, L"Error I/O(load txt file: %s).", FileName );
		LogFile.OutputLogFileErrorString( __eRR, __FILE__, __FUNCTION__, __LINE__ );
		file.close();
		return S_FALSE;
	}

	Name = FileName;

	TCHAR mESS[1024];
	wsprintf( mESS, L"OK read mesh txt file: %s.", FileName );
	LogFile.OutputLogFileString( mESS );	

	return S_OK;
}

bool CResourceSceletMesh::UpdateAnimation( float DeltaTime, D3DXMATRIX* m_GlobalWordMat )
{
	static D3DXVECTOR3 min, max;

	static ID3D11ShaderResourceView*	aRViewsSRV[1];
	static ID3D11UnorderedAccessView*	aRViewsUAV[2];

	static ID3D11ShaderResourceView*	ppSRVNULL[1] = { NULL };
	static ID3D11UnorderedAccessView*	ppUAVNULL[2] = { NULL, NULL };

	// Устаравливаем константный буффер
	V_( c_CBMatr.of_CPU_in_GPU_Copy( m_GlobalWordMat ) );
	c_CBMatr.CSSetConstantBuffers( 0 );

	// Устанавливаем входной буффер
	aRViewsSRV[0] = c_SRVIn.GetSRV();
	SystemRedKid.GetDeviceContext()->CSSetShaderResources( 0, 1, aRViewsSRV );

	// Устанавливаем выходной буффер
	aRViewsUAV[0] = c_UAVOut.GetBufferUAV();
	aRViewsUAV[1] = SystemRedKid.GetUAVBufferCalcAABB()->GetBufferUAV();
	SystemRedKid.GetDeviceContext()->CSSetUnorderedAccessViews( 0, 2, aRViewsUAV, NULL );

	SystemRedKid.GetDeviceContext()->CSSetShader( SystemRedKid.GetCSCopyCalcAABB_Scelet(), NULL, 0 );
	SystemRedKid.GetDeviceContext()->Dispatch( 1, 1, 1 );

	if ( ActivAnimation != NULL )
	{
		// Обновляем анимацию в зависимости от текущего времени
		UpdateScelet( DeltaTime );
		// Копируем структуры анимации костей на GPU
		Scelet.SetupCBInGPU( 1 );

		SystemRedKid.GetDeviceContext()->CSSetShader( g_pCSAnim, NULL, 0 );
	}
	else
		SystemRedKid.GetDeviceContext()->CSSetShader( g_pCSCopy, NULL, 0 );

	// Выполняем шейдер
	//for ( UINT i = 0; i < 1000; i++ )
	SystemRedKid.GetDeviceContext()->Dispatch( (UINT)ceil( (float)mVertexCount / 32.0f ), 1, 1 );

	///////////////////////////////////////////////////////////////////////////////////////
	// AABB
	SystemRedKid.GetBufferReadAABB()->CopyResource( SystemRedKid.GetUAVBufferCalcAABB() );

	static int* curGPU;
	static int i;
	static D3D11_MAPPED_SUBRESOURCE MappedResource; 
	V_( g_pImmediateContext->Map( SystemRedKid.GetBufferReadAABB()->GetBuffer(), 0, D3D11_MAP_READ, 0, &MappedResource ) );
	curGPU = (int*)MappedResource.pData;

	min.x = (float)curGPU[0] / 100000.0f;
	min.y = (float)curGPU[1] / 100000.0f;
	min.z = (float)curGPU[2] / 100000.0f;

	max.x = (float)curGPU[3] / 100000.0f;
	max.y = (float)curGPU[4] / 100000.0f;
	max.z = (float)curGPU[5] / 100000.0f;

	//min = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	//max = D3DXVECTOR3( 10.0f, 10.0f, 10.0f );

	cAABBScelet.Set( min, max );

	g_pImmediateContext->Unmap( SystemRedKid.GetBufferReadAABB()->GetBuffer(), 0 );
	// AABB
	///////////////////////////////////////////////////////////////////////////////////////

	// Обнуляем буффера
	SystemRedKid.GetDeviceContext()->CSSetShaderResources( 0, 1, ppSRVNULL );
	SystemRedKid.GetDeviceContext()->CSSetUnorderedAccessViews( 0, 2, ppUAVNULL, NULL );
	//SystemRedKid.GetDeviceContext()->CSSetShader( NULL, NULL, 0 );

	return true;
}

HRESULT CResourceSceletMesh::SetMaterial( CResMat* mat )
{
	#if defined( DEBUG ) || defined( _DEBUG )
		if ( mat == NULL )
		{
			LogFile.OutputLogFileErrorString( _T( "mat == NULL." ), __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}
	#endif

	if ( cMeshFormat != *mat->GetMeshFormat() )
	{
		LogFile.OutputLogFileErrorString( _T( "Different formats mesh and format material." ), __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}

	// Настраиваем переменные в зависимости от формата сетки
	if ( mat->GetMeshFormat()->GetfTangBinor() )
	{
		g_pCSCopy = SystemRedKid.GetCSSceletTBNCopy();
		g_pCSAnim = SystemRedKid.GetCSSceletTBNAnim();
		StructSize = sizeof( VERTEX_TBN_UV );
	}
	else
	{
		g_pCSCopy = SystemRedKid.GetCSSceletNormalCopy();
		g_pCSAnim = SystemRedKid.GetCSSceletNormalAnim();
		StructSize = sizeof( VERTEX_NORMAL_UV );
	}

	// Создание буфферов для скелетки
	V_RETURN( c_CBMatr.Create( sizeof( D3DXMATRIX ) ) );
	V_RETURN( c_SRVIn.Create( g_pVertexBuffer ) );
	V_RETURN( c_UAVOut.Create( StructSize, mVertexCount ) );

	cMaterial = mat;
	cMaterial->IncrementLink();

	return S_OK;
}

HRESULT CResourceSceletMesh::AddSceletAnimation( CResourceAnimationScelet* Anim, UINT &IndexAdd, bool fSetAnim  )
{
	#if defined( DEBUG ) || defined( _DEBUG )
		if ( Anim == NULL )
		{
			LogFile.OutputLogFileErrorString( _T( "Anim == NULL." ), __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}
	#endif

	if ( GetBoneCount() != Anim->GetBoneCount() )
	{
		LogFile.OutputLogFileErrorString( _T( "Different quantity of bones." ), __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}

	if ( S_OK != FindAnimationByName( Anim->GetName(), IndexAdd ) )
	{
		V_RETURN( aSceletAnimation.Add( Anim, IndexAdd ) );

		Anim->IncrementLink();
	}

	if ( fSetAnim )
		SetSceletAnimation( IndexAdd, 0.0f );

	return S_OK;
}

HRESULT CResourceSceletMesh::SetSceletAnimation( UINT Index, float dt )
{
	if ( Index >= aSceletAnimation.GetCount() || aSceletAnimation[Index] == NULL )
		{
			LogFile.OutputLogFileErrorString( _T( "Not a correct index." ), __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}

	ActivAnimation = aSceletAnimation[Index];
	mDTimeScAnim = dt;
	UpdateScelet( 0.0f );
	Scelet.SetupBindPose();

	return S_OK;
}

void CResourceSceletMesh::UpdateScelet( float dt )
{
	static float deltaTime;
	deltaTime = mDTimeScAnim + dt;

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
	static UINT mScCouny;
	mScCouny = Scelet.GetBoneCount();
	for ( i = 0; i < mScCouny; ++i )
	{
		D3DXVec4Lerp( &Offset, &frame0->GetOffset( i ), &frame1->GetOffset( i ), frameLerp );
		D3DXQuaternionSlerp( &Rotation, &frame0->GetRotation( i ), &frame1->GetRotation( i ), frameLerp );
		D3DXQuaternionNormalize( &Rotation, &Rotation );

		Scelet.m_BonesTransform[i].Rotations = Rotation;
		Scelet.m_BonesTransform[i].Offsets = Offset;
	}

	Scelet.AnimateHierarhy();
}

HRESULT CResourceSceletMesh::FindAnimationByName( TCHAR* Name, UINT &Index )
{
	for ( Index = 0; Index < aSceletAnimation.GetCount(); Index++ )
		if ( aSceletAnimation[Index] != NULL )
			if ( !lstrcmp( aSceletAnimation[Index]->GetName(), Name ) )
				return S_OK;

	return S_FALSE;
}
