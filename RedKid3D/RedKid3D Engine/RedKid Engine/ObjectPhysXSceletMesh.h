#pragma once

#include "OBB.h"
#include "Scelet.h"
#include "ObjectPhysx.h"
#include "ResPhysXSceletMesh.h"

class CObjectPhysXSceletMesh :
	public CObjectPhysX
{
public:
	CObjectPhysXSceletMesh( TCHAR* name );
	~CObjectPhysXSceletMesh( void );
	void Release( void );

	HRESULT AddMesh( CResMesh* mesh, PxScene* mPhysXScene, CResourceAnimationScelet* scanim );
	HRESULT AddMesh( CResMesh* mesh, PxScene* mPhysXScene ) { return S_FALSE; };

protected:
	void SetPhysXPosition( float x, float y, float z );
	void UpdatePhysXPose( void );

	void CalcAABB( D3DXMATRIX* m_GlobalWordMat );
	HRESULT AddSignMesh( CArrayStack<CSignMesh*> &SignMesh );
	//void MulVec3BoneTrans( D3DXVECTOR3 *vOut, D3DXVECTOR3 *vIn, LPBONE_ANIMATION bt );
	//LPBONE_ANIMATION BoneTransformInverse( LPBONE_ANIMATION bt );

	CScelet Scelet;
	COBB*	GlobalSceletOBB;

	UINT ActivAnim;
	CArray<CResourceAnimationScelet*> aSceletAnim;
	void UpdateScelet( float dt );
	float mDTimeScAnim;

	// PhysX
	PxRigidDynamic**	PhysXActor;
	PxMaterial*			PhysXMaterial;
	PxShape**			PhysXShape;
};

