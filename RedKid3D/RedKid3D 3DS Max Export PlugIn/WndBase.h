#pragma once

#include "header.h"

class CWndBase
{
public:
	CWndBase(void);
	~CWndBase(void);

	inline void Init( HWND h, int id )
	{
		m_DlgWnd = h;
		ID = id;
	}
	inline void Release( void ) {};
	inline void SetText( const TCHAR* txt ) { SetWindowText( GetDlgItem( m_DlgWnd, ID ), txt ); }
	inline void SetText( int c )
	{
		TCHAR t[256];

		sprintf( &t[0], "%d", c );

		SetText( t );
	}
	inline void SetText( UINT c )
	{
		TCHAR t[256];

		sprintf( &t[0], "%d", c );

		SetText( t );
	}
	inline void SetEnable( BOOL fVisible = TRUE ) {	EnableWindow( GetDlgItem( m_DlgWnd, ID ), fVisible ); }

protected:
	HWND m_DlgWnd;	// Родительское окно
	HWND m_HWnd;	// Элемент
	int ID;
};

