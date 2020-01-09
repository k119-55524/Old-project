#pragma once

#include "header.h"
#include "wndbase.h"

class CWndCheckBox :
	public CWndBase
{
public:
	CWndCheckBox(void);
	~CWndCheckBox(void);

	inline void SetCheck( BOOL fCheck )
	{
		if ( fCheck )
			SendDlgItemMessage( m_DlgWnd, ID, BM_SETCHECK, BST_CHECKED, 0L ); 
		else
			SendDlgItemMessage( m_DlgWnd, ID, BM_SETCHECK, BST_UNCHECKED, 0L );
	}
	inline void SetCheck( BOOL fCheck, BOOL fVisible )
	{
		if ( fCheck )
			SendDlgItemMessage( m_DlgWnd, ID, BM_SETCHECK, BST_CHECKED, 0L ); 
		else
			SendDlgItemMessage( m_DlgWnd, ID, BM_SETCHECK, BST_UNCHECKED, 0L );

		if ( fVisible )
			SendDlgItemMessage( m_DlgWnd, ID, WM_ENABLE, TRUE, 0L ); 
		else
			SendDlgItemMessage( m_DlgWnd, ID, WM_ENABLE, FALSE, 0L );
	}
	inline BOOL GetCheck( void ) { return (int)SendDlgItemMessage( m_DlgWnd, ID, BM_GETCHECK, 0L, 0L ); }
};

