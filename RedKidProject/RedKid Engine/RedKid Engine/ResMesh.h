#pragma once

#include "AABB.h"
#include "ResMat.h"
#include "ResBase.h"
#include "BufferSRV.h"
#include "ResourceAnimationScelet.h"

class CResMesh :
	public CResBase
{
	friend class CRenderPool;

public:
	CResMesh( void );
	virtual ~CResMesh( void );
	void Release( void );

	inline ID3D11Buffer* GetVertexBuffer( void ) { return g_pVertexBuffer; };
	inline UINT GetVertexCount( void ) { return mVertexCount; };
	inline UINT GetTriangleCount( void ) { return mTriangleCount; };
	inline ID3D11Buffer* GetIndexBuffer( void ) { return g_pIndexBuffer; };
	inline UINT GetIndexCount( void ) { return mIndexCount; };

	virtual HRESULT SetMaterial( CResMat* mat )
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

		cMaterial = mat;
		cMaterial->IncrementLink();

		return S_OK;
	}
	inline void SetVertexBuffer( void )
	{
		static UINT offset = 0;
		cMaterial->g_pTechnique->GetDesc( &techDesc );

		SystemRedKid.GetDeviceContext()->IASetVertexBuffers( 0, 1, g_pRenderVertexBuffer, &StructSize, &offset );
	};
	inline void SetIndexBuffer( void ) { g_pImmediateContext->IASetIndexBuffer( g_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0 ); };
	inline void CResMesh::Render( void )
	{
		for( UINT p = 0; p < techDesc.Passes; ++p )
		{
			cMaterial->g_pTechnique->GetPassByIndex( p )->Apply( 0, SystemRedKid.GetDeviceContext() );
			SystemRedKid.GetDeviceContext()->DrawIndexed( mIndexCount, 0, 0 );
		}
	}

	inline CResMat* GetMaterial( void ) { return cMaterial; };
	virtual HRESULT CreateMeshOfTXTFile( TCHAR* FileName, CMeshFormat* mForm );

	virtual void CalcAABB( D3DXMATRIX* m_GlobalWordMat, CAABB &cAABB );
	virtual bool GetScelet( void ) { return false; };
	virtual UINT GetBoneCount( void ) { return 0; };
	virtual HRESULT AddSceletAnimation( CResourceAnimationScelet* Anim, UINT &Index, bool fSetAnim ) { return S_FALSE; };

protected:
	virtual HRESULT LoadMeshBlock( std::ifstream &file, CMeshFormat* mForm, TCHAR* FileName );
	CResMat* cMaterial;

	UINT mRealArraySize;
	UINT mVertexCount;
	UINT mTriangleCount;
	UINT mIndexCount;
	CMeshFormat cMeshFormat;

	ID3D11Buffer**	g_pRenderVertexBuffer;	// ”казатель на реальный буффер дл€ отрисовки
	ID3D11Buffer*	g_pVertexBuffer;
	ID3D11Buffer*	g_pIndexBuffer;
	CBufferSRV		g_pBufferSRV;
	UINT			StructSize;

	D3DX11_TECHNIQUE_DESC	techDesc;

	int IndexRenderMeshArray;
};

