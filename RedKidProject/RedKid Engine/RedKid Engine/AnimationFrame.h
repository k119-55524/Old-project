#pragma once

#include "header.h"

class CAnimationFrame
{
public:
	CAnimationFrame( void );
	~CAnimationFrame( void );
	HRESULT LoadFrameInTextFile( std::ifstream &file, UINT cb );
	inline D3DXVECTOR4& GetOffset( UINT Index ) { return BoneAnimation[Index].Offsets; };
	inline D3DXQUATERNION& GetRotation( UINT Index ) { return BoneAnimation[Index].Rotations; };

	LPBONE_ANIMATION BoneAnimation;

protected:
	void Release( void );
};

