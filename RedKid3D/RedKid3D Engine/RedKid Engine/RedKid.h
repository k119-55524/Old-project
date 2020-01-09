#pragma once

#include "header.h"
#include "System.h"
#include "LogFile.h"
#include "DXInput.h"
#include "ResMesh.h"
#include "ResBase.h"
#include "ResSound.h"
#include "RenderPool.h"
#include "ObjectMesh.h"
#include "ObjectPivot.h"
#include "GameInterface.h"
#include "ManagerResource.h"
#include "ObjectLightPoint.h"
#include "ObjectLightDirection.h"
#include "ObjectPhysXSceletMesh.h"
#include "ObjectPhysXConvexDinamicMesh.h"
#include "ObjectPhysXTriangleStaticMesh.h"

class CRedKid
{
friend class CSystem;

public:
	CRedKid(void);
	~CRedKid(void);
	void Release( void );

	inline void SetCallbackInicialise( LPCALLBACKINICIALISE func )
	{
		pFuncInicialise = func;
		LogFile.OutputLogFileString( L"Set callback Inicialise function: OK." );
	};
	inline void SetCallbackPreFrameRender( LPCALLBACKPREFRAMERENDER func )
	{
		pFuncPreFrameRender = func;
		LogFile.OutputLogFileString( L"Set callback Inicialise function: OK." );
	};

	HRESULT InicialiseForEditor( HWND hWnd, PxDefaultAllocator &PhysXDefAlloc, PxErrorCallback &PhysXDefError );
	HRESULT Inicialise( HINSTANCE hInstance, PxDefaultAllocator &PhysXDefAlloc, PxErrorCallback &PhysXDefError, D3D_FEATURE_LEVEL cLev = D3D_FEATURE_LEVEL_11_0 );
	inline HRESULT RenderForEditor( void ) { return SystemRedKid.FrameRender(); }
	inline HRESULT ResizeForEditor( void ) { return SystemRedKid.Resize(); }
	int Run( void ) { return SystemRedKid.Run(); };
	inline void Exit( void ) { DestroyWindow( SystemRedKid.GetHWND() ); };
	inline void SetCameraPosition( float x, float y, float z ) {	ActivRenderPool->SetCameraPosition( x, y, z ); };
	inline void CameraStrafe( float x ) { ActivRenderPool->CameraStrafe( x ); };
	inline void CameraFly( float y ) { ActivRenderPool->CameraFly( y ); };
	inline void CameraWalk( float z ) {	ActivRenderPool->CameraWalk( z ); };

	HRESULT AddObjectMesh( CObjectBase* &Obj, TCHAR* Name, CObjectBase* ParentObj, UINT MeshIndex, bool fRenderSign = false );
	inline HRESULT CreateMatTexture( TCHAR* Name, UINT &Index, TCHAR* TextureFileName, bool fScelet = false )
	{
		if ( !SystemRedKid.GetfInit() )
			return S_FALSE;

		if ( Name == NULL )
		{
			LogFile.OutputLogFileErrorString( L"Name == NULL (CreateMatTexture).", __FILE__, __FUNCTION__, __LINE__  );
			return S_FALSE;
		}

		if ( TextureFileName == NULL )
		{
			LogFile.OutputLogFileErrorString( L"TextureFileName == NULL (CreateMatTexture).", __FILE__, __FUNCTION__, __LINE__  );
			return S_FALSE;
		}

		return cResManager.CreateMatTexture( Name, Index, fScelet, TextureFileName );
	};
	inline HRESULT AddPhysXSceletObject( TCHAR* Name, UINT &Index, TCHAR* TextureFileName, bool fScelet = false );
	inline HRESULT Create_MeshOfTXTFile( TCHAR* FileName, UINT &Index, UINT IndexMat ) { return cResManager.Create_MeshOfTXTFile( FileName, Index, IndexMat ); };

	HRESULT AddPointLight( TCHAR* Name, D3DXVECTOR3 &Pos, D3DXVECTOR4 &Color, float Radius, CObjectBase* ParentObj = NULL, bool fRenderSign = false );
	HRESULT AddDirectionLight( TCHAR* Name, CObjectBase* &Light, D3DXVECTOR4 &Color, bool fRenderSign = false, float SignScale = 0.0f );

	HRESULT AddInterfaceText( CGameInterface* &Text, UINT Line, TCHAR* TDFName, TCHAR* TextName );

	inline bool GetKeyStatus( UINT key ) { return g_cDInput.GetKeyStatus( key ); };
	inline bool GetMouseKeyStatus( UINT key ) {	return g_cDInput.GetMouseKeyStatus( key ); };
	inline void GetMousePos( LONG &x, LONG &y, LONG &z ) { g_cDInput.GetMousePos( x, y, z ); };

	inline void GetSceneStatistic( UINT &ro, UINT &rl, UINT &rt ) { ActivRenderPool->GetSceneStatistic( ro, rl, rt ); };

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// PhysX func
	HRESULT AddPhysXTriangleStaticMeshObject( CObjectBase* &Obj, TCHAR* Name, TCHAR* FileName, UINT IndexMat, CObjectBase* ParentObj = NULL, bool fRenderSign = false );
	HRESULT AddPhysXDinamicConvexMeshObject( CObjectBase* &Obj, TCHAR* Name, TCHAR* FileName, UINT IndexMat, CObjectBase* ParentObj = NULL, bool fRenderSign = false );
	HRESULT AddPhysXSceletObject( CObjectBase* &Obj, TCHAR* Name, TCHAR* FileName, TCHAR* FileAnimName, UINT IndexMat, CObjectBase* ParentObj = NULL, bool fRenderSign = false );
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

private:

	// ”казатель на внешнюю функцию вызываемую после инициализации основного приложени€
	LPCALLBACKINICIALISE		pFuncInicialise;
	LPCALLBACKPREFRAMERENDER	pFuncPreFrameRender;

	CDXInput g_cDInput;

	HRESULT FrameRender( float DeltaTime, float ElapsedTime );
	HRESULT Resize( UINT Width, UINT Height );
	void Acquire( void ) { g_cDInput.Acquire(); };
	void UnAcquire( void ) { g_cDInput.UnAcquire(); };

	CRenderPool* ActivRenderPool;
	CArray<CRenderPool*> aRenderPool;

	CManagerResource cResManager;
};
