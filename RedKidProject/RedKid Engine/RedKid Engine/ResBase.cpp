#include "System.h"
#include "ResBase.h"

CResBase::CResBase( void )
{
	Name = NULL;
	IndexArray = -1;
	LinkCount = 0;

	g_pd3dDevice = SystemRedKid.GetDevice();
	g_pImmediateContext = SystemRedKid.GetDeviceContext();
}

CResBase::CResBase( TCHAR* name )
{
	CResBase();

	Name = name;
}

CResBase::~CResBase( void )
{
	Release();
}

void CResBase::Release( void )
{
	Name = NULL;
	IndexArray = -1;
	LinkCount = 0;
}