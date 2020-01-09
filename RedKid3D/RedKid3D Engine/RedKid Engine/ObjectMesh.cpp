#include "ObjectMesh.h"

CObjectMesh::CObjectMesh( TCHAR* name )
	: CObjectBase( name )
{
	Mesh = NULL;
	Type = OBJ_MESH;
}

CObjectMesh::CObjectMesh( void )
	: CObjectBase()
{
	Mesh = NULL;
}

CObjectMesh::~CObjectMesh( void )
{
	Release();
}

void CObjectMesh::Release( void )
{
	if ( Mesh != NULL )
	{
		Mesh->DecrementLink();
		Mesh = NULL;
	}
}

HRESULT CObjectMesh::AddMesh( CResMesh* mesh )
{
	if ( Mesh != NULL )
		Mesh->DecrementLink();

	Mesh = mesh;

	Mesh->IncrementLink();

	return S_OK;
}