#pragma once

#include "objectbase.h"

class CObjectLightDirection :
	public CObjectBase
{
	friend class CRedKid;

public:
	CObjectLightDirection( TCHAR* Name, D3DXVECTOR4* Col, float SignScale );
	~CObjectLightDirection( void );

	inline D3DXVECTOR4* GetColor( void ) { return &Color; };
	inline D3DXVECTOR4* GetDirection( void )
	{
		if ( fCalcAABB )
			CObjectBase::CalcAABB();

		static D3DXVECTOR3 Dir;
		Dir = D3DXVECTOR3( 0.0f, 0.0f, -1.0f );
		D3DXVec3TransformCoord( &Dir, &Dir, GetGlobalWordMat() );
		Direction.x = Dir.x;
		Direction.y = Dir.y;
		Direction.z = Dir.z;

		return &Direction;
	};

protected:
	void Release( void  );

	inline void UpdateObjAnimation( float DeltaTime ) {};
	inline void CalcAABB( D3DXMATRIX* m_GlobalWordMat )
	{
		#if ( defined( DEBUG ) || defined( _DEBUG ) )
			if ( fRenderSign )
				V_( cSignMesh.CreateDirectoinLightMesh( GetGlobalWordMat(), mSignScale ) );
		#endif
	};

	D3DXVECTOR4 Color;
	D3DXVECTOR4 Direction;
	float mSignScale;
};

