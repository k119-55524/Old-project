#include "System.h"
#include "LogFile.h"
#include "ResMesh.h"

CResMesh::CResMesh( void )
{
	Type = RES_MESH;
	mVertexCount = 0;
	mTriangleCount = 0;
	mIndexCount = 0;
	mRealArraySize = 0;

	cMaterial = NULL;
	IndexRenderMeshArray = -1;

	g_pRenderVertexBuffer = &g_pVertexBuffer;
	g_pVertexBuffer = NULL;
	g_pIndexBuffer = NULL;
	StructSize = 0;
}

CResMesh::~CResMesh( void )
{
	Release();
}

void CResMesh::Release( void )
{
	mVertexCount = 0;
	mTriangleCount = 0;
	mIndexCount = 0;
	mRealArraySize = 0;

	if ( cMaterial != NULL )
	{
		cMaterial->DecrementLink();
		cMaterial = NULL;
	}

	cMeshFormat.Release();

	RELEASE( g_pIndexBuffer );
	RELEASE( g_pVertexBuffer );
	StructSize = 0;
}

void CResMesh::CalcAABB( D3DXMATRIX* m_GlobalWordMat, CAABB &cAABB )
{
}

HRESULT CResMesh::CreateMeshOfTXTFile( TCHAR* FileName, CMeshFormat* mForm )
{
	std::ifstream file;

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
		Release();

		std::string Line;

		file >> Line;
		if ( Line != "[RedMax3D.com._RedKid_PlugIn_V4.0.0.]" )
		{
			TCHAR __eRR[1024];
			wsprintf( __eRR, L"Unknow format file(open txt file: %s).", FileName );
			LogFile.OutputLogFileErrorString( __eRR, __FILE__, __FUNCTION__, __LINE__ );
			throw 1;
		}

		file >> Line;
		if ( Line != "[Eport_mesh,_sceleton_and_sceleton_animation_of_3Ds_Max,_in_text_format_for_DirectX_dimension.]" )
		{
			TCHAR __eRR[1024];
			wsprintf( __eRR, L"Unknow format file(open txt file: %s).", FileName );
			LogFile.OutputLogFileErrorString( __eRR, __FILE__, __FUNCTION__, __LINE__ );
			throw 1;
		}

		// Если надо то пропускаем блок с костями
		file >> Line;

		if ( mForm->GetfSceletData() && ( Line != "[Start_Scelet_Block]" ) )
		{
			TCHAR __eRR[1024];
			wsprintf( __eRR, L"No scelet data(open txt file: %s).", FileName );
			LogFile.OutputLogFileErrorString( __eRR, __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;

			throw 1;
		}

		if ( Line == "[Start_Scelet_Block]" )
		{
			if ( S_OK != LoadSceletBlock( file, FileName ) )
				throw 1;

			file >> Line;
			file >> Line;
		}

		if ( Line != "[Start_Mesh_block]" )
		{
			TCHAR __eRR[1024];
			wsprintf( __eRR, L"Unknow format file(open txt file: %s).", FileName );
			LogFile.OutputLogFileErrorString( __eRR, __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}

		if ( S_OK != LoadMeshBlock( file, mForm, FileName ) )
			throw 1;

		file >> Line;
		if ( Line != "[End_Mesh_block]" )
		{
			TCHAR __eRR[1024];
			wsprintf( __eRR, L"Unknow format file(open txt file: %s).", FileName );
			LogFile.OutputLogFileErrorString( __eRR, __FILE__, __FUNCTION__, __LINE__ );
			throw 1;
		}

		file.close();
	}
	catch ( ... )
	{
		file.close();
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

HRESULT CResMesh::LoadMeshBlock( std::ifstream &file, CMeshFormat* mForm, TCHAR* FileName )
{
	cMeshFormat.Release();
	CMeshFormat LocalFormat;

	std::string Line;
	file >> Line;
	file >> Line;

	if ( Line == "Normal:TRUE." )
	{
		LocalFormat.SetfNormal();
		file >> Line;
		if ( Line == "TangBinor:TRUE." )
			LocalFormat.SetfTangBinor();
		else
			if ( Line == "TangBinor:FALSE." )
			{
				if ( mForm->GetfTangBinor() )
				{
					LogFile.OutputLogFileErrorString( _T( "In a file there is no data about tangent & binormal." ), __FILE__, __FUNCTION__, __LINE__ );
					return S_FALSE;
				}
			}
			else
			{
				TCHAR __eRR[1024];
				wsprintf( __eRR, L"Unknow format file(open txt file: %s).", FileName );
				LogFile.OutputLogFileErrorString( __eRR, __FILE__, __FUNCTION__, __LINE__ );
				return S_FALSE;
			}
	}
	else
		if ( Line == "Normal:FALSE." )
		{
			if ( mForm->GetfNormal() )
			{
				LogFile.OutputLogFileErrorString( _T( "In a file there is no data about normals." ), __FILE__, __FUNCTION__, __LINE__ );
				return S_FALSE;
			}
		}
		else
			{
				TCHAR __eRR[1024];
				wsprintf( __eRR, L"Unknow format file(open txt file: %s).", FileName );
				LogFile.OutputLogFileErrorString( __eRR, __FILE__, __FUNCTION__, __LINE__ );
				return S_FALSE;
			}

	file >> Line;
	if ( Line == "UVW:TRUE." )
		LocalFormat.SetfUV();
	else
		if ( Line == "UVW:FALSE." )
			if ( mForm->GetfUV() )
			{
				LogFile.OutputLogFileErrorString( _T( "In a file there is no data about UV." ), __FILE__, __FUNCTION__, __LINE__ );
				return S_FALSE;
			}
			else
			{
				TCHAR __eRR[1024];
				wsprintf( __eRR, L"Unknow format file(open txt file: %s).", FileName );
				LogFile.OutputLogFileErrorString( __eRR, __FILE__, __FUNCTION__, __LINE__ );
				return S_FALSE;
			}

	file >> Line;
	if ( Line == "SceletData:TRUE." )
		LocalFormat.SetfSceletData();
	else
		if ( Line == "SceletData:FALSE." )
		{
			if ( mForm->GetfSceletData() )
			{
				LogFile.OutputLogFileErrorString( _T( "In a file there is no data about scelet data." ), __FILE__, __FUNCTION__, __LINE__ );
				return S_FALSE;
			}
		}
		else
		{
			TCHAR __eRR[1024];
			wsprintf( __eRR, L"Unknow format file(open txt file: %s).", FileName );
			LogFile.OutputLogFileErrorString( __eRR, __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}

	file >> Line;
	if ( Line != "Vertex_count:" )
	{
		TCHAR __eRR[1024];
		wsprintf( __eRR, L"Unknow format file(open txt file: %s).", FileName );
		LogFile.OutputLogFileErrorString( __eRR, __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}

	file >> mVertexCount;

	static LPVERTEX_NORMAL_UV		pV_NOR_UV;
	static LPVERTEX_NORMAL_UV_BONE	pV_NOR_UV_BON;
	static LPVERTEX_TBN_UV			pV_TBN_UV;
	static LPVERTEX_TBN_UV_BONE		pV_TBN_UV_BON;
	pV_NOR_UV		= NULL;
	pV_NOR_UV_BON	= NULL;
	pV_TBN_UV		= NULL;
	pV_TBN_UV_BON	= NULL;

	mRealArraySize = (UINT)ceil( (float)mVertexCount / 128.0f ) * 128;

	///////////////////////////////////////////////////////////////////////////
	//
	if ( mForm->GetfNormal() && mForm->GetfTangBinor() && mForm->GetfUV() && mForm->GetfSceletData() )
	{
		pV_TBN_UV_BON = new VERTEX_TBN_UV_BONE[mRealArraySize];
		if ( pV_TBN_UV_BON == NULL )
		{
			LogFile.OutputLogFileErrorString( L"Error of allocation of memory.", __FILE__, __FUNCTION__, __LINE__ );
			throw 1;
		}
		ZeroMemory( pV_TBN_UV_BON, sizeof(VERTEX_TBN_UV_BONE) * mRealArraySize );
		StructSize = sizeof(VERTEX_TBN_UV);
	}
	else
		if ( mForm->GetfNormal() && mForm->GetfTangBinor() && mForm->GetfUV() )
		{
			pV_TBN_UV = new VERTEX_TBN_UV[mRealArraySize];
			if ( pV_TBN_UV == NULL )
			{
				LogFile.OutputLogFileErrorString( L"Error of allocation of memory.", __FILE__, __FUNCTION__, __LINE__ );
				throw 1;
			}
			ZeroMemory( pV_TBN_UV, sizeof(VERTEX_TBN_UV) * mRealArraySize );
			StructSize = sizeof(VERTEX_TBN_UV);
		}
		else
			if ( mForm->GetfNormal() && mForm->GetfUV() && mForm->GetfSceletData() )
			{
				pV_NOR_UV_BON = new VERTEX_NORMAL_UV_BONE[mRealArraySize];
				if ( pV_NOR_UV_BON == NULL )
				{
					LogFile.OutputLogFileErrorString( L"Error of allocation of memory.", __FILE__, __FUNCTION__, __LINE__ );
					throw 1;
				}
				ZeroMemory( pV_NOR_UV_BON, sizeof(VERTEX_NORMAL_UV) * mRealArraySize );
				StructSize = sizeof(VERTEX_NORMAL_UV_BONE);
			}
			else
				{
					pV_NOR_UV = new VERTEX_NORMAL_UV[mRealArraySize];
					if ( pV_NOR_UV == NULL )
					{
						LogFile.OutputLogFileErrorString( L"Error of allocation of memory.", __FILE__, __FUNCTION__, __LINE__ );
						throw 1;
					}
					ZeroMemory( pV_NOR_UV, sizeof(VERTEX_NORMAL_UV) * mRealArraySize );
					StructSize = sizeof(VERTEX_NORMAL_UV);
				}
	//
	///////////////////////////////////////////////////////////////////////////

	LPVERTEX_TBN_UV_BONE m_Vertex = NULL;
	m_Vertex = new VERTEX_TBN_UV_BONE[mRealArraySize];
	if ( m_Vertex == NULL )
	{
		LogFile.OutputLogFileErrorString( L"Error of allocation of memory.", __FILE__, __FUNCTION__, __LINE__ );
		throw 1;
	}
	ZeroMemory( m_Vertex, sizeof(VERTEX_TBN_UV_BONE) * mRealArraySize );

	UINT i, j;
	UINT cBuf;
	cBuf = 0;
	LPVERTEXWEIGHT vwBuf;
	vwBuf = NULL;
	float acW;
	UINT c1,c2;
	VERTEXWEIGHT vw;
	LPDWORD m_Indexes;
	try
	{
		for ( i = 0; i < mVertexCount; i++ )
		{
			file >> m_Vertex[i].Pos.x >> m_Vertex[i].Pos.y >> m_Vertex[i].Pos.z;

			if ( LocalFormat.GetfNormal() )
			{
				file >> m_Vertex[i].Normal.x >> m_Vertex[i].Normal.y >> m_Vertex[i].Normal.z;
				if ( LocalFormat.GetfTangBinor() )
				{
					file >> m_Vertex[i].Tangent.x >> m_Vertex[i].Tangent.y >> m_Vertex[i].Tangent.z;
					file >> m_Vertex[i].BiNormal.x >> m_Vertex[i].BiNormal.y >> m_Vertex[i].BiNormal.z;
				}
			}

			if ( LocalFormat.GetfUV() )
				file >> m_Vertex[i].UV.x >> m_Vertex[i].UV.y;

			if ( LocalFormat.GetfSceletData() )
			{
				file >> m_Vertex[i].NumWeigth;

				if ( m_Vertex[i].NumWeigth <= MAX_WEIGHTS )
					for ( j = 0; j < m_Vertex[i].NumWeigth; ++j )
						file >> m_Vertex[i].Weigth[j].BoneID >> m_Vertex[i].Weigth[j].Weigth;
				else
				// Отбрасываем лишние кости
				{
					// Изменяем размер промежуточного массива
					if ( cBuf < m_Vertex[i].NumWeigth )
					{
						DEL_ARRAY( vwBuf );
						vwBuf = new VERTEXWEIGHT[m_Vertex[i].NumWeigth];
						if ( vwBuf == NULL )
						{
							Release();
							LogFile.OutputLogFileErrorString( L"Error of allocation of memory.", __FILE__, __FUNCTION__, __LINE__ );
							return S_FALSE;
						}
					}

					// Читаем веса
					for ( j = 0; j < m_Vertex[i].NumWeigth; ++j )
						file >> vwBuf[j].BoneID >> vwBuf[j].Weigth;

					// Сортируем(для отбрасывания наименьших весов)
					for ( c1 = 0; c1 < m_Vertex[i].NumWeigth - 1; c1++ )
						for ( c2 = c1 + 1; c2 < m_Vertex[i].NumWeigth; c2++ )
						{
							if ( vwBuf[c1].Weigth < vwBuf[c2].Weigth )
							{
								vw = vwBuf[c1];
								vwBuf[c1] = vwBuf[c2];
								vwBuf[c2] = vw;
							}
						}

					// Перераспределяем веса
					acW = 0.0f;
					for ( j = 0; j < MAX_WEIGHTS; j++ )
							acW += vwBuf[j].Weigth;
					acW = 1.0f / acW;

					// Переносим в основной массив с учетом обрезания лишней информации
					for ( j = 0; j < MAX_WEIGHTS; j ++ )
					{
						m_Vertex[i].Weigth[j].BoneID = vwBuf[j].BoneID;
						m_Vertex[i].Weigth[j].Weigth = vwBuf[j].Weigth * acW;
					}

					cBuf = m_Vertex[i].NumWeigth;

					m_Vertex[i].NumWeigth = MAX_WEIGHTS;
				}
			}

			///////////////////////////////////////////////////////////////////////////
			// Чтение вертекса закончено, начинаем заполнять соответствующий массив
			if ( mForm->GetfNormal() && mForm->GetfTangBinor() && mForm->GetfUV() && mForm->GetfSceletData() )
			{
				pV_TBN_UV_BON[i] = m_Vertex[i];
			}
			else
				if ( mForm->GetfNormal() && mForm->GetfTangBinor() && mForm->GetfUV() )
				{
					pV_TBN_UV[i].Pos = m_Vertex[i].Pos;
					pV_TBN_UV[i].Normal = m_Vertex[i].Normal;
					pV_TBN_UV[i].BiNormal = m_Vertex[i].BiNormal;
					pV_TBN_UV[i].Tangent = m_Vertex[i].Tangent;
					pV_TBN_UV[i].UV = m_Vertex[i].UV;
				}
				else
					if ( mForm->GetfNormal() && mForm->GetfUV() && mForm->GetfSceletData() )
					{
						pV_NOR_UV_BON[i].Pos = m_Vertex[i].Pos;
						pV_NOR_UV_BON[i].Normal = m_Vertex[i].Normal;
						pV_NOR_UV_BON[i].UV = m_Vertex[i].UV;
						pV_NOR_UV_BON[i].NumWeigth = m_Vertex[i].NumWeigth;
						for ( j = 0; j < MAX_WEIGHTS; j++ )
							pV_NOR_UV_BON[i].Weigth[j] = m_Vertex[i].Weigth[j];
					}
					else
						{
							pV_NOR_UV[i].Pos = m_Vertex[i].Pos;
							pV_NOR_UV[i].Normal = m_Vertex[i].Normal;
							pV_NOR_UV[i].UV = m_Vertex[i].UV;
						}
			//
			///////////////////////////////////////////////////////////////////////////
		}

		DEL_ARRAY( vwBuf );

		///////////////////////////////////////////////////////////////////////////////
		//
		D3D11_BUFFER_DESC bd;
		ZeroMemory( &bd, sizeof(D3D11_BUFFER_DESC) );
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory( &InitData, sizeof(D3D11_SUBRESOURCE_DATA) );

		bd.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_VERTEX_BUFFER;
		bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS; //D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

		if ( mForm->GetfNormal() && mForm->GetfTangBinor() && mForm->GetfUV() && mForm->GetfSceletData() )
		{
			InitData.pSysMem = pV_TBN_UV_BON;
			bd.ByteWidth = sizeof( VERTEX_TBN_UV_BONE ) * mRealArraySize;
			bd.StructureByteStride = sizeof( VERTEX_TBN_UV_BONE );
		}
		else
			if ( mForm->GetfNormal() && mForm->GetfTangBinor() && mForm->GetfUV() )
			{
				InitData.pSysMem = pV_TBN_UV;
				bd.ByteWidth = sizeof( VERTEX_TBN_UV ) * mRealArraySize;
				bd.StructureByteStride = sizeof( VERTEX_TBN_UV );
			}
			else
				if ( mForm->GetfNormal() && mForm->GetfUV() && mForm->GetfSceletData() )
				{
					InitData.pSysMem = pV_NOR_UV_BON;
					bd.ByteWidth = sizeof( VERTEX_NORMAL_UV_BONE ) * mRealArraySize;
					bd.StructureByteStride = sizeof( VERTEX_NORMAL_UV_BONE );
				}
				else
					{
						InitData.pSysMem = pV_NOR_UV;
						bd.ByteWidth = sizeof( VERTEX_NORMAL_UV ) * mRealArraySize;
						bd.StructureByteStride = sizeof( VERTEX_NORMAL_UV );
					}
		//
		///////////////////////////////////////////////////////////////////////////////

		res = SystemRedKid.GetDevice()->CreateBuffer( &bd, &InitData, &g_pVertexBuffer );

		DEL_ARRAY( pV_NOR_UV );
		DEL_ARRAY( pV_NOR_UV_BON );
		DEL_ARRAY( pV_TBN_UV );
		DEL_ARRAY( pV_TBN_UV_BON );

		if ( res != S_OK )
		{
			LogFile.OutputLogFileErrorString( L"Error create vertex buffer.", __FILE__, __FUNCTION__, __LINE__ );
			throw 1;
		}

		if ( g_pBufferSRV.Create( g_pVertexBuffer ) )
		{
			LogFile.OutputLogFileErrorString( L"Error create bufferSRV.", __FILE__, __FUNCTION__, __LINE__ );
			throw 1;
		}

		file >> Line;
		if ( Line != "[Start_Index_Block]" )
		{
			TCHAR __eRR[1024];
			wsprintf( __eRR, L"Unknow format file(open txt file: %s).", FileName );
			LogFile.OutputLogFileErrorString( __eRR, __FILE__, __FUNCTION__, __LINE__ );
			throw 1;
		}

		file >> Line;
		if ( Line != "Index_count:" )
		{
			TCHAR __eRR[1024];
			wsprintf( __eRR, L"Unknow format file(open txt file: %s).", FileName );
			LogFile.OutputLogFileErrorString( __eRR, __FILE__, __FUNCTION__, __LINE__ );
			throw 1;
		}

		file >> mIndexCount;
		m_Indexes = NULL;
		m_Indexes = new DWORD[mIndexCount];
		if ( m_Indexes == NULL )
		{
			LogFile.OutputLogFileErrorString( L"Error of allocation of memory.", __FILE__, __FUNCTION__, __LINE__ );
			throw 1;
		}

		for ( i = 0; i < mIndexCount; i++ )
			file >> m_Indexes[i];

		if ( S_OK != InitResIndividualData( m_Vertex, mVertexCount, m_Indexes, mIndexCount ) )
			throw 1;

		DEL_ARRAY( m_Vertex );

		ZeroMemory( &bd, sizeof(bd) );
		bd.ByteWidth = sizeof(DWORD) * mIndexCount;	// Размер буффера в байтах
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;		// Флаги определяют в каком качестве буффер связывается с граф. конвейером
		InitData.pSysMem = m_Indexes;

		// Создаём индексный буффер
		res = SystemRedKid.GetDevice()->CreateBuffer( &bd, &InitData, &g_pIndexBuffer );
		if ( res != S_OK )
		{
			LogFile.OutputLogFileErrorString( L"Error create index buffer.", __FILE__, __FUNCTION__, __LINE__ );
			throw 1;
		}

		DEL_ARRAY( m_Indexes );

		file >> Line;
		if ( Line != "[End_Index_Block]" )
		{
			TCHAR __eRR[1024];
			wsprintf( __eRR, L"Unknow format file(open txt file: %s).", FileName );
			LogFile.OutputLogFileErrorString( __eRR, __FILE__, __FUNCTION__, __LINE__ );
			throw 1;
		}
	}
	catch( ... )
	{
		file.close();

		DEL_ARRAY( m_Vertex );
		DEL_ARRAY( m_Indexes );

		DEL_ARRAY( vwBuf );

		DEL_ARRAY( pV_NOR_UV );
		DEL_ARRAY( pV_NOR_UV_BON );
		DEL_ARRAY( pV_TBN_UV );
		DEL_ARRAY( pV_TBN_UV_BON );

		Release();
		TCHAR __eRR[1024];
		wsprintf( __eRR, L"Unknow error mesh block read(open txt file: %s).", FileName );
		LogFile.OutputLogFileErrorString( __eRR, __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}
	
	mTriangleCount = mIndexCount / 3;
	cMeshFormat = *mForm;

	return S_OK;
}

HRESULT CResMesh::LoadSceletBlock( std::ifstream &file, TCHAR* FileName )
{
	std::string Line;

	file >> Line;
	if ( Line != "Bones_Count:" )
	{
		TCHAR __eRR[1024];
		wsprintf( __eRR, L"Unknow format file(open txt file: %s).", FileName );
		LogFile.OutputLogFileErrorString( __eRR, __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}

	UINT NumBones;
	file >> NumBones;
	std::string name;
	int id, parentID;
	for ( UINT i = 0; i < NumBones; i++ )
		file >> name >> id >> parentID;

	file >> Line;
	if ( Line != "[End_Scelet_Block]" )
	{
		TCHAR __eRR[1024];
		wsprintf( __eRR, L"Unknow format file(open txt file: %s).", FileName );
		LogFile.OutputLogFileErrorString( __eRR, __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}

	return S_OK;
}