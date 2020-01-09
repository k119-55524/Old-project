#pragma once

#include "header.h"
#include "ObjectPhysX.h"
#include "ResPhysXStaticTriangleMesh.h"

class CObjectPhysXTriangleStaticMesh :
	public CObjectPhysX
{
public:
	CObjectPhysXTriangleStaticMesh( TCHAR* name );
	~CObjectPhysXTriangleStaticMesh( void );
	void Release( void );

	HRESULT AddMesh( CResMesh* mesh, PxScene* mPhysXScene );
	void SetPhysXPosition( float x, float y, float z )
	{
		SetPosition( x, y, z );

		static PxTransform Pos;
		Pos = ((PxRigidStatic*)PhysXActor)->getGlobalPose();
		Pos.p.x = x;
		Pos.p.y = y;
		Pos.p.z = z;
		((PxRigidStatic*)PhysXActor)->setGlobalPose( Pos );

		CObjectBase::CalcAABB();
		Frustum->DefineFrustumStart( this );
	};
	void UpdatePhysXPose( void )
	{
		static PxTransform pxTr;
		static D3DXQUATERNION q;
		static D3DXMATRIX g_Position;
		static D3DXMATRIX g_Rotation;

		pxTr = ((PxRigidStatic*)PhysXActor)->getGlobalPose();

		SetPosition( pxTr.p.x, pxTr.p.y, pxTr.p.z );

		q.x = pxTr.q.x;
		q.y = pxTr.q.y;
		q.z = pxTr.q.z;
		q.w = pxTr.q.w;

		D3DXMatrixTranslation( &g_Position, Position.x, Position.y, Position.z );
		D3DXMatrixRotationQuaternion ( &g_Rotation, &q );
		//D3DXMatrixTranspose( &g_Rotation, &g_Rotation );

		m_LocalWordMat = g_Rotation * g_Position;

		fCalcLocalMat = false;

		CObjectBase::CalcAABB();
		Frustum->DefineFrustumStart( this );
	};

protected:
	void CalcAABB( D3DXMATRIX* m_GlobalWordMat );

	//PxRigidStatic*	PhysXActor;
	PxMaterial*     PhysXMaterial;
	PxShape*		PhysXShape;
};

