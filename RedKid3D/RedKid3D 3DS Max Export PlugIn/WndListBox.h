#pragma once

#include "WndBase.h"

class CWndListBox :
	public CWndBase
{
public:
	CWndListBox(void);
	~CWndListBox(void);
	void Release( void );

	inline DWORD AddLine( TCHAR* s, LPARAM data = NULL )
	{
		DWORD index = -1;
		index = (DWORD)SendDlgItemMessage( m_DlgWnd, ID, LB_ADDSTRING , 0, (LPARAM)(TCHAR*)s );
		SendDlgItemMessage( m_DlgWnd, ID, LB_SETITEMDATA , (WPARAM)(int)index, (LPARAM)data );

		return index;
	}
	// Количество строк
	inline UINT GetCount( void ) { return (UINT)SendDlgItemMessage( m_DlgWnd, ID, LB_GETCOUNT , 0L, 0L ); };
	inline UINT GetSelectCount( void )
	{
		UINT sel = 0;

		int i = (int)SendDlgItemMessage( m_DlgWnd, ID, LB_GETSELCOUNT , 0L, 0L );
		if ( i != LB_ERR )
			sel = (UINT)i;

		return sel;
	}
	inline void GetSelectItems( int* Arr, int Size ) { SendDlgItemMessage( m_DlgWnd, ID, LB_GETSELITEMS, (WPARAM)Size, (LPARAM)Arr ); }
	// Индекс выбранной строки
	inline int GetCurSel( void ) { return (int)SendDlgItemMessage( m_DlgWnd, ID, LB_GETCURSEL, 0L, 0L ); };
	inline LPVOID GetItemData( int Index )
	{
		LPVOID Data = NULL;
		Data = (LPVOID)SendDlgItemMessage( m_DlgWnd, ID, LB_GETITEMDATA , (WPARAM)Index, 0L );

		if ( (int)Data == LB_ERR )
			return NULL;
		else
			return Data;
	}
	inline int GetItemText( int Index, TCHAR* Buff ) {	return (int)SendDlgItemMessage( m_DlgWnd, ID, LB_GETTEXT, Index, (LPARAM)Buff );	};
	inline void SelectAll( void ) { SendDlgItemMessage( m_DlgWnd, ID, LB_SELITEMRANGEEX, 0L, GetCount() ); }

	inline void DeleteLine( UINT Index ) { SendDlgItemMessage( m_DlgWnd, ID, LB_DELETESTRING, Index, 0L ); };
	inline void DeleteItemData( UINT Index )
	{
		LPVOID Data = NULL;

		Data = GetItemData( Index );

		if ( Data != NULL )
			delete Data;
	}
	void DeleteListLine( int* Index, UINT Count, BOOL fDD );
};

