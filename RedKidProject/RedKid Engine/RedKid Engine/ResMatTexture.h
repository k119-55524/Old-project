#pragma once

#include "ResMat.h"
#include "ResourceTexture.h"

class CResMatTexture :
	public CResMat
{
	friend class CRenderPool;

public:
	CResMatTexture( void );
	~CResMatTexture( void );
	void Release( void );

	void Inicialise( CResourceTexture* &Text, bool fScelet );
	void SetMaterialIndividual( void );

protected:
	CResourceTexture* DiffuseTexture;
};

