#pragma once

#include "header.h"
#include "SignMesh.h"

class COBB
{
	friend class CObjectPhysXSceletMesh;

public:
	COBB( void );
	~COBB( void );
	void Release( void );

	inline float GetXSize( void ) { return abs( Box[7].x - Box[0].x ); };
	inline float GetYSize( void ) { return abs( Box[7].y - Box[0].y ); };
	inline float GetZSize( void ) { return abs( Box[7].z - Box[0].z ); };

	D3DXVECTOR3* GetMin( void ) { return &Box[0]; };
	D3DXVECTOR3* GetMax( void ) { return &Box[7]; };
	void SetMin( D3DXVECTOR3* p ) { Box[0] = *p; };
	void SetMax( D3DXVECTOR3* p ) { Box[7] = *p; };

	inline void CheckMinMaxPoint( D3DXVECTOR3 &p )
	{
		if ( fMMStart )
		{
			Box[0] = Box[7] = p;
			fMMStart = false;
		}
		else
		{
			if ( Box[0].x > p.x )
				Box[0].x = p.x;

			if ( Box[0].y > p.y )
				Box[0].y = p.y;

			if ( Box[0].z > p.z )
				Box[0].z = p.z;

			if ( Box[7].x < p.x )
				Box[7].x = p.x;

			if ( Box[7].y < p.y )
				Box[7].y = p.y;

			if ( Box[7].z < p.z )
				Box[7].z = p.z;
		}
	}
	inline void EndCheck( void )
	{
		fMMStart = true;
		CalcCenter();

		Box[0].x = Box[0].x - Center.x;
		Box[0].y = Box[0].y - Center.y;
		Box[0].z = Box[0].z - Center.z;

		Box[7].x = Box[7].x - Center.x;
		Box[7].y = Box[7].y - Center.y;			
		Box[7].z = Box[7].z - Center.z;

		Box[1] = D3DXVECTOR3( Box[0].x, Box[7].y, Box[0].z );
		Box[2] = D3DXVECTOR3( Box[0].x, Box[0].y, Box[7].z );
		Box[3] = D3DXVECTOR3( Box[0].x, Box[7].y, Box[7].z );

		Box[4] = D3DXVECTOR3( Box[7].x, Box[0].y, Box[0].z );
		Box[5] = D3DXVECTOR3( Box[7].x, Box[7].y, Box[0].z );
		Box[6] = D3DXVECTOR3( Box[7].x, Box[0].y, Box[7].z );

		CalcCenter();

		#if ( defined( DEBUG ) || defined( _DEBUG ) )
			V_( SignMesh.CreateOBBMesh( Box, D3DXVECTOR4( 0.0f, 0.0f, 1.0f, 1.0f ) ) );
		#endif
	}

	inline CSignMesh* GetSignMesh( void ) { return &SignMesh; };
	inline HRESULT CreateSignMesh( void ) { return SignMesh.CreateOBBMesh( Box, D3DXVECTOR4( 0.0f, 0.0f, 1.0f, 1.0f ) ); };

protected:
	inline void CalcCenter( void )
	{
		Center.x = ( Box[0].x + Box[7].x ) / 2.0f;
		Center.y = ( Box[0].y + Box[7].y ) / 2.0f;
		Center.z = ( Box[0].z + Box[7].z ) / 2.0f;
	};

	bool fMMStart;
	D3DXVECTOR3 Box[8], Center;

	CSignMesh SignMesh;
};

