#pragma once

#include "header.h"

class CAABB
{
	friend class CSignMesh;
	friend class CFrustumBox;

public:
	CAABB( void );
	inline void Set( float minx, float miny, float minz, float maxx, float maxy, float maxz )
	{
		Min.x = minx;
		Min.y = miny;
		Min.z = minz;

		Max.x = maxx;
		Max.y = maxy;
		Max.z = maxz;

		Center.x = ( Min.x + Max.x ) / 2.0f;
		Center.y = ( Min.y + Max.y ) / 2.0f;
		Center.z = ( Min.z + Max.z ) / 2.0f;
	}
	inline void Set( D3DXVECTOR3 &min, D3DXVECTOR3 &max )
	{
		Min = min;
		Max = max;

		//float t;
		//if ( Min.x > Max.x )
		//{
		//	t = Min.x;
		//	Min.x = Max.x;
		//	Max.x = t;
		//}
		//if ( Min.y > Max.y )
		//{
		//	t = Min.y;
		//	Min.y = Max.y;
		//	Max.y = t;
		//}
		//if ( Min.z > Max.z )
		//{
		//	t = Min.z;
		//	Min.z = Max.z;
		//	Max.z = t;
		//}

		Center.x = ( Min.x + Max.x ) / 2.0f;
		Center.y = ( Min.y + Max.y ) / 2.0f;
		Center.z = ( Min.z + Max.z ) / 2.0f;
	};
	inline D3DXVECTOR3* GetMin( void ) { return &Min; };
	inline D3DXVECTOR3* GetMax( void ) { return &Max; };
	inline D3DXVECTOR3* GetCenter( void ) { return &Center; };

	inline bool TestAABB_In( CAABB* ab ) // Если тестируемый бокс находится в нутри этого бокса(класса)
	{
		if ( ab->Min.x < Min.x || ab->Min.y < Min.y || ab->Min.z < Min.z ||
			 ab->Max.x > Max.x || ab->Max.y > Max.y || ab->Max.z > Max.z )
			return false;

		return true;
	};

private:
	D3DXVECTOR3 Min;
	D3DXVECTOR3 Max;
	D3DXVECTOR3 Center;
};

