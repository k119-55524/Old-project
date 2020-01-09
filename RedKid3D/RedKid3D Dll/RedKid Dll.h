// RedKid Dll.h

#pragma once

#pragma unmanaged

#include "header.h"

#include "PhysXErrorCallback.h"
#include "PhysXDefaultAllocator.h"

PhysXErrorCallback PhysXDefError;
PhysXDefaultAllocator PhysXDefAlloc;

#pragma managed

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Runtime::InteropServices;

using namespace System;

namespace RedKidDll {

	bool fInit = false; 

	extern "C" 
	{
		__declspec(dllexport) bool Inicialise( VOID* hWnd );
		__declspec(dllexport) void Release( void );
		__declspec(dllexport) bool Render( void );
		__declspec(dllexport) bool Resize( void );
		//__declspec(dllexport) bool AddSceneObjectToIndexPool( CSceneObject* &SceneObj, UINT Index, LPCTSTR Name );
		//__declspec(dllexport) bool GetObjectName( CSceneObject* SceneObj, LPCTSTR &Name );
		//__declspec(dllexport) bool AddRenderPool( LPCTSTR Name, UINT &index );
		//__declspec(dllexport) bool SetActiveRenderPool( UINT index );
		//__declspec(dllexport) bool RenameRenderPool( LPCTSTR Name, UINT index );
		//__declspec(dllexport) void RenameObject( CSceneObject* SceneObj, LPCTSTR Name );
		//__declspec(dllexport) bool AddBaseMaterial( UINT &IndexMat, LPCTSTR MatName, LPCTSTR TexName );
	}

	bool Inicialise( VOID* hWnd )
	{
		if ( fInit )
			return false;

		if ( hWnd == NULL )
			return false;

		if ( S_OK != RedKid.InicialiseForEditor( (HWND)hWnd, PhysXDefAlloc, PhysXDefError ) )
			return false;

		fInit = true;

		return true;
	}
	void Release( void )
	{
		if ( !fInit )
			return;

		RedKid.Release();
	}
	bool Render( void )
	{
		if ( !fInit )
			return false;

		if ( S_OK != RedKid.RenderForEditor() )
			return false;

		return true;
	}
	bool Resize( void )
	{
		if ( !fInit )
			return false;

		if ( S_OK != RedKid.ResizeForEditor() )
			return false;

		return true;
	}
}
