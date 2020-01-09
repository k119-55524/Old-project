#include "ObjectPhysX.h"

CObjectPhysX::CObjectPhysX( TCHAR* name )
	: CObjectMesh( name )
{
	PhysXActor	= NULL;
	PhysXScene	= NULL;
}

CObjectPhysX::~CObjectPhysX( void )
{
	Release();
}

void CObjectPhysX::Release( void )
{
	rELEASE( PhysXActor );
	PhysXScene	= NULL;
}