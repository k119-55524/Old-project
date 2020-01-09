#pragma once

#include "header.h"
#include "CArray.h"
#include "ResMat.h"
#include "LogFile.h"
#include "ResMesh.h"
#include "ResSound.h"
#include "ResMatTexture.h"
#include "ResourceTexture.h"
#include "ResourceSceletMesh.h"
#include "ResourceAnimationScelet.h"
#include "ResPhysXDinamicConvexMesh.h"
#include "ResPhysXStaticTriangleMesh.h"

class CManagerResource
{
	friend class CRedKid;

public:
	CManagerResource( void );
	~CManagerResource( void );
	void Release( void );

	HRESULT CreateMatTexture( TCHAR* Name, UINT &Index, bool fScelet, TCHAR* TextureName );

	HRESULT Create_MeshOfTXTFile( TCHAR* FileName, UINT &Index, UINT IndexMat );
	HRESULT CreatePhysXStaticTriangleMeshOfTXTFile( TCHAR* FileName, UINT IndexMat, UINT &IndexMesh );
	HRESULT CreatePhysXDinamicConvexMeshOfTXTFile( TCHAR* FileName, UINT IndexMat, UINT &IndexMesh );
	HRESULT AddMeshSceletAnimation( UINT IndexMesh, TCHAR* FileName, UINT &IndexAddAnim, bool fSetAnim );

private:
	CArray<CResMesh*> aMesh;
	CArray<CResSound*> aSound;
	CArray<CResourceTexture*> aTexture;
	CArray<CResMat*> aMaterial;
	CArray<CResourceAnimationScelet*> aSceletAnim;

	HRESULT GetTexture( TCHAR* Name, CResourceTexture* &Texture );
	HRESULT FindTexture( TCHAR* Name, CResourceTexture* &Texture );
	HRESULT FindMaterialByName( TCHAR* Name, UINT &Index );
	HRESULT FindMeshByName( TCHAR* Name, UINT &Index );
	HRESULT FindSceletAnimationByName( TCHAR* Name, UINT &Index );

	HRESULT FindPhysXStaticTriangleMeshByName( TCHAR* Name, UINT IndexMat, UINT &Index );
	HRESULT FindPhysXDinamicConvexMeshByName( TCHAR* Name, UINT IndexMat, UINT &Index );
};

