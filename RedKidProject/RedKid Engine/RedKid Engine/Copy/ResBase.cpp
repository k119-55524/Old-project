#include "System.h"
#include "ResBase.h"

CResBase::CResBase( void )
{
	Name = NULL;
	LinkCount = 0;
	IndexArray = -1;
	Condition = COND_CREATE;

	g_pImmediateContext = System.GetDeviceContext();
}

CResBase::~CResBase( void )
{

}
