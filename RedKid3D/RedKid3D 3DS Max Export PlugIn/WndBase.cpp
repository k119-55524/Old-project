#include "header.h"
#include "WndBase.h"

CWndBase::CWndBase(void)
{
	m_DlgWnd	= NULL;
	m_HWnd		= NULL;
	ID			= -1;
}

CWndBase::~CWndBase(void)
{
	Release();
}
