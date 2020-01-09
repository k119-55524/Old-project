#pragma once

#include "header.h"
#include "FrustumBox.h"

class CObjectBase
{
	friend class CRedKid;
	friend class CRenderPool;

public:
	CObjectBase( void );
	CObjectBase( TCHAR* name );
	virtual ~CObjectBase( void );
	void Release( void );
	eTypeObject GetType( void ) { return Type; };
	inline CSignMesh* GetSignMesh( void ) {	return &cSignMesh; };

	inline void AddChild( CObjectBase* Obj )
	{
		if ( ChildObj != NULL )
		{
			Obj->SetPrev( ChildObj->GetPrev() );
			ChildObj->SetPrev( Obj );
			Obj->SetNext( ChildObj );
		}

		ChildObj = Obj;
		Obj->SetParent( this );
	}
	inline void SetFrustum( CFrustumBox* fr ) {	Frustum = fr; };
	inline void MovePosition( float x, float y, float z )
	{
		Position.x += x;
		Position.y += y;
		Position.z += z;

		fCalcLocalMat = true;
		fCalcGlobalMat = true;
		fCalcAABB = true;
	};
	virtual void SetPosition( float x, float y, float z )
	{
		Position.x = x;
		Position.y = y;
		Position.z = z;

		fCalcLocalMat = true;
		fCalcGlobalMat = true;
		fCalcAABB = true;
	};
	inline void MoveRotation( float x, float y, float z )
	{
		Angle.x += x;
		Angle.y += y;
		Angle.z += z;

		fCalcLocalMat = true;
		fCalcGlobalMat = true;
		fCalcAABB = true;
	};
	inline void SetRotation( float x, float y, float z )
	{
		Angle.x = x;
		Angle.y = y;
		Angle.z = z;

		fCalcLocalMat = true;
		fCalcGlobalMat = true;
		fCalcAABB = true;
	};
	inline D3DXVECTOR3* GetPosition( void ) { return &Position; };

	inline bool GetRenderSign( void ) { return fRenderSign; };
	inline void SetRenderSign( bool f ) { fRenderSign = f; };

	virtual inline CAABB* GetAABB( void )
	{
		if ( !fCalcAABB )
			return &AABB;

		CalcAABB( GetGlobalWordMat() );

		fCalcAABB = false;
		return &AABB;
	};

protected:
	inline D3DXMATRIX* GetLocalMatrix( void )
	{
		if ( !fCalcLocalMat )
			return &m_LocalWordMat;

		static D3DXMATRIX g_Position, xRot, yRot, zRot;

		D3DXMatrixRotationX( &xRot, Angle.x );
		D3DXMatrixRotationY( &yRot, Angle.y );
		D3DXMatrixRotationZ( &zRot, Angle.z );
		D3DXMatrixTranslation( &g_Position, Position.x, Position.y, Position.z );

		m_LocalWordMat = xRot * yRot * zRot * g_Position;

		fCalcLocalMat = false;

		return &m_LocalWordMat;
	};
	inline D3DXMATRIX* GetGlobalWordMat( void )	// Выдаёт глобальную матрицу объекта
	{
		// Если не надо изменять то сразу выдаём
		if ( !fCalcLocalMat && !fCalcGlobalMat )
			return &m_GlobalWordMat;

		// Иначе пересчитываем с учётом положения материнского объекта
		UpdateGlobalWordMat();

		// Устанавливаем флаги пересчёта глобальных матриц дочерних объектов
		if ( ChildObj != NULL )
			SetfCalcGlobalMat( ChildObj );

		//CalcAABB( &m_GlobalWordMat );

		return &m_GlobalWordMat;
	};
	inline D3DXMATRIX* UpdateGlobalWordMat( void )
	{
		// Если не главный конень
		if ( ParentObj != NULL )
			m_GlobalWordMat = *ParentObj->UpdateGlobalWordMat() * *GetLocalMatrix();
		else
			m_GlobalWordMat = *GetLocalMatrix();

		fCalcGlobalMat = false;

		return &m_GlobalWordMat;
	}
	inline void SetfCalcGlobalMat( CObjectBase* Obj )
	{
		if ( Obj == NULL )
			return;

		fCalcGlobalMat = true;
		fCalcAABB = true;

		do
		{
			SetfCalcGlobalMat( Obj );
			SetfCalcGlobalMat( Obj->ChildObj );

			Obj = NextObj;
		} while ( Obj != NULL );
	};
	virtual void CalcAABB( D3DXMATRIX* m_GlobalWordMat ) = 0;
	inline void CalcAABB( void )
	{
		CalcAABB( GetGlobalWordMat() );
		fCalcAABB = false;
	};

	TCHAR* Name;
	bool fRenderSign;
	bool fCalcLocalMat;
	bool fCalcGlobalMat;
	bool fCalcAABB;

	D3DXVECTOR3 Position;
	D3DXVECTOR3 Angle;
	D3DXMATRIX	m_LocalWordMat;
	D3DXMATRIX	m_GlobalWordMat;

	inline void SetParent( CObjectBase* Obj ) { ParentObj = Obj; };
	inline void SetChild( CObjectBase* Obj ) { ChildObj = Obj; };
	inline void SetPrev( CObjectBase* Obj ) { PrevObj = Obj; };
	inline void SetNext( CObjectBase* Obj ) { NextObj = Obj; };

	inline TCHAR* GetName( void ) { return Name; };
	inline CObjectBase* GetParent( void ) { return ParentObj; };
	inline CObjectBase* GetChild( void ) { return ChildObj; };
	inline CObjectBase* GetPrev( void ) { return PrevObj; };
	inline CObjectBase* GetNext( void ) { return NextObj; };

	CObjectBase* ParentObj;
	CObjectBase* ChildObj;
	CObjectBase* PrevObj;
	CObjectBase* NextObj;

	CAABB AABB;
	CSignMesh cSignMesh;
	CFrustumBox* Frustum;
	eTypeObject Type;
};

