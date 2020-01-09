#pragma once

#include "ResBase.h"

class CResMat :
	public CResBase
{
	friend class CResMesh;
	friend class CRenderPool;

public:
	CResMat( void );
	virtual ~CResMat( void );
	void Release( void );

	void SetName( TCHAR* name ) { Name = name; };
	inline CMeshFormat* GetMeshFormat( void ) { return &cMeshFormat; };
	inline int GetIndexConcrRenderList( void ) { return IndexRenderMaterialConcrete; };
	inline void SetIndexConcrRenderList( int i ) { IndexRenderMaterialConcrete = i; };
	inline int GetIndexRenderList( void ) { return IndexRenderMaterialList; };
	inline void SetIndexRenderList( int i ) { IndexRenderMaterialList = i; };

	inline void SetMaterial( void ) { g_pImmediateContext->IASetInputLayout( g_pInputLayout ); };
	virtual void SetMaterialIndividual( void ) = 0;

protected:
	eTypeMaterial			TypeMat;
	CMeshFormat				cMeshFormat;
	ID3DX11EffectTechnique*	g_pTechnique;
	ID3D11InputLayout*		g_pInputLayout;

	int						IndexRenderMaterialConcrete;
	int						IndexRenderMaterialList;
};

