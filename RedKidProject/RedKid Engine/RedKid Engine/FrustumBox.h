#pragma once

#include "header.h"
#include "AABB.h"
#include "CList.h"
#include "SignMesh.h"
#include "ObjectBase.h"

class CFrustumBox
{
	friend class CCamera;
	friend class CFrustumBox;

public:
	CFrustumBox( float d, UINT nLev );
	CFrustumBox( UINT numlevel, CFrustumBox* mParent, D3DXVECTOR3 Min, D3DXVECTOR3 Max );
	~CFrustumBox(void);
	void Release( void );

	HRESULT AddObject( CObjectBase* Obj );

	inline D3DXVECTOR3* GetMin( void ) { return &cAABB.Min; };
	inline D3DXVECTOR3* GetMax( void ) { return &cAABB.Max; };
	inline CSignMesh* GetSignGreenMesh( void ) { return &SignGreenMesh; };
	inline CSignMesh* GetSignFillMesh( void ) { return &SignFillMesh; };

	inline CFrustumBox* GetParentFrustum( void ) { return mParentFrustum; };
	inline UINT GetObjectCount( void ) { return RenderObj.GetSize(); };
	inline void FindObjStart( void ) { RenderObj.FindStart(); };
	inline CObjectBase* GetFindObject( void ) { return RenderObj.GetFindObject(); };
	inline void FindNext( void ) { RenderObj.FindNext(); };
	inline CFrustumBox* GetChildFrustum( UINT i ) { return ChildFrustum[i]; };

	void DefineFrustumStart( CObjectBase* Obj );

protected:
	HRESULT DefineFrustum( CObjectBase* Obj );

	UINT NumLevel;
	CAABB cAABB;
	HRESULT AddObjectChild( CObjectBase* Obj );
	inline bool CFrustumBox::TestChildBox( UINT i, CAABB* AABBObj )
	{
		if ( i > 7 )
			return S_FALSE;

		if ( AABBObj->GetMin()->x < TestChlrFrust[i][0].x || AABBObj->GetMin()->y < TestChlrFrust[i][0].y || AABBObj->GetMin()->z < TestChlrFrust[i][0].z ||
			 AABBObj->GetMax()->x > TestChlrFrust[i][1].x || AABBObj->GetMax()->y > TestChlrFrust[i][1].y || AABBObj->GetMax()->z > TestChlrFrust[i][1].z )
			 return false;

		return true;
	};
	inline void SetAABB( D3DXVECTOR3 &min, D3DXVECTOR3 &max ) { cAABB.Set( min, max ); };

	CFrustumBox* ChildFrustum[8];
	D3DXVECTOR3 TestChlrFrust[8][2];
	CList<CObjectBase*> RenderObj;

	CSignMesh SignGreenMesh;
	CSignMesh SignFillMesh;

private:
	CFrustumBox* mParentFrustum;
};

