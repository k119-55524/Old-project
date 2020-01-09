#include "MaterialTexture.h"

CMaterialTexture::CMaterialTexture( void )
{
	TypeMat = MAT_TEXTURE;
	IndexRenderMaterialList = -1;

	// Устанавливаем формат необходимой сетки
	cMeshFormat.SetfNormal();
	cMeshFormat.SetfUV();

	DiffuseTexture = NULL;
}

CMaterialTexture::~CMaterialTexture( void )
{
	Release();
}

void CMaterialTexture::SetMaterialIndividual( void )
{
	System.GetDiffuseTexture()->SetResource( DiffuseTexture->GetSRV() );
}

void CMaterialTexture::Release( void )
{
	g_pInputLayout	= NULL;
	g_pTechnique	= NULL;

	if ( DiffuseTexture != NULL )
	{
		DiffuseTexture->DecrementLink();
		DiffuseTexture = NULL;
	}
}

void CMaterialTexture::Inicialise( CResourceTexture* &Text, bool fScelet )
{
	Release();

	cMeshFormat.SetfSceletData( fScelet );

	DiffuseTexture = Text;
	DiffuseTexture->IncrementLink();

	if ( fScelet )
	{
		g_pInputLayout	= System.GetLayoutTextureBone();
		g_pTechnique	= System.GetTechTextureBone();
	}
	else
	{
		g_pInputLayout	= System.GetLayoutTexture();
		g_pTechnique	= System.GetTechTexture();
	}
}