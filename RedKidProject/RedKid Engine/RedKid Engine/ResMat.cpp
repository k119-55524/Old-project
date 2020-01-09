#include "ResMat.h"

CResMat::CResMat( void )
	: CResBase()
{
	IndexRenderMaterialList = -1;
	IndexRenderMaterialConcrete = -1;
}

CResMat::~CResMat( void )
{
	Release();
}

void CResMat::Release( void )
{
	IndexRenderMaterialList = -1;
	IndexRenderMaterialConcrete = -1;
}