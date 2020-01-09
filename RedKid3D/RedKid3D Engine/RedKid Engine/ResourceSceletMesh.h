#pragma once

#include "header.h"
#include "AABB.h"
#include "Scelet.h"
#include "ResMesh.h"
#include "BufferUAV.h"
#include "BufferSRV.h"
#include "BufferConstant.h"
#include "ResourceAnimationScelet.h"

class CResourceSceletMesh :
	public CResMesh
{
public:
	CResourceSceletMesh(void);
	~CResourceSceletMesh(void);

	bool GetScelet( void ) { return true; };
	UINT GetBoneCount( void ) { return Scelet.GetBoneCount(); };
	HRESULT CreateMeshOfTXTFile( TCHAR* FileName, CMeshFormat* mForm );
	HRESULT AddSceletAnimation( CResourceAnimationScelet* Anim, UINT &IndexAdd, bool fSetAnim );
	bool UpdateAnimation( float DeltaTime, D3DXMATRIX* m_GlobalWordMat );
	HRESULT SetSceletAnimation( UINT Index, float dt );

protected:
	void Release( void );
	CScelet Scelet;

private:
	HRESULT SetMaterial( CResMat* mat );
	HRESULT FindAnimationByName( TCHAR* Name, UINT &Index );

	CAABB cAABBScelet;
	void CalcAABB( D3DXMATRIX* m_GlobalWordMat, CAABB &cAABB )
	{
		if ( *cAABBScelet.GetMin() == *cAABBScelet.GetMax() )
			UpdateAnimation( 0.0f, m_GlobalWordMat );

		cAABB = cAABBScelet;
	};

	CBufferConstant			c_CBMatr;
	CBufferSRV				c_SRVIn;
	CBufferUAV				c_UAVOut;
	ID3D11ComputeShader*	g_pCSCopy;
	ID3D11ComputeShader*	g_pCSAnim;

	CArray<CResourceAnimationScelet*> aSceletAnimation;
	CResourceAnimationScelet* ActivAnimation;
	void UpdateScelet( float dt );
	float mDTimeScAnim;
};

