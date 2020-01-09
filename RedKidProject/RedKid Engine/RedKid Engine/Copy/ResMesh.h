pragma once

#include "AABB.h"
#include "header.h"
#include "System.h"
#include "ResMat.h"
#include "LogFile.h"
#include "ResBase.h"
#include "BufferSRV.h"
#include "BufferUAV.h"
#include "BufferConstant.h"
#include "ResourceAnimationScelet.h"

//class CResMesh :
//	public CResBase
//{
//	friend class CRenderPool;
//	friend class CObjectMesh;
//	friend class CManagerResource;
//	friend class CResourceSceletMesh;
//
//public:
//	CResMesh( void );
//	~CResMesh( void );
//
//	virtual bool GetScelet( void ) { return false; };
//	virtual UINT GetBoneCount( void ) { return 0; };
//	inline ID3D11Buffer* GetVertexBuffer( void ) { return g_pVertexBuffer; };
//	inline UINT GetVertexCount( void ) { return mVertexCount; };
//	inline UINT GetTriangleCount( void ) { return mTriangleCount; };
//	inline ID3D11Buffer* GetIndexBuffer( void ) { return g_pIndexBuffer; };
//	inline UINT GetIndexCount( void ) { return mIndexCount; };
//	void SetVertexBuffer( void )
//	{
//		static UINT offset = 0;
//		cMaterial->g_pTechnique->GetDesc( &techDesc );
//
//		System.GetDeviceContext()->IASetVertexBuffers( 0, 1, g_pRenderVertexBuffer, &StructSize, &offset );
//	};
//	inline void SetIndexBuffer( void ) { System.GetDeviceContext()->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0 ); };
//	inline CResMat* GetMaterial( void ) { return cMaterial; };
//	virtual HRESULT CreateMeshOfTXTFile( TCHAR* FileName, CMeshFormat* mForm );
//	void Render( void );
//	virtual HRESULT AddSceletAnimation( CResourceAnimationScelet* Anim, UINT &Index, bool fSetAnim ) { return S_FALSE; };
//	bool UpdateAnimation( float DeltaTime, D3DXMATRIX* m_GlobalWordMat ) { return false; };
//
//protected:
//	HRESULT LoadMeshBlock( std::ifstream &file, CMeshFormat* mForm, TCHAR* FileName );
//	void Release( void );
//
//	UINT mRealArraySize;
//	UINT mVertexCount;
//	UINT mTriangleCount;
//	UINT mIndexCount;
//	CMeshFormat cMeshFormat;
//
//	ID3D11Buffer**	g_pRenderVertexBuffer;	// ”казатель на реальный буффер дл€ отрисовки
//	ID3D11Buffer*	g_pVertexBuffer;
//	ID3D11Buffer*	g_pIndexBuffer;
//	CBufferSRV		g_pBufferSRV;
//	UINT			StructSize;
//
//	D3DX11_TECHNIQUE_DESC	techDesc;
//
//private:
//	virtual HRESULT SetMaterial( CResMat* mat )
//	{
//		#if defined( DEBUG ) || defined( _DEBUG )
//			if ( mat == NULL )
//			{
//				LogFile.OutputLogFileErrorString( _T( "mat == NULL." ), __FILE__, __FUNCTION__, __LINE__ );
//				return S_FALSE;
//			}
//		#endif
//
//		if ( cMeshFormat != *mat->GetMeshFormat() )
//		{
//			LogFile.OutputLogFileErrorString( _T( "Different formats mesh and format material." ), __FILE__, __FUNCTION__, __LINE__ );
//			return S_FALSE;
//		}
//
//		cMaterial = mat;
//		cMaterial->IncrementLink();
//
//		return S_OK;
//	}
//	virtual void CalcAABB( D3DXMATRIX* m_GlobalWordMat, CAABB &cAABB );
//	CResMat* cMaterial;
//	int IndexRenderMeshArray;
//};

class CResMesh :
	public CResBase
{
	friend class CRenderPool;
	friend class CObjectMesh;
	friend class CManagerResource;
	friend class CResourceSceletMesh;

public:
	CResMesh( void );
	virtual ~CResMesh( void );

	bool GetScelet( void ) { return false; };
	UINT GetBoneCount( void ) { return 0; };
	inline ID3D11Buffer* GetVertexBuffer( void ) { return g_pVertexBuffer; };
	inline UINT GetVertexCount( void ) { return mVertexCount; };
	inline UINT GetTriangleCount( void ) { return mTriangleCount; };
	inline ID3D11Buffer* GetIndexBuffer( void ) { return g_pIndexBuffer; };
	inline UINT GetIndexCount( void ) { return mIndexCount; };
	void SetVertexBuffer( void )
	{
		static UINT offset = 0;
		cMaterial->g_pTechnique->GetDesc( &techDesc );

		System.GetDeviceContext()->IASetVertexBuffers( 0, 1, g_pRenderVertexBuffer, &StructSize, &offset );
	};
	inline void SetIndexBuffer( void ) { System.GetDeviceContext()->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0 ); };
	inline CResMat* GetMaterial( void ) { return cMaterial; };
	HRESULT CreateMeshOfTXTFile( TCHAR* FileName, CMeshFormat* mForm );
	void Render( void );
	HRESULT AddSceletAnimation( CResourceAnimationScelet* Anim, UINT &Index, bool fSetAnim ) { return S_FALSE; };
	bool UpdateAnimation( float DeltaTime, D3DXMATRIX* m_GlobalWordMat ) { return false; };

protected:
	HRESULT LoadMeshBlock( std::ifstream &file, CMeshFormat* mForm, TCHAR* FileName );
	void Release( void );

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

private:
	HRESULT SetMaterial( CResMat* mat )
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
	virtual void CalcAABB( D3DXMATRIX* m_GlobalWordMat, CAABB &cAABB );
	CResMat* cMaterial;
	int IndexRenderMeshArray;
};