#pragma once

#include "header.h"
#include "LogFile.h"
#include "System.h"

class CDXInput
{
	// Макрос для проверки нажатий клавиш клавиатуры
	#define KEYDOWN(name, key) (name[key] & 0x80)

	struct MouseState
	{
		LONG AxisX;
		LONG AxisY;
		LONG AxisZ;
		BYTE Buttons[3];
		BYTE bPadding;
	};

public:
	CDXInput(void);
	~CDXInput(void);
	void Release( void );

	HRESULT Inicialise( HWND hWnd, HINSTANCE m_hInstance );
	inline void Acquire(void )
	{
		if( g_pKeyboard )
			g_pKeyboard->Acquire();

		if( g_pMouse )
			g_pMouse->Acquire();
	};
	inline void UnAcquire(void )
	{
		if( g_pKeyboard )
			g_pKeyboard->Unacquire();

		if( g_pMouse )
			g_pMouse->Unacquire();
	};

	inline bool GetKeyStatus( UINT key )
		{
			if ( KEYDOWN ( keyboard, key ) )
				return true;
			else
				return false;
		};
	inline bool GetMouseKeyStatus( UINT key )
	{
		#if defined( DEBUG ) || defined( _DEBUG )
			if ( key > 2 )
				return false;
		#endif

		if( KEYDOWN( mouse.Buttons, key ) )
			return true;

		return false;
	}
	inline void GetMousePos( LONG &x, LONG &y, LONG &z )
	{
		x = mouse.AxisX;
		y = mouse.AxisY;
		z = mouse.AxisZ;
	}
	inline void FixedInputStatus(void)
	{
		// Keyboard
		if( g_pKeyboard ) 
			g_pKeyboard->GetDeviceState( sizeof( keyboard ), (LPVOID)&keyboard);

		// Mouse
		if( DI_OK != g_pMouse->Poll() )
			g_pMouse->Acquire();

		g_pMouse->GetDeviceState( sizeof( MouseState ), &mouse );
	};

protected:
	LPDIRECTINPUT8			g_pDinput;			// Указатель интерфейса DirectInput
	LPDIRECTINPUTDEVICE8	g_pKeyboard;		// Указатель на устройство ( в данном случае клавиатура )
	LPDIRECTINPUTDEVICE8	g_pMouse;			// Указатель на устройство ( в данном случае мышь )

	char 					keyboard[256];		// Буффер для опроса клавиатуры
	MouseState				mouse;
};

