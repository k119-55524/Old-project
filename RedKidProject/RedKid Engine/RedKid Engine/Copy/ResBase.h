pragma once

#include "header.h"

// Состояние ресурса
enum eResourceCondition : unsigned int
{
	COND_CREATE = 0,	// Ресурс толькочто создан и не подлежит удалению
	COND_DEL_NULL_LINK,	// Удалять при отсутствии ссылок на ресурс
};

class CResBase
{
	friend class CObjectMesh;
	friend class CResMesh;
	friend class CGameInterface;
	friend class CManagerResource;
	friend class CMaterialTexture;
	friend class CResourceSceletMesh;

public:
	CResBase( void );
	~CResBase( void );

	inline TCHAR* GetName( void ) { return Name; };
	inline eTypeResource GetType( void ) { return Type; };
	inline eResourceCondition GetCondition( void ) { return Condition; };

protected:
	TCHAR* Name;
	eTypeResource Type;
	eResourceCondition Condition;

private:
	UINT LinkCount;
	int IndexArray;
	inline int GetIndexArray( void ) { return IndexArray; };
	inline void SetIndexArray( int Index ) { IndexArray = Index; };
	inline void IncrementLink() { LinkCount++; Condition = COND_DEL_NULL_LINK; };
	inline void DecrementLink() { if ( LinkCount > 0 ) LinkCount--; };

	ID3D11DeviceContext*	g_pImmediateContext;
};

