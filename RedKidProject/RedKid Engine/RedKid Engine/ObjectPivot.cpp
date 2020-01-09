#include "ObjectPivot.h"

CObjectPivot::CObjectPivot( TCHAR* Name )
	: CObjectBase( Name )
{
}

CObjectPivot::~CObjectPivot( void )
{
	Release();
	CObjectBase::~CObjectBase();
}

void CObjectPivot::Release( void )
{

}
