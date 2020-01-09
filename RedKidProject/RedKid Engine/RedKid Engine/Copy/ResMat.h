#pragma once

#include "header.h"
#include "System.h"
#include "ResBase.h"

// Тип ресурса
enum eTypeMaterial : unsigned int
{
	MAT_TEXTURE = 0,
};

class CResMat :
	public CResBase
{
	friend class CResMesh;

public:
	CResMat( void );
	virtual ~CResMat( void );

	void SetName( TCHAR* name ) { Name = name; };

	eTypeMaterial GetMaterialType( void ) { return TypeMat; };
	inline void SetMaterial( void )
	{
		System.GetDeviceContext()->IASetInputLayout( g_pInputLayout );
	};
	virtual void Release( void ) = 0;
	virtual int GetIndexRenderList( void ) = 0;
	virtual void SetIndexRenderList( int i ) = 0;
	virtual void SetMaterialIndividual( void ) = 0;
	int GetIndexConcrRenderList( void ) { return IndexRenderMaterialConcrete; };
	void SetIndexConcrRenderList( int i ) { IndexRenderMaterialConcrete = i; };
	inline CMeshFormat* GetMeshFormat( void ) { return &cMeshFormat; };


protected:
	eTypeMaterial			TypeMat;
	CMeshFormat			cMeshFormat;
	ID3DX11EffectTechnique*	g_pTechnique;
	ID3D11InputLayout*		g_pInputLayout;

	int						IndexRenderMaterialConcrete;
};
