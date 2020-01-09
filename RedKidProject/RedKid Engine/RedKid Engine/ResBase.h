#pragma once

#include "header.h"

class CResBase
{
public:
	CResBase( void );
	CResBase( TCHAR* name );
	~CResBase( void );
	void Release( void );

	inline eTypeResource GetType( void ) { return Type; };
	inline TCHAR* GetName( void ) { return Name; };
	inline void SetIndexArray( int Index ) { IndexArray = Index; };

	inline void IncrementLink() { LinkCount++; Condition = COND_DEL_NULL_LINK; };
	inline void DecrementLink() { if ( LinkCount > 0 ) LinkCount--; };

protected:
	TCHAR* Name;
	eTypeResource Type;
	eResourceCondition Condition;
	int IndexArray;

	ID3D11Device*				g_pd3dDevice;
	ID3D11DeviceContext*		g_pImmediateContext;

	UINT						LinkCount;
};
