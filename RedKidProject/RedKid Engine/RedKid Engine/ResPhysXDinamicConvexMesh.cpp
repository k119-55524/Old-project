#include "ResPhysXDinamicConvexMesh.h"

CResPhysXDinamicConvexMesh::CResPhysXDinamicConvexMesh( void )
{
	Type = RES_PHYSX_CONVEX_DINAMIC_MESH;

	//PhysX
	pVerts = NULL;
	PhysXMesh = NULL;
	pIndex = NULL;
}

CResPhysXDinamicConvexMesh::~CResPhysXDinamicConvexMesh( void )
{
	Release();
}

void CResPhysXDinamicConvexMesh::Release( void )
{
	rELEASE( PhysXMesh );
	DEL_ARRAY( pVerts );
	DEL_ARRAY( pIndex );
}

HRESULT CResPhysXDinamicConvexMesh::LoadMeshBlock( std::ifstream &file, CMeshFormat* mForm, TCHAR* FileName )
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

	// PhysX
	pVerts = new _declspec(align(16)) PxVec3[mVertexCount];
	if ( pVerts == NULL )
	{
		Release();
		LogFile.OutputLogFileErrorString( L"Error of allocation of memory(PhysX).", __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}

	static LPVERTEX_NORMAL_UV		pV_NOR_UV;
	static LPVERTEX_NORMAL_UV_BONE	pV_NOR_UV_BON;
	static LPVERTEX_TBN_UV			pV_TBN_UV;
	static LPVERTEX_TBN_UV_BONE		pV_TBN_UV_BON;
	pV_NOR_UV		= NULL;
	pV_NOR_UV_BON	= NULL;
	pV_TBN_UV		= NULL;
	pV_TBN_UV_BON	= NULL;

	///////////////////////////////////////////////////////////////////////////
	//
	if ( mForm->GetfNormal() && mForm->GetfTangBinor() && mForm->GetfUV() && mForm->GetfSceletData() )
	{
		pV_TBN_UV_BON = new VERTEX_TBN_UV_BONE[mVertexCount];
		if ( pV_TBN_UV_BON == NULL )
		{
			Release();
			LogFile.OutputLogFileErrorString( L"Error of allocation of memory.", __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}
		ZeroMemory( pV_TBN_UV_BON, sizeof(VERTEX_TBN_UV_BONE) * mVertexCount );
		StructSize = sizeof(VERTEX_TBN_UV);
	}
	else
		if ( mForm->GetfNormal() && mForm->GetfTangBinor() && mForm->GetfUV() )
		{
			pV_TBN_UV = new VERTEX_TBN_UV[mVertexCount];
			if ( pV_TBN_UV == NULL )
			{
				Release();
				LogFile.OutputLogFileErrorString( L"Error of allocation of memory.", __FILE__, __FUNCTION__, __LINE__ );
				return S_FALSE;
			}
			ZeroMemory( pV_TBN_UV, sizeof(VERTEX_TBN_UV) * mVertexCount );
			mVertexCount = sizeof(VERTEX_TBN_UV);
		}
		else
			if ( mForm->GetfNormal() && mForm->GetfUV() && mForm->GetfSceletData() )
			{
				pV_NOR_UV_BON = new VERTEX_NORMAL_UV_BONE[mVertexCount];
				if ( pV_NOR_UV_BON == NULL )
				{
					Release();
					LogFile.OutputLogFileErrorString( L"Error of allocation of memory.", __FILE__, __FUNCTION__, __LINE__ );
					return S_FALSE;
				}
				ZeroMemory( pV_NOR_UV_BON, sizeof(VERTEX_NORMAL_UV) * mVertexCount );
				StructSize = sizeof(VERTEX_NORMAL_UV_BONE);
			}
			else
				{
					pV_NOR_UV = new VERTEX_NORMAL_UV[mVertexCount];
					if ( pV_NOR_UV == NULL )
					{
						Release();
						LogFile.OutputLogFileErrorString( L"Error of allocation of memory.", __FILE__, __FUNCTION__, __LINE__ );
						return S_FALSE;
					}
					ZeroMemory( pV_NOR_UV, sizeof(VERTEX_NORMAL_UV) * mVertexCount );
					StructSize = sizeof(VERTEX_NORMAL_UV);
				}
	//
	///////////////////////////////////////////////////////////////////////////

	VERTEX_TBN_UV_BONE Point;
	ZeroMemory( &Point, sizeof(VERTEX_TBN_UV_BONE) );
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
			file >> Point.Pos.x >> Point.Pos.y >> Point.Pos.z;

			// PhysX
			pVerts[i].x = Point.Pos.x;
			pVerts[i].y = Point.Pos.y;
			pVerts[i].z = Point.Pos.z;

			if ( LocalFormat.GetfNormal() )
			{
				file >> Point.Normal.x >> Point.Normal.y >> Point.Normal.z;
				if ( LocalFormat.GetfTangBinor() )
				{
					file >> Point.Tangent.x >> Point.Tangent.y >> Point.Tangent.z;
					file >> Point.BiNormal.x >> Point.BiNormal.y >> Point.BiNormal.z;
				}
			}

			if ( LocalFormat.GetfUV() )
				file >> Point.UV.x >> Point.UV.y;

			if ( LocalFormat.GetfSceletData() )
			{
				file >> Point.NumWeigth;

				if ( Point.NumWeigth <= MAX_WEIGHTS )
					for ( j = 0; j < Point.NumWeigth; ++j )
						file >> Point.Weigth[j].BoneID >> Point.Weigth[j].Weigth;
				else
				// Отбрасываем лишние кости
				{
					// Изменяем размер промежуточного массива
					if ( cBuf < Point.NumWeigth )
					{
						DEL_ARRAY( vwBuf );
						vwBuf = new VERTEXWEIGHT[Point.NumWeigth];
						if ( vwBuf == NULL )
						{
							Release();
							LogFile.OutputLogFileErrorString( L"Error of allocation of memory.", __FILE__, __FUNCTION__, __LINE__ );
							return S_FALSE;
						}
					}

					// Читаем веса
					for ( j = 0; j < Point.NumWeigth; ++j )
						file >> vwBuf[j].BoneID >> vwBuf[j].Weigth;

					// Сортируем(для отбрасывания наименьших весов)
					for ( c1 = 0; c1 < Point.NumWeigth - 1; c1++ )
						for ( c2 = c1 + 1; c2 < Point.NumWeigth; c2++ )
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
						Point.Weigth[j].BoneID = vwBuf[j].BoneID;
						Point.Weigth[j].Weigth = vwBuf[j].Weigth * acW;
					}

					cBuf = Point.NumWeigth;

					Point.NumWeigth = MAX_WEIGHTS;
				}
			}

			///////////////////////////////////////////////////////////////////////////
			// Чтение вертекса закончено, начинаем заполнять соответствующий массив
			if ( mForm->GetfNormal() && mForm->GetfTangBinor() && mForm->GetfUV() && mForm->GetfSceletData() )
			{
				pV_TBN_UV_BON[i] = Point;
			}
			else
				if ( mForm->GetfNormal() && mForm->GetfTangBinor() && mForm->GetfUV() )
				{
					pV_TBN_UV[i].Pos = Point.Pos;
					pV_TBN_UV[i].Normal = Point.Normal;
					pV_TBN_UV[i].BiNormal = Point.BiNormal;
					pV_TBN_UV[i].Tangent = Point.Tangent;
					pV_TBN_UV[i].UV = Point.UV;
				}
				else
					if ( mForm->GetfNormal() && mForm->GetfUV() && mForm->GetfSceletData() )
					{
						pV_NOR_UV_BON[i].Pos = Point.Pos;
						pV_NOR_UV_BON[i].Normal = Point.Normal;
						pV_NOR_UV_BON[i].UV = Point.UV;
						pV_NOR_UV_BON[i].NumWeigth = Point.NumWeigth;
						for ( j = 0; j < MAX_WEIGHTS; j++ )
							pV_NOR_UV_BON[i].Weigth[j] = Point.Weigth[j];
					}
					else
						{
							pV_NOR_UV[i].Pos = Point.Pos;
							pV_NOR_UV[i].Normal = Point.Normal;
							pV_NOR_UV[i].UV = Point.UV;
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
			bd.ByteWidth = sizeof( VERTEX_TBN_UV_BONE ) * mVertexCount;
			bd.StructureByteStride = sizeof( VERTEX_TBN_UV_BONE );
		}
		else
			if ( mForm->GetfNormal() && mForm->GetfTangBinor() && mForm->GetfUV() )
			{
				InitData.pSysMem = pV_TBN_UV;
				bd.ByteWidth = sizeof( VERTEX_TBN_UV ) * mVertexCount;
				bd.StructureByteStride = sizeof( VERTEX_TBN_UV );
			}
			else
				if ( mForm->GetfNormal() && mForm->GetfUV() && mForm->GetfSceletData() )
				{
					InitData.pSysMem = pV_NOR_UV_BON;
					bd.ByteWidth = sizeof( VERTEX_NORMAL_UV_BONE ) * mVertexCount;
					bd.StructureByteStride = sizeof( VERTEX_NORMAL_UV_BONE );
				}
				else
					{
						InitData.pSysMem = pV_NOR_UV;
						bd.ByteWidth = sizeof( VERTEX_NORMAL_UV ) * mVertexCount;
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
			Release();
			LogFile.OutputLogFileErrorString( L"Error create vertex buffer.", __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}

		if ( g_pBufferSRV.Create( g_pVertexBuffer ) )
		{
			Release();
			LogFile.OutputLogFileErrorString( L"Error create bufferSRV.", __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}

		file >> Line;
		if ( Line != "[Start_Index_Block]" )
		{
			Release();
			TCHAR __eRR[1024];
			wsprintf( __eRR, L"Unknow format file(open txt file: %s).", FileName );
			LogFile.OutputLogFileErrorString( __eRR, __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}

		file >> Line;
		if ( Line != "Index_count:" )
		{
			Release();
			TCHAR __eRR[1024];
			wsprintf( __eRR, L"Unknow format file(open txt file: %s).", FileName );
			LogFile.OutputLogFileErrorString( __eRR, __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}

		file >> mIndexCount;

		m_Indexes = NULL;
		m_Indexes = new DWORD[mIndexCount];
		if ( m_Indexes == NULL )
		{
			Release();
			LogFile.OutputLogFileErrorString( L"Error of allocation of memory.", __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}

		// PhysX
		pIndex = new PxU32[mIndexCount * 3];
		if ( pIndex == NULL )
		{
			Release();
			LogFile.OutputLogFileErrorString( L"Error of allocation of memory(PhysX).", __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}

		for ( i = 0; i < mIndexCount; i++ )
		{
			file >> m_Indexes[i];

			// PhysX
			pIndex[i] = m_Indexes[i];
		}

		ZeroMemory( &bd, sizeof(bd) );
		bd.ByteWidth = sizeof(DWORD) * mIndexCount;	// Размер буффера в байтах
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;		// Флаги определяют в каком качестве буффер связывается с граф. конвейером
		InitData.pSysMem = m_Indexes;

		// Создаём индексный буффер
		res = SystemRedKid.GetDevice()->CreateBuffer( &bd, &InitData, &g_pIndexBuffer );
		if ( res != S_OK )
		{
			Release();
			file.close();
			LogFile.OutputLogFileErrorString( L"Error create index buffer.", __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////
		// PhysX
		PxConvexMeshDesc convexDesc;
		convexDesc.points.count		= mVertexCount;
		convexDesc.points.stride	= sizeof(PxVec3);
		convexDesc.points.data		= pVerts;
		convexDesc.triangles.count	= mIndexCount / 3;
		convexDesc.triangles.stride = 3 * sizeof( PxU32 );
		convexDesc.triangles.data	= pIndex;
		convexDesc.flags			= PxConvexFlag::eCOMPUTE_CONVEX;

		//if ( !convexDesc.isValid() )
		//{
		//	Release();
		//	LogFile.OutputLogFileErrorString( L"Error PxCreateCooking (PhysX).", __FILE__, __FUNCTION__, __LINE__ );
		//	return S_FALSE;
		//}

		MemoryWriteBuffer buf;
		if ( !SystemRedKid.GetPhysXCooking()->cookConvexMesh( convexDesc, buf ) )
		{
			Release();
			LogFile.OutputLogFileErrorString( L"Error cookConvexMesh (PhysX).", __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}

		PhysXMesh = SystemRedKid.GetPhysX()->createConvexMesh( MemoryReadBuffer( buf.data ) );
		if ( PhysXMesh == NULL )
		{
			Release();
			LogFile.OutputLogFileErrorString( L"Error createConvexMesh (PhysX).", __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}

		DEL_ARRAY( pVerts );

		//
		///////////////////////////////////////////////////////////////////////////////////////////////

		file >> Line;
		if ( Line != "[End_Index_Block]" )
		{
			Release();
			TCHAR __eRR[1024];
			wsprintf( __eRR, L"Unknow format file(open txt file: %s).", FileName );
			LogFile.OutputLogFileErrorString( __eRR, __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}
	}
	catch( ... )
	{
		DEL_ARRAY( vwBuf );

		DEL_ARRAY( pV_NOR_UV );
		DEL_ARRAY( pV_NOR_UV_BON );
		DEL_ARRAY( pV_TBN_UV );
		DEL_ARRAY( pV_TBN_UV_BON );

		DEL_ARRAY( m_Indexes );

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