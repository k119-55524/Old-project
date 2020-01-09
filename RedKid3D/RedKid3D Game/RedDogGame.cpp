#include "stdafx.h"
#include "RedDogGame.h"

int APIENTRY _tWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	RedKid.SetCallbackInicialise( OnInicialise );
	RedKid.SetCallbackPreFrameRender( OnPreFrameRender );
	V_RETURN( RedKid.Inicialise( hInstance, PhysXDefAlloc, PhysXDefError ) );

	return RedKid.Run();
}

HRESULT CALLBACK OnInicialise( void )
{
	RedKid.SetCameraPosition( 0.0f, 0.0f, -600.0f );


	UINT IndexMat = 0;
	UINT IndexMesh = 0;
	UINT IndexScAnim = 0;
	CObjectBase* Obj = NULL;

	//V_RETURN( RedKid.CreateMatTexture( _T("Terrain_000"), IndexMat, _T( "GameResource\\Textures\\001.jpg" ) ) );
	//V_RETURN( RedKid.AddPhysXTriangleStaticMeshObject( Obj, NULL, _T( "GameResource\\Mesh\\Terrain_001.RM3DT" ), IndexMat, NULL, true ) );
	//((CObjectPhysX*)Obj)->SetPhysXPosition( 20.0f, -200.0f, 0.0f );

	//V_RETURN( RedKid.CreateMatTexture( _T("Dinamic_000"), IndexMat, _T( "GameResource\\Textures\\Stones_TBN_Height.dds" ) ) );
	//V_RETURN( RedKid.AddPhysXDinamicConvexMeshObject( Obj, NULL, _T( "GameResource\\Mesh\\Sphere.RM3DT" ), IndexMat, NULL, true ) );

	V_RETURN( RedKid.CreateMatTexture( _T("Arab_000"), IndexMat, _T( "GameResource\\Textures\\Arab.jpg" ), true ) );
	V_RETURN( RedKid.AddPhysXSceletObject( Obj, NULL, _T( "GameResource\\Mesh\\Arab.RM3DT" ), _T( "GameResource\\Scelet animation\\Arab_001.rm3dt" ), IndexMat, NULL, true ) );

	//V_RETURN( RedKid.CreateMatTexture( _T("Tanker_000"), IndexMat, false, _T( "GameResource\\Textures\\Tanker_000.jpg" ) ) );
	//V_RETURN( RedKid.Create_MeshOfTXTFile( _T( "GameResource\\Mesh\\Tanker_000.rm3dt" ), IndexMesh, IndexMat ) );
	//V_RETURN( RedKid.AddObjectMesh( Obj, _T( "Object_Box" ), NULL, IndexMesh, true ) );
	//Obj->MovePosition( -170.0f, 85.0f, 170.0f );
	//ObjRot = Obj;

	//V_RETURN( RedKid.CreateMatTexture( _T("Arab_000"), IndexMat, _T( "GameResource\\Textures\\Arab.jpg" ), true ) );
	//V_RETURN( RedKid.Create_MeshOfTXTFile( _T( "GameResource\\Mesh\\Arab.rm3dt" ), IndexMesh, IndexMat ) );
	////V_RETURN( RedKid.AddMeshSceletAnimation( IndexMesh, _T( "GameResource\\Scelet animation\\Arab_001.rm3dt" ), IndexScAnim, true ) );
	//V_RETURN( RedKid.AddObjectMesh( Obj, _T( "Object_Arab" ), NULL, IndexMesh, true ) );
	//Obj->MovePosition( -100 , -100.0f, -100.0f );

	//V_RETURN( RedKid.CreateMatTexture( _T("Generator_000"), IndexMat, false, _T( "GameResource\\Textures\\Generator_000.jpg" ) ) );
	//V_RETURN( RedKid.Create_MeshOfTXTFile( _T( "GameResource\\Mesh\\Generator_000.rm3dt" ), IndexMesh, IndexMat ) );
	//V_RETURN( RedKid.AddObjectMesh( Obj, _T( "Generator_000" ), NULL, IndexMesh, true ) );
	//Obj->MovePosition( 150.0f , 1.0f, -100.0f );

	//V_RETURN( RedKid.AddPointLight( _T( "Point Light 000" ), D3DXVECTOR3( 0.0f, 150.0f, 50.0f ), D3DXVECTOR4( 1.0f, 0.0f, 0.0f, 1.0f ), 8000.0f, NULL, true ) );
	//V_RETURN( RedKid.AddPointLight( _T( "Point Light 001" ), D3DXVECTOR3(  -80.0f,  -70.0f, -90.0f ), D3DXVECTOR4( 0.0f, 1.0f, 0.0f, 1.0f ), 50.0f, NULL, true ) );
	//V_RETURN( RedKid.AddPointLight( _T( "Point Light 002" ), D3DXVECTOR3(  70.0f, -140.0f, -90.0f ), D3DXVECTOR4( 0.0f, 0.0f, 1.0f, 1.0f ), 60.0f, NULL, true ) );

	CObjectBase* Sun;
	V_RETURN( RedKid.AddDirectionLight( _T( "Direction Light 000" ), Sun, D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f ), true, 1000.0f ) );
	Sun->MoveRotation( (float)D3DX_PI / 4.0f, 3 * ( (float)D3DX_PI / 4.0f ), 0.0f );

	V_RETURN( RedKid.AddInterfaceText( cFPS, 30, _T( "GameResource\\Fonts\\Font_000.fntData" ), _T( "GameResource\\Fonts\\Font_000.tif" ) ) );
	cFPS->SetText( "Fps:00000." );

	V_RETURN( RedKid.AddInterfaceText( cInfoObj, 30, _T( "GameResource\\Fonts\\Font_000.fntData" ), _T( "GameResource\\Fonts\\Font_000.tif" ) ) );
	cInfoObj->SetTextLine( 1 );

	return S_OK;
}

void CALLBACK OnPreFrameRender( float deltaTime )
{
	static LONG x, y, z;
	static UINT FPS = SystemRedKid.GetFps();
	static UINT fps = FPS;
	static UINT mRO, mRL, mRT;

	fps = SystemRedKid.GetFps();
	static char fpsTxt[50];
	if ( FPS != fps )
	{
		FPS = fps;

		sprintf( fpsTxt, "FPS: %d.", FPS );
		cFPS->SetText( fpsTxt );
	}

	RedKid.GetSceneStatistic( mRO, mRL, mRT );
	sprintf( fpsTxt, "Obj:%d, Triangle:%d, Light:%d.", mRO, mRT, mRL );
	cInfoObj->SetText( fpsTxt );

	RedKid.GetMousePos( x, y, z );

	if ( z != 0 )
		RedKid.CameraWalk( (float)z / 2.0f );

	if ( RedKid.GetMouseKeyStatus( 2 ) )
	{
		RedKid.CameraStrafe( (float)-x );
		RedKid.CameraFly( (float)y );
	}

	if ( RedKid.GetKeyStatus( DIK_ESCAPE ) )
		RedKid.Exit();

	//ObjRot->SetRotation( 0.0f, 0.2f * deltaTime, 0.1f * deltaTime );
	//ObjRot->MovePosition( 0.1f, 0.025f, 0.025f );
}