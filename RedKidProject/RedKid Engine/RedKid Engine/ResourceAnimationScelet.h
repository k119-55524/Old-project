#pragma once

#include "header.h"
#include "CArray.h"
#include "ResBase.h"
#include "AnimationFrame.h"

class CResourceAnimationScelet
	: public CResBase
{

public:
	CResourceAnimationScelet(void);
	~CResourceAnimationScelet(void);
	void Release();
	HRESULT CreateAnimationOfTXTFile( TCHAR* FileName, UINT nB );
	inline UINT GetBoneCount( void ) { return NumBone; };
	inline UINT GetFrameCount( void ) { return NumFrame; };
	inline CAnimationFrame* GetFrame( UINT Index ) { return &AnimationFrames[Index]; };
	inline UINT GetPerFrame( void ) { return cPerFrame; };

private:
	CAnimationFrame* AnimationFrames;
	UINT cPerFrame;	//  оличество кадров анимации в секунду(fps)
	UINT NumBone, NumFrame;

};

