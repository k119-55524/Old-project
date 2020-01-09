#include "DXInput.h"

CDXInput::CDXInput( void )
{
	g_pDinput	= NULL;
	g_pKeyboard	= NULL;
	g_pMouse	= NULL;

	ZeroMemory( &mouse, sizeof( MouseState ) );
}

CDXInput::~CDXInput(void)
{
	Release();
}

void CDXInput::Release( void )
{
	if ( g_pKeyboard )
		g_pKeyboard->Unacquire();

	if ( g_pMouse )
		g_pMouse->Unacquire();

	RELEASE( g_pMouse );
	RELEASE( g_pKeyboard );
	RELEASE( g_pDinput );
}

HRESULT CDXInput::Inicialise( HWND hWnd, HINSTANCE m_hInstance )
{
	LogFile.OutputLogFileString( L"DirectX Input create: START." );

	V_RETURN( DirectInput8Create( m_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pDinput, NULL ) ); 

	V_RETURN( g_pDinput->CreateDevice( GUID_SysKeyboardEm2, &g_pKeyboard, NULL ) );
	V_RETURN( g_pKeyboard->SetDataFormat( &c_dfDIKeyboard ) );
	V_RETURN( g_pKeyboard->SetCooperativeLevel( hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY ) );

	DIOBJECTDATAFORMAT g_aObjectFormats[] =	{
		{ &GUID_XAxis, FIELD_OFFSET( MouseState, AxisX ),    // X axis
			DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
		{ &GUID_YAxis, FIELD_OFFSET( MouseState, AxisY ),    // Y axis
			DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
		{ &GUID_ZAxis, FIELD_OFFSET( MouseState, AxisZ ),    // Z axis(колёсико)
			DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
		{ 0, FIELD_OFFSET( MouseState, Buttons[0] ),        // Button 0
			DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
		{ 0, FIELD_OFFSET( MouseState, Buttons[1] ),        // Button 1 (optional)
			DIDFT_BUTTON | DIDFT_ANYINSTANCE | DIDFT_OPTIONAL, 0 },
		{ 0, FIELD_OFFSET( MouseState, Buttons[2] ),        // Button 2 (optional)
			DIDFT_BUTTON | DIDFT_ANYINSTANCE | DIDFT_OPTIONAL, 0 } };
	UINT numMouseObjects  = ( sizeof( g_aObjectFormats ) / sizeof( DIOBJECTDATAFORMAT ) );

	DIDATAFORMAT g_dfMouse = 
	{
	    sizeof( DIDATAFORMAT ),
	    sizeof( DIOBJECTDATAFORMAT ),
		DIDF_RELAXIS, 
	    //DIDF_ABSAXIS,
	    sizeof( MouseState ),
	    numMouseObjects,
	    g_aObjectFormats
	};

	V_RETURN( g_pDinput->CreateDevice( GUID_SysMouseEm2, &g_pMouse, NULL ) );
	V_RETURN( g_pMouse->SetDataFormat( &g_dfMouse ) );
	V_RETURN( g_pKeyboard->SetCooperativeLevel( hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE ) );

	LogFile.OutputLogFileString( L"DirectX Input create: OK." );

	return S_OK;
}
