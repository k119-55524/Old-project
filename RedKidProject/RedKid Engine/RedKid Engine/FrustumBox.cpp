#include "FrustumBox.h"

CFrustumBox::CFrustumBox( float d, UINT nLev )
{
	cAABB.Set( D3DXVECTOR3( -d, -d, -d ), D3DXVECTOR3( d, d, d ) );

	for ( UINT i = 0; i < 8; i++ )
		ChildFrustum[i] = NULL;

	NumLevel = nLev;	// Самый верхний уровень(к-во вложенностей)
	mParentFrustum = NULL;

	TestChlrFrust[0][0] = D3DXVECTOR3( cAABB.Min.x, cAABB.Center.y, cAABB.Min.z );
	TestChlrFrust[0][1] = D3DXVECTOR3( cAABB.Center.x, cAABB.Max.y, cAABB.Center.z );
	TestChlrFrust[1][0] = D3DXVECTOR3( cAABB.Center.x, cAABB.Center.y, cAABB.Min.z );
	TestChlrFrust[1][1] = D3DXVECTOR3( cAABB.Max.x,	cAABB.Max.y, cAABB.Center.z );
	TestChlrFrust[2][0] = D3DXVECTOR3( cAABB.Min.x, cAABB.Min.y, cAABB.Min.z );
	TestChlrFrust[2][1] = D3DXVECTOR3( cAABB.Center.x, cAABB.Center.y, cAABB.Center.z );
	TestChlrFrust[3][0] = D3DXVECTOR3( cAABB.Center.x, cAABB.Min.y, cAABB.Min.z );
	TestChlrFrust[3][1] = D3DXVECTOR3( cAABB.Max.x, cAABB.Center.y, cAABB.Center.z );
	TestChlrFrust[4][0] = D3DXVECTOR3( cAABB.Min.x, cAABB.Center.y, cAABB.Center.z );
	TestChlrFrust[4][1] = D3DXVECTOR3( cAABB.Center.x, cAABB.Max.y, cAABB.Max.z );
	TestChlrFrust[5][0] = D3DXVECTOR3( cAABB.Center.x, cAABB.Center.y, cAABB.Center.z );
	TestChlrFrust[5][1] = D3DXVECTOR3( cAABB.Max.x, cAABB.Max.y, cAABB.Max.z );
	TestChlrFrust[6][0] = D3DXVECTOR3( cAABB.Min.x, cAABB.Min.y, cAABB.Center.z );
	TestChlrFrust[6][1] = D3DXVECTOR3( cAABB.Center.x, cAABB.Center.y, cAABB.Max.z );
	TestChlrFrust[7][0] = D3DXVECTOR3( cAABB.Center.x, cAABB.Min.y, cAABB.Center.z );
	TestChlrFrust[7][1] = D3DXVECTOR3( cAABB.Max.x, cAABB.Center.y, cAABB.Max.z );

	#if (  defined( DEBUG ) || defined( _DEBUG ) )
		V_( SignGreenMesh.CreateAABBMesh( &cAABB, D3DXVECTOR4( 0.0f, 1.0f, 0.0f, 1.0f ) ) );
		V_( SignFillMesh.CreateAABBMesh( &cAABB, D3DXVECTOR4( F_COLOR( 96 ), F_COLOR( 41 ), F_COLOR( 197 ), 1.0f ) ) );
	#endif
}

CFrustumBox::CFrustumBox( UINT numlevel, CFrustumBox* mParent, D3DXVECTOR3 Min, D3DXVECTOR3 Max )
{
	for ( UINT i = 0; i < 8; i++ )
		ChildFrustum[i] = NULL;

	cAABB.Set( Min, Max );

	NumLevel = numlevel;	// Самый верхний уровень(к-во вложенностей)
	mParentFrustum = mParent;

	TestChlrFrust[0][0] = D3DXVECTOR3( cAABB.Min.x, cAABB.Center.y, cAABB.Min.z );
	TestChlrFrust[0][1] = D3DXVECTOR3( cAABB.Center.x, cAABB.Max.y, cAABB.Center.z );
	TestChlrFrust[1][0] = D3DXVECTOR3( cAABB.Center.x, cAABB.Center.y, cAABB.Min.z );
	TestChlrFrust[1][1] = D3DXVECTOR3( cAABB.Max.x,	cAABB.Max.y, cAABB.Center.z );
	TestChlrFrust[2][0] = D3DXVECTOR3( cAABB.Min.x, cAABB.Min.y, cAABB.Min.z );
	TestChlrFrust[2][1] = D3DXVECTOR3( cAABB.Center.x, cAABB.Center.y, cAABB.Center.z );
	TestChlrFrust[3][0] = D3DXVECTOR3( cAABB.Center.x, cAABB.Min.y, cAABB.Min.z );
	TestChlrFrust[3][1] = D3DXVECTOR3( cAABB.Max.x, cAABB.Center.y, cAABB.Center.z );
	TestChlrFrust[4][0] = D3DXVECTOR3( cAABB.Min.x, cAABB.Center.y, cAABB.Center.z );
	TestChlrFrust[4][1] = D3DXVECTOR3( cAABB.Center.x, cAABB.Max.y, cAABB.Max.z );
	TestChlrFrust[5][0] = D3DXVECTOR3( cAABB.Center.x, cAABB.Center.y, cAABB.Center.z );
	TestChlrFrust[5][1] = D3DXVECTOR3( cAABB.Max.x, cAABB.Max.y, cAABB.Max.z );
	TestChlrFrust[6][0] = D3DXVECTOR3( cAABB.Min.x, cAABB.Min.y, cAABB.Center.z );
	TestChlrFrust[6][1] = D3DXVECTOR3( cAABB.Center.x, cAABB.Center.y, cAABB.Max.z );
	TestChlrFrust[7][0] = D3DXVECTOR3( cAABB.Center.x, cAABB.Min.y, cAABB.Center.z );
	TestChlrFrust[7][1] = D3DXVECTOR3( cAABB.Max.x, cAABB.Center.y, cAABB.Max.z );

	#if (  defined( DEBUG ) || defined( _DEBUG ) )
		V_( SignGreenMesh.CreateAABBMesh( &cAABB, D3DXVECTOR4( 0.0f, 1.0f, 0.0f, 1.0f ) ) );
		V_( SignFillMesh.CreateAABBMesh( &cAABB, D3DXVECTOR4( F_COLOR( 96 ), F_COLOR( 41 ), F_COLOR( 197 ), 1.0f ) ) );
	#endif
}

CFrustumBox::~CFrustumBox( void )
{
	Release();
}

void CFrustumBox::Release( void )
{
	for ( UINT i = 0; i < 8; i++ )
		RELEASE( ChildFrustum[i] );
}

HRESULT CFrustumBox::AddObject( CObjectBase* Obj )
{
	#if ( !( defined( DEBUG ) || defined( _DEBUG ) ) )
		if ( Obj->GetType() == OBJ_PIVOT )
			return S_OK;
	#endif

	CAABB* AABBObj = Obj->GetAABB();
	if ( cAABB.TestAABB_In( AABBObj ) )
		V_RETURN( AddObjectChild( Obj ) )
	else
	{
		V_RETURN( RenderObj.Add( Obj ) ); 
		Obj->SetFrustum( this );
	}

	return S_OK;
}

HRESULT CFrustumBox::AddObjectChild( CObjectBase* Obj )
{
	CAABB* AABBObj = Obj->GetAABB();

	if ( !cAABB.TestAABB_In( AABBObj ) )
		return S_FALSE;

	D3DXVECTOR3* MinObj = AABBObj->GetMin();
	D3DXVECTOR3* MaxObj = AABBObj->GetMax();

	if ( NumLevel == 0 )
	{
		V_RETURN( RenderObj.Add( Obj ) );
		Obj->SetFrustum( this );
		return S_OK;
	}

	for ( UINT i = 0; i < 8; i ++ )
		if ( TestChildBox( i, AABBObj ) )
		{
			if ( ChildFrustum[i] == NULL )
			{
				ChildFrustum[i] = new CFrustumBox( NumLevel - 1, this, TestChlrFrust[i][0], TestChlrFrust[i][1] );
				if ( ChildFrustum[i] == NULL )
				{
					LogFile.OutputLogFileErrorString( L"Memory allocation failure( new CFrustumBox() ).", __FILE__, __FUNCTION__, __LINE__ );
					return S_FALSE;
				}
			}
			if ( S_OK == ChildFrustum[i]->AddObjectChild( Obj ) )
				return S_OK;
		}

	V_RETURN( RenderObj.Add( Obj ) );
	Obj->SetFrustum( this );

	return S_OK;
}

void CFrustumBox::DefineFrustumStart( CObjectBase* Obj )
{
	static CAABB* AABBObj;
	AABBObj = Obj->GetAABB();

	static D3DXVECTOR3* MinObj, *MaxObj;
	MaxObj = AABBObj->GetMin();
	MaxObj = AABBObj->GetMax();

	// Если попадает
	if ( cAABB.TestAABB_In( AABBObj ) )
	{
		for ( UINT i = 0; i < 8; i ++ )
			if ( TestChildBox( i, AABBObj ) )
			{
				if ( ChildFrustum[i] == NULL )
				{
					ChildFrustum[i] = new CFrustumBox( NumLevel - 1, this, TestChlrFrust[i][0], TestChlrFrust[i][1] );
					if ( ChildFrustum[i] == NULL )
					{
						LogFile.OutputLogFileErrorString( L"Memory allocation failure( new CFrustumBox() ).", __FILE__, __FUNCTION__, __LINE__ );
						return;
					}
				}
				if ( S_OK == ChildFrustum[i]->AddObjectChild( Obj ) )
				{
					RenderObj.FindAndCutObject( Obj );
					return;
				}
			}
	}
	else
		if ( mParentFrustum != NULL )
			if ( S_OK == mParentFrustum->DefineFrustum( Obj ) )
				RenderObj.FindAndCutObject( Obj );

	return;
}

HRESULT CFrustumBox::DefineFrustum( CObjectBase* Obj )
{
	static CAABB* AABBObj;
	AABBObj = Obj->GetAABB();

	static D3DXVECTOR3* MinObj, *MaxObj;
	MaxObj = AABBObj->GetMin();
	MaxObj = AABBObj->GetMax();

	// Если попадает
	if ( cAABB.TestAABB_In( AABBObj ) )
	{
		for ( UINT i = 0; i < 8; i ++ )
			if ( TestChildBox( i, AABBObj ) )
			{
				if ( ChildFrustum[i] == NULL )
				{
					ChildFrustum[i] = new CFrustumBox( NumLevel - 1, this, TestChlrFrust[i][0], TestChlrFrust[i][1] );
					if ( ChildFrustum[i] == NULL )
					{
						LogFile.OutputLogFileErrorString( L"Memory allocation failure( new CFrustumBox() ).", __FILE__, __FUNCTION__, __LINE__ );
						return S_FALSE;
					}
				}
				if ( S_OK == ChildFrustum[i]->AddObjectChild( Obj ) )
					return S_OK;
			}

		V_RETURN( RenderObj.Add( Obj ) );
		Obj->SetFrustum( this );
	}
	else
	{
		if ( mParentFrustum != NULL )
			mParentFrustum->DefineFrustum( Obj );
		else
		{
			V_RETURN( RenderObj.Add( Obj ) );
			Obj->SetFrustum( this );
		}
		return S_OK;
	}

	return S_OK;
}
