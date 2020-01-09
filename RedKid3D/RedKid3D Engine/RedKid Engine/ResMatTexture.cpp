#include "System.h"
#include "ResMatTexture.h"

CResMatTexture::CResMatTexture( void )
	: CResMat()
{
	TypeMat = MAT_TEXTURE;

	// Устанавливаем формат необходимой сетки
	cMeshFormat.SetfNormal();
	cMeshFormat.SetfUV();

	DiffuseTexture = NULL;
}

CResMatTexture::~CResMatTexture( void )
{
	Release();
}

void CResMatTexture::Release( void )
{
	g_pInputLayout	= NULL;
	g_pTechnique	= NULL;

	if ( DiffuseTexture != NULL )
	{
		DiffuseTexture->DecrementLink();
		DiffuseTexture = NULL;
	}
}

void CResMatTexture::Inicialise( CResourceTexture* &Text, bool fScelet )
{
	Release();

	cMeshFormat.SetfSceletData( fScelet );

	DiffuseTexture = Text;
	DiffuseTexture->IncrementLink();

	if ( fScelet )
	{
		g_pInputLayout	= SystemRedKid.GetLayoutTextureBone();
		g_pTechnique	= SystemRedKid.GetTechTextureBone();
	}
	else
	{
		g_pInputLayout	= SystemRedKid.GetLayoutTexture();
		g_pTechnique	= SystemRedKid.GetTechTexture();
	}
}

void CResMatTexture::SetMaterialIndividual( void )
{
	SystemRedKid.GetDiffuseTexture()->SetResource( DiffuseTexture->GetSRV() );
}