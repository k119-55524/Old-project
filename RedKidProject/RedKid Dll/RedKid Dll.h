// RedKid Dll.h

#pragma once

#pragma unmanaged

#include <RedKid.h>

#pragma managed

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Runtime::InteropServices;

using namespace System;

namespace RedKidDll {

	public ref class Class1
	{
		// TODO: здесь следует добавить свои методы для этого класса.
	};

	bool fInit = false; 

	extern "C" 
	{
		__declspec(dllexport) bool Inicialise( VOID* hWnd );
		//__declspec(dllexport) void Release( void );
		//__declspec(dllexport) void Render( void );
		//__declspec(dllexport) void Resize( void );
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

		return true;
	}
}
