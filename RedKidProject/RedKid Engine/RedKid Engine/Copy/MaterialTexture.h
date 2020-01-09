#pragma once

#include "header.h"
#include "ResMat.h"
#include "ResBase.h"
#include "ResourceTexture.h"

class CMaterialTexture :
	public CResMat
{
public:
	CMaterialTexture( void );
	~CMaterialTexture( void );

	void SetMaterialIndividual( void );
	void Inicialise( CResourceTexture* &Text, bool fScelet );

	int GetIndexRenderList( void ) { return IndexRenderMaterialList; };
	void SetIndexRenderList( int i ) { IndexRenderMaterialList = i; };

protected:
	CResourceTexture* DiffuseTexture;
	void Release( void );

	int	IndexRenderMaterialList;
};

