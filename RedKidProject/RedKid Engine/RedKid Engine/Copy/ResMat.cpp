#include "ResMat.h"

CResMat::CResMat( void )
{
	Type						= RES_MATERIAL;
	g_pInputLayout				= NULL;
	g_pTechnique				= NULL;
	IndexRenderMaterialConcrete = -1;
}

CResMat::~CResMat( void )
{
}

