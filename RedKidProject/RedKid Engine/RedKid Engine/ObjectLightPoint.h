#pragma once

#include "ObjectBase.h"

class CObjectLightPoint :
	public CObjectBase
{
public:
	CObjectLightPoint( TCHAR* Name, D3DXVECTOR3* Pos, D3DXVECTOR4* Col, float rad );
	~CObjectLightPoint( void );
	void Release( void );

	inline float GetRadius( void ) { return Radius; };
	inline D3DXVECTOR4* GetColor( void ) { return &Color; };

protected:
	void UpdateObjAnimation( float DeltaTime );
	void CalcAABB( D3DXMATRIX* m_GlobalWordMat );

	D3DXVECTOR4 Color;
	float Radius;
};

