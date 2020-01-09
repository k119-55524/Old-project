#include "AnimationFrame.h"

CAnimationFrame::CAnimationFrame( void )
{
	BoneAnimation = NULL;
}

CAnimationFrame::~CAnimationFrame( void )
{
	Release();
}

void CAnimationFrame::Release( void )
{
	DEL_ARRAY( BoneAnimation );
}

HRESULT CAnimationFrame::LoadFrameInTextFile( std::ifstream &file, UINT cb )
{
	Release();

	BoneAnimation = new BONE_ANIMATION[cb];
	if ( BoneAnimation == NULL )
		return S_FALSE;

	for ( UINT i = 0; i < cb; i++ )
	{
		file >> BoneAnimation[i].Offsets.x;
		file >> BoneAnimation[i].Offsets.y;
		file >> BoneAnimation[i].Offsets.z;

		file >> BoneAnimation[i].Rotations.x;
		file >> BoneAnimation[i].Rotations.y;
		file >> BoneAnimation[i].Rotations.z;
		file >> BoneAnimation[i].Rotations.w;
	};

	return S_OK;
}
