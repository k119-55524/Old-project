#pragma once

#include "header.h"
#include "CArray.h"
#include "Camera.h"
#include "Thread.h"
#include "ResMat.h"
#include "SignMesh.h"
#include "FrustumBox.h"
#include "ObjectMesh.h"
#include "ObjectPivot.h"
#include "ObjectPhysX.h"
#include "GameInterface.h"
#include "ObjectLightPoint.h"
#include "ArrayBufferStruct.h"
#include "ObjectLightDirection.h"

class CRenderPool
{
	friend class CRenderPool;

	class CObjList
	{
	public:
		CArrayStack<CObjectBase*> Obj;
	};
	class CMaterialConcrete
	{
	public:
		~CMaterialConcrete( void )
		{
			ObjList.ReleaseLP();
		};

		CArray<CObjList*> ObjList;
	};
	class CMaterialList
	{
	public:
		~CMaterialList( void )
		{
			ConcreteMaterialList.ReleaseLP();
		};

		CArray<CMaterialConcrete*> ConcreteMaterialList;
		CResMat* Material;
	};

public:
	CRenderPool(void);
	~CRenderPool(void);
	void Release( void );

	HRESULT Inicialise( void );

	inline PxScene* GetPhysXScene( void ) { return PhysXScene; };

	D3DXVECTOR4* GetClearColor( void ) { return &ClearColor; };
	void Render( float DTime, float ElapsedTime );
	void RenderInterface( void );
	HRESULT Resize( UINT Width, UINT Height );

	inline HRESULT AddObject( CObjectBase* &Obj, CObjectBase* ParentObj )
	{
		if ( ParentObj == NULL )
			Scene.AddChild( Obj );
		else
			ParentObj->AddChild( Obj );

		V_RETURN( Frustum.AddObject( Obj ) );

		return S_OK;
	}
	inline HRESULT AddInterface( CGameInterface* interf )
	{
		#if ( defined( DEBUG ) || defined( _DEBUG ) )
			if ( interf == NULL )
			{
				LogFile.OutputLogFileErrorString( _T( "Error add in InterfaceList." ), __FILE__, __FUNCTION__, __LINE__ );
				return S_FALSE;
			}
		#endif

		return Interface.Add( interf );
	};
	inline HRESULT AddDirectionLight( CObjectLightDirection* Light ) { return m_lLightDirection.Add( Light ); };

	inline void SetCameraPosition( float x, float y, float z ) { Camera.SetPosition( x, y, z ); };
	inline void CameraStrafe( float x ) { Camera.Strafe( x ); };
	inline void CameraFly( float y ) { Camera.Fly( y ); };
	inline void CameraWalk( float z ) {	Camera.Walk( z ); };

	inline void GetSceneStatistic( UINT &ro, UINT &rl, UINT &rt ) { ro = RendObjCount; rl = RendLightCount; rt = RendTrianglCount; };

protected:
	ID3D11Device*			g_pd3dDevice;
	ID3D11DeviceContext*	g_pImmediateContext;

	D3DXVECTOR4 ClearColor;
	CObjectPivot Scene;
	CList<CObjectLightDirection*> m_lLightDirection;
	CArray<CMaterialList*> SceneSortForRendring;
	CArrayStack<CSignMesh*> SignMesh;

	void FindRenderObject( CFrustumBox* Frustum );
	void AddAllRenderObject( CFrustumBox* Frustum );
	void AddOblectForRendring( CObjectBase* Obj );
	void RenderSortList( void );
	void FrameCalcPhysics( void );

	CCamera Camera;
	CFrustumBox Frustum;
	CList<CGameInterface*> Interface;

	CArrayBufferStruct<LIGHT_POINT> aLightPoint;
	CArrayBufferStruct<LIGHT_DIRECTION> aLightDirection;

	UINT RendObjCount, RendLightCount, RendTrianglCount;

	D3DXMATRIX				g_View;
	D3DXMATRIX				g_Projection;
	D3DXMATRIX				g_ViewProj;
	float					zFar, zNear;
	bool					fInit;

	float DeltaTime;
	DWORD BuildRenderData( void );

	// PhysX
	PxScene* PhysXScene;
	pxtask::CudaContextManager*	mCudaContextManager;
	PxDefaultCpuDispatcher*		mCpuDispatcher;
};

