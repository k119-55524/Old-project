#pragma once

#include "header.h"

class CPlane
{
public:
	CPlane(void);

	D3DXVECTOR3 Normal;
	float Distance;
	inline void Normalize()
	{
		float tmp = 1.0f / sqrtf( ( Normal.x * Normal.x ) + Normal.y * Normal.y ) + ( Normal.z * Normal.z );
		Normal.x = Normal.x * tmp;
		Normal.y = Normal.y * tmp;
		Normal.z = Normal.z * tmp;
		Distance = Distance * tmp;
	}
	inline float DistanceToPoint( D3DXVECTOR3 &Point ) { return D3DXVec3Dot( &Normal, &Point ) + Distance; }
};

