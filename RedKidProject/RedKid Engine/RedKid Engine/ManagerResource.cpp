#include "ManagerResource.h"

CManagerResource::CManagerResource( void )
	: aMesh( 50 ), aTexture( 50 ), aMaterial( 10 ), aSound( 20 ), aSceletAnim( 20 )
{
}

CManagerResource::~CManagerResource( void )
{
	Release();
}

void CManagerResource::Release( void )
{
	aMesh.ReleaseLP();
	aSound.ReleaseLP();
	aMaterial.ReleaseLP();
	aTexture.ReleaseLP();
	aSceletAnim.ReleaseLP();
}

HRESULT CManagerResource::CreateMatTexture( TCHAR* Name, UINT &Index, bool fScelet, TCHAR* TextureName )
{
	if ( S_OK == FindMaterialByName( Name, Index ) )
	{
		LogFile.OutputLogFileErrorString( _T( "The material with such name already is." ), __FILE__, __FUNCTION__, __LINE__  );
		return S_FALSE;
	}

	CResMatTexture* Mat = NULL;
	Mat = new CResMatTexture();
	if ( Mat == NULL )
	{
		LogFile.OutputLogFileErrorString( _T( "Memory allocation failure." ), __FILE__, __FUNCTION__, __LINE__  );
		return S_FALSE;
	}

	CResourceTexture* Texture;
	if ( S_OK != GetTexture( TextureName, Texture ) )
	{
		delete Mat;
		return S_FALSE;
	}

	Mat->Inicialise( Texture, fScelet );

	if ( S_OK != aMaterial.Add( Mat, Index ) )
	{
		delete Mat;
		return S_FALSE;
	}

	Mat->SetName( Name );
	Mat->SetIndexArray( Index );

	return S_OK;
}

HRESULT CManagerResource::FindMaterialByName( TCHAR* Name, UINT &Index )
{
	for ( Index = 0; Index < aMaterial.GetCount(); Index++ )
		if ( aMaterial[Index] != NULL )
			if ( !lstrcmp( aMaterial[Index]->GetName(), Name ) )
				return S_OK;

	return S_FALSE;
}

HRESULT CManagerResource::GetTexture( TCHAR* Name, CResourceTexture* &Texture )
{

	if ( S_OK == FindTexture( Name, Texture ) )
		return S_OK;

	Texture = NULL;
	Texture = new CResourceTexture();
	if ( Texture == NULL )
	{
		LogFile.OutputLogFileErrorString( L"Memory allocation failure.", __FILE__, __FUNCTION__, __LINE__  );
		return S_FALSE;
	}

	if ( S_OK != Texture->CreateTextureOfFile( Name ) )
	{
		delete Texture;
		return S_FALSE;
	}

	UINT Index;
	if ( S_OK != aTexture.Add( Texture, Index ) )
	{
		delete Texture;
		return S_FALSE;
	}

	Texture->SetIndexArray( Index );

	return S_OK;
}

HRESULT CManagerResource::FindTexture( TCHAR* Name, CResourceTexture* &Texture )
{
	for ( UINT Index = 0; Index < aTexture.GetCount(); Index++ )
		if ( aTexture[Index] != NULL )
			if ( !lstrcmp( aTexture[Index]->GetName(), Name ) )
			{
				Texture = aTexture[Index];
				return S_OK;
			}

	return S_FALSE;
}

HRESULT CManagerResource::Create_MeshOfTXTFile( TCHAR* FileName, UINT &Index, UINT IndexMat )
{
	if ( FileName == NULL )
	{
		LogFile.OutputLogFileErrorString( L"NULL file name.", __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}

	if ( IndexMat >= aMaterial.GetCount() || aMaterial[IndexMat] == NULL )
	{
		LogFile.OutputLogFileErrorString( L"Not a correct index of a material.", __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}

	if ( S_OK == FindMeshByName( FileName, Index ) )
	{
		LogFile.OutputLogFileErrorString( L"The mesh with such name already is.", __FILE__, __FUNCTION__, __LINE__  );
		return S_FALSE;
	}

	CResMesh* Mesh = NULL;
	if ( aMaterial[IndexMat]->GetMeshFormat()->GetfSceletData() )
	{
		Mesh = new CResourceSceletMesh();
		if ( Mesh == NULL )
		{
			LogFile.OutputLogFileErrorString( L"Memory allocation failure.", __FILE__, __FUNCTION__, __LINE__  );
			return S_FALSE;
		}
	}
	else
	{
		Mesh = new _declspec(align(16)) CResMesh();
		if ( Mesh == NULL )
		{
			LogFile.OutputLogFileErrorString( L"Memory allocation failure.", __FILE__, __FUNCTION__, __LINE__  );
			return S_FALSE;
		}
	}

	if ( S_OK != Mesh->CreateMeshOfTXTFile( FileName, aMaterial[IndexMat]->GetMeshFormat() ) )
	{
		delete Mesh;
		return S_FALSE;
	}

	if ( S_OK != Mesh->SetMaterial( aMaterial[IndexMat] ) )
	{
		delete Mesh;
		return S_FALSE;
	}

	if ( S_OK != aMesh.Add( Mesh, Index ) )
	{
		LogFile.OutputLogFileErrorString( L"Error add mesh in mesh array.", __FILE__, __FUNCTION__, __LINE__  );
		delete Mesh;
		return S_FALSE;
	}

	Mesh->SetIndexArray( Index );

	return S_OK;
}

HRESULT CManagerResource::CreatePhysXStaticTriangleMeshOfTXTFile( TCHAR* FileName, UINT IndexMat, UINT &IndexMesh )
{
	if ( FileName == NULL )
	{
		LogFile.OutputLogFileErrorString( L"NULL TXT mesh file name.", __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}

	if ( IndexMat >= aMaterial.GetCount() || aMaterial[IndexMat] == NULL )
	{
		LogFile.OutputLogFileErrorString( L"Not a correct index of a material(CreatePhysXStaticMeshOfTXTFile).", __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}

	if ( aMaterial[IndexMat]->GetMeshFormat()->GetfSceletData() )
	{
		LogFile.OutputLogFileErrorString( L"The material contains the data about a skeleton.", __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}

	CResPhysXStaticTriangleMesh* Mesh = NULL;
	if ( S_OK != FindPhysXStaticTriangleMeshByName( FileName, IndexMat, IndexMesh ) )
	{
		Mesh = new CResPhysXStaticTriangleMesh();
		if ( Mesh == NULL )
		{
			LogFile.OutputLogFileErrorString( L"Memory allocation failure.", __FILE__, __FUNCTION__, __LINE__  );
			return S_FALSE;
		}

		if ( S_OK != Mesh->CreateMeshOfTXTFile( FileName, aMaterial[IndexMat]->GetMeshFormat() ) )
		{
			delete Mesh;
			return S_FALSE;
		}

		if ( S_OK != Mesh->SetMaterial( aMaterial[IndexMat] ) )
		{
			delete Mesh;
			return S_FALSE;
		}

		if ( S_OK != aMesh.Add( Mesh, IndexMesh ) )
		{
			LogFile.OutputLogFileErrorString( L"Error add mesh in mesh array.", __FILE__, __FUNCTION__, __LINE__  );
			delete Mesh;
			return S_FALSE;
		}

		Mesh->SetIndexArray( IndexMesh );
	}

	return S_OK;
}

HRESULT CManagerResource::CreatePhysXDinamicConvexMeshOfTXTFile( TCHAR* FileName, UINT IndexMat, UINT &IndexMesh )
{
	if ( FileName == NULL )
	{
		LogFile.OutputLogFileErrorString( L"NULL TXT mesh file name.", __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}

	if ( IndexMat >= aMaterial.GetCount() || aMaterial[IndexMat] == NULL )
	{
		LogFile.OutputLogFileErrorString( L"Not a correct index of a material(CreatePhysXStaticMeshOfTXTFile).", __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}

	if ( aMaterial[IndexMat]->GetMeshFormat()->GetfSceletData() )
	{
		LogFile.OutputLogFileErrorString( L"The material contains the data about a skeleton.", __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}

	CResPhysXDinamicConvexMesh* Mesh = NULL;
	if ( S_OK != FindPhysXDinamicConvexMeshByName( FileName, IndexMat, IndexMesh ) )
	{
		Mesh = new CResPhysXDinamicConvexMesh();
		if ( Mesh == NULL )
		{
			LogFile.OutputLogFileErrorString( L"Memory allocation failure.", __FILE__, __FUNCTION__, __LINE__  );
			return S_FALSE;
		}

		if ( S_OK != Mesh->CreateMeshOfTXTFile( FileName, aMaterial[IndexMat]->GetMeshFormat() ) )
		{
			delete Mesh;
			return S_FALSE;
		}

		if ( S_OK != Mesh->SetMaterial( aMaterial[IndexMat] ) )
		{
			delete Mesh;
			return S_FALSE;
		}

		if ( S_OK != aMesh.Add( Mesh, IndexMesh ) )
		{
			LogFile.OutputLogFileErrorString( L"Error add mesh in mesh array.", __FILE__, __FUNCTION__, __LINE__  );
			delete Mesh;
			return S_FALSE;
		}

		Mesh->SetIndexArray( IndexMesh );
	}

	return S_OK;
}

HRESULT CManagerResource::FindMeshByName( TCHAR* Name, UINT &Index )
{
	for ( Index = 0; Index < aMesh.GetCount(); Index++ )
		if ( aMesh[Index] != NULL )
			if ( !lstrcmp( aMesh[Index]->GetName(), Name ) )
				return S_OK;

	return S_FALSE;
}

HRESULT CManagerResource::AddMeshSceletAnimation( UINT IndexMesh, TCHAR* FileName, UINT &IndexAddAnim, bool fSetAnim )
{
	if ( IndexMesh >= aMesh.GetCount() || aMesh[IndexMesh] == NULL )
	{
		LogFile.OutputLogFileErrorString( _T( "Not a correct index of a mesh." ), __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}

	if ( !aMesh[IndexMesh]->GetScelet() )
	{
		LogFile.OutputLogFileErrorString( _T( "The mesh does not contain a skeleton." ), __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}

	UINT IndexAnim;
	if ( S_OK != FindSceletAnimationByName( FileName, IndexAnim ) )
	{
		CResourceAnimationScelet* Anim = NULL;
		Anim = new CResourceAnimationScelet();
		if ( Anim == NULL )
		{
			LogFile.OutputLogFileErrorString( _T( "Memory allocation failure." ), __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}

		V_RETURN( Anim->CreateAnimationOfTXTFile( FileName, aMesh[IndexMesh]->GetBoneCount() ) );

		if ( S_OK != aSceletAnim.Add( Anim, IndexAnim ) )
		{
			LogFile.OutputLogFileErrorString( _T( "Error add scelet animation in array." ), __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}
	}
	else
		if ( aSceletAnim[IndexAnim]->GetBoneCount() != aMesh[IndexMesh]->GetBoneCount() )
		{
			TCHAR mESS[100];
			wsprintf( mESS, _T( "Different quantity of bones(read file: %s)." ), FileName );
			LogFile.OutputLogFileString( mESS );
			return S_FALSE;
		}

	V_RETURN( aMesh[IndexMesh]->AddSceletAnimation( aSceletAnim[IndexAnim], IndexAddAnim, fSetAnim ) );

	return S_OK;
}

// Find mesh by Type, material and Name file
HRESULT CManagerResource::FindPhysXStaticTriangleMeshByName( TCHAR* Name, UINT IndexMat, UINT &Index )
{
	for ( Index = 0; Index < aMesh.GetCount(); Index++ )
		if ( aMesh[Index] != NULL )
			if ( aMesh[Index]->GetType() == RES_PHYSX_TRIANGLE_STATIC_MESH )
				if ( aMaterial[IndexMat] == aMesh[Index]->GetMaterial() )
					if ( !lstrcmp( aMesh[Index]->GetName(), Name ) )
						return S_OK;

	return S_FALSE;
}

HRESULT CManagerResource::FindPhysXDinamicConvexMeshByName( TCHAR* Name, UINT IndexMat, UINT &Index )
{
	for ( Index = 0; Index < aMesh.GetCount(); Index++ )
		if ( aMesh[Index] != NULL )
			if ( aMesh[Index]->GetType() == RES_PHYSX_CONVEX_DINAMIC_MESH )
				if ( aMaterial[IndexMat] == aMesh[Index]->GetMaterial() )
					if ( !lstrcmp( aMesh[Index]->GetName(), Name ) )
						return S_OK;

	return S_FALSE;
}

HRESULT CManagerResource::FindSceletAnimationByName( TCHAR* Name, UINT &Index )
{
	for ( Index = 0; Index < aSceletAnim.GetCount(); Index++ )
		if ( aSceletAnim[Index] != NULL )
			if ( !lstrcmp( aSceletAnim[Index]->GetName(), Name ) )
				return S_OK;

	return S_FALSE;
}