#include "WndListBox.h"

CWndListBox::CWndListBox(void)
	: CWndBase()
{
}

CWndListBox::~CWndListBox(void)
{
	Release();
}

void CWndListBox::Release( void )
{
	LPARAM data = NULL;
	UINT Count = GetCount();

	for ( UINT i = 0; i < Count; i++ )
	{
		data = (LRESULT)(DWORD)SendDlgItemMessage( m_DlgWnd, ID, LB_GETITEMDATA , (WPARAM)(int)i, 0L );
		if ( data != NULL )
			delete (LPVOID)data;
	}

	SendDlgItemMessage( m_DlgWnd, ID, LB_RESETCONTENT, 0, NULL );

	Count = 0;
}

void CWndListBox::DeleteListLine( int* Index, UINT Count, BOOL fDD = TRUE )
{
	// Sort
	int r;
	if ( Count > 1 )
		for ( UINT i1 = 0; i1 < Count - 1; i1++ )
			for ( UINT i2 = i1 + 1; i2 < Count; i2++ )
				if ( Index[i1] > Index[i2] )
				{
					r = Index[i1];
					Index[i1] = Index[i2];
					Index[i2] = r;
				}

	if ( fDD )
		DeleteItemData( Index[0] );
	DeleteLine( Index[0] );
	for ( UINT i = 1; i < Count; i++ )
	{
		if ( fDD )
			DeleteItemData( Index[i] - i );

		DeleteLine( Index[i] - i );
	}
}