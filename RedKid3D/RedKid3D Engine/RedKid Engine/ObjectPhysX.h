#pragma once

#include "header.h"
#include "ObjectMesh.h"

class CObjectPhysX :
	public CObjectMesh
{
public:
	CObjectPhysX( TCHAR* name );
	~CObjectPhysX( void );
	void Release( void );

	virtual void SetPhysXPosition( float x, float y, float z ) = 0;
	virtual void UpdatePhysXPose( void ) = 0;
	HRESULT AddMesh( CResMesh* mesh ) { return S_FALSE; };
	virtual HRESULT AddMesh( CResMesh* mesh, PxScene* mPhysXScene ) = 0;

protected:
	PxActor* PhysXActor;
	PxScene* PhysXScene;
};

