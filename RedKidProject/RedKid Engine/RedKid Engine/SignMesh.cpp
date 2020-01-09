#include "System.h"
#include "SignMesh.h"

CSignMesh::CSignMesh(void)
{
	g_pVertexBuffer = NULL;
	vb				= NULL;
	vbCount			= 0;
	PointCount		= 0;
}

CSignMesh::~CSignMesh(void)
{
	Release();
}

void CSignMesh::Release( void )
{
	RELEASE( g_pVertexBuffer );
	DEL_ARRAY( vb );
	PointCount = 0;
}

HRESULT CSignMesh::ResizeBuffer( UINT NewCount )
{
	if ( NewCount <= vbCount )
	{
		PointCount = NewCount;
		return S_OK;
	}

	DEL_ARRAY( vb );
	vb = new VERTEX_LINE[ NewCount ];
	if ( vb == NULL )
	{
		Release();
		LogFile.OutputLogFileErrorString( L"Memory allocation failure.", __FILE__, __FUNCTION__, __LINE__  );
		return S_FALSE;
	}

	static D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof( VERTEX_LINE ) * NewCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	if ( S_OK != SystemRedKid.GetDevice()->CreateBuffer( &bd, NULL, &g_pVertexBuffer ) )
	{
		Release();
		LogFile.OutputLogFileErrorString( L"Error CreateBuffer.", __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}

	PointCount = NewCount;
	vbCount = NewCount;

	return S_OK;
}

void CSignMesh::Draw( void )
{
	static UINT stride = sizeof( VERTEX_LINE );
	static UINT offset = 0;

	SystemRedKid.GetDeviceContext()->IASetVertexBuffers( 0, 1, &g_pVertexBuffer, &stride, &offset );
	SystemRedKid.GetDeviceContext()->Draw( PointCount, 0 );
}

HRESULT CSignMesh::CreateAABBMesh( CAABB* AABB, D3DXVECTOR4 &Col )
{
	V_RETURN( ResizeBuffer( 24 ) );

	for ( UINT i = 0; i < PointCount; i++ )
		vb[i].Col = Col;

	vb[0].Pos = D3DXVECTOR3( AABB->Min.x, AABB->Min.y, AABB->Min.z );
	vb[1].Pos = D3DXVECTOR3( AABB->Max.x, AABB->Min.y, AABB->Min.z );

	vb[2].Pos = D3DXVECTOR3( AABB->Max.x, AABB->Min.y, AABB->Min.z );
	vb[3].Pos = D3DXVECTOR3( AABB->Max.x, AABB->Min.y, AABB->Max.z );

	vb[4].Pos = D3DXVECTOR3( AABB->Max.x, AABB->Min.y, AABB->Max.z );
	vb[5].Pos = D3DXVECTOR3( AABB->Min.x, AABB->Min.y, AABB->Max.z );

	vb[6].Pos = D3DXVECTOR3( AABB->Min.x, AABB->Min.y, AABB->Max.z );
	vb[7].Pos = D3DXVECTOR3( AABB->Min.x, AABB->Min.y, AABB->Min.z );

	////////////////////////////////////////////////////////
	vb[8].Pos = D3DXVECTOR3( AABB->Min.x, AABB->Max.y, AABB->Min.z );
	vb[9].Pos = D3DXVECTOR3( AABB->Max.x, AABB->Max.y, AABB->Min.z );

	vb[10].Pos = D3DXVECTOR3( AABB->Max.x, AABB->Max.y, AABB->Min.z );
	vb[11].Pos = D3DXVECTOR3( AABB->Max.x, AABB->Max.y, AABB->Max.z );

	vb[12].Pos = D3DXVECTOR3( AABB->Max.x, AABB->Max.y, AABB->Max.z );
	vb[13].Pos = D3DXVECTOR3( AABB->Min.x, AABB->Max.y, AABB->Max.z );

	vb[14].Pos = D3DXVECTOR3( AABB->Min.x, AABB->Max.y, AABB->Max.z );
	vb[15].Pos = D3DXVECTOR3( AABB->Min.x, AABB->Max.y, AABB->Min.z );

	////////////////////////////////////////////////////////
	vb[16].Pos = D3DXVECTOR3( AABB->Min.x, AABB->Min.y, AABB->Min.z );
	vb[17].Pos = D3DXVECTOR3( AABB->Min.x, AABB->Max.y, AABB->Min.z );

	vb[18].Pos = D3DXVECTOR3( AABB->Max.x, AABB->Min.y, AABB->Min.z );
	vb[19].Pos = D3DXVECTOR3( AABB->Max.x, AABB->Max.y, AABB->Min.z );

	vb[20].Pos = D3DXVECTOR3( AABB->Max.x, AABB->Min.y, AABB->Max.z );
	vb[21].Pos = D3DXVECTOR3( AABB->Max.x, AABB->Max.y, AABB->Max.z );

	vb[22].Pos = D3DXVECTOR3( AABB->Min.x, AABB->Min.y, AABB->Max.z );
	vb[23].Pos = D3DXVECTOR3( AABB->Min.x, AABB->Max.y, AABB->Max.z );

	// Копируем данные массива в буффер
	static D3D11_MAPPED_SUBRESOURCE MappedResource;
	if ( S_OK == SystemRedKid.GetDeviceContext()->Map( g_pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &MappedResource ) )
	{
		memcpy( MappedResource.pData, vb, sizeof( VERTEX_LINE ) * PointCount );
		SystemRedKid.GetDeviceContext()->Unmap( g_pVertexBuffer, NULL );
	}
	else
		return S_FALSE;

	return S_OK;
}

HRESULT CSignMesh::CreatePointLightMesh( D3DXVECTOR3 Pos, float Rad )
{
	const UINT step = 32;

	V_RETURN( ResizeBuffer( 6 + step * 2 * 3 ) );

	for ( UINT i = 0; i < PointCount; i++ )
		vb[i].Col = D3DXVECTOR4( 1.0f, 1.0f, 0.0f, 1.0f );

	vb[0].Pos = D3DXVECTOR3( Pos.x - Rad, Pos.y, Pos.z );
	vb[1].Pos = D3DXVECTOR3( Pos.x + Rad, Pos.y, Pos.z );

	vb[2].Pos = D3DXVECTOR3( Pos.x, Pos.y - Rad, Pos.z );
	vb[3].Pos = D3DXVECTOR3( Pos.x, Pos.y + Rad, Pos.z );

	vb[4].Pos = D3DXVECTOR3( Pos.x, Pos.y, Pos.z - Rad );
	vb[5].Pos = D3DXVECTOR3( Pos.x, Pos.y, Pos.z + Rad );

	static float x,y, angle, stAngl;
	angle = 0;
	stAngl = ( 2.0f * (float)D3DX_PI ) / (float)step;
	for ( UINT i = 0; i < step; i++ )
	{
		vb[( i * 2 ) + 6].Pos = D3DXVECTOR3( Pos.x + Rad * cos( angle ), Pos.y + Rad * sin( angle ), Pos.z );
		vb[( i * 2 ) + 6 + step * 2].Pos = D3DXVECTOR3( Pos.x + Rad * cos( angle ), Pos.y, Pos.z + Rad * sin( angle ) );
		vb[( i * 2 ) + 6 + step * 4].Pos = D3DXVECTOR3( Pos.x, Pos.y + Rad * cos( angle ), Pos.z + Rad * sin( angle ) );
		angle += stAngl;
		vb[( i * 2 ) + 7].Pos = D3DXVECTOR3( Pos.x + Rad * cos( angle ), Pos.y + Rad * sin( angle ), Pos.z );
		vb[( i * 2 ) + 7 + step * 2].Pos = D3DXVECTOR3( Pos.x + Rad * cos( angle ), Pos.y, Pos.z + Rad * sin( angle ) );
		vb[( i * 2 ) + 7 + step * 4].Pos = D3DXVECTOR3( Pos.x, Pos.y + Rad * cos( angle ), Pos.z + Rad * sin( angle ) );
	}

	// Копируем данные массива в буффер
	static D3D11_MAPPED_SUBRESOURCE MappedResource;
	if ( S_OK == SystemRedKid.GetDeviceContext()->Map( g_pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &MappedResource ) )
	{
		memcpy( MappedResource.pData, vb, sizeof( VERTEX_LINE ) * PointCount );
		SystemRedKid.GetDeviceContext()->Unmap( g_pVertexBuffer, NULL );
	}
	else
		return S_FALSE;

	return S_OK;
}

HRESULT CSignMesh::CreateDirectoinLightMesh( D3DXMATRIX* WorldMat, float mSignScale )
{
	const UINT step = 24;
	static D3DXVECTOR3 Direction = D3DXVECTOR3( 0.0f, 0.0f, -1.0f );
	static float ss = 0.2f;
	static D3DXMATRIX Mat;

	V_RETURN( ResizeBuffer( 2 + 8 + 4 + step * 2 * 3 ) );

	for ( UINT i = 0; i < PointCount; i++ )
		vb[i].Col = D3DXVECTOR4( 1.0f, 1.0f, 0.0f, 1.0f );

	// Основная линия
	vb[0].Pos = Direction;
	vb[1].Pos = Direction; vb[1].Pos.z *= -1.0f;

	// Стрелки
	vb[2].Pos = vb[1].Pos;
	vb[3].Pos = vb[1].Pos; vb[3].Pos.x -= ss / 3.0f; vb[3].Pos.z -= ss;
	vb[4].Pos = vb[1].Pos;
	vb[5].Pos = vb[1].Pos; vb[5].Pos.x += ss / 3.0f; vb[5].Pos.z -= ss;
	vb[6].Pos = vb[1].Pos;
	vb[7].Pos = vb[1].Pos; vb[7].Pos.y -= ss / 3.0f; vb[7].Pos.z -= ss;
	vb[8].Pos = vb[1].Pos;
	vb[9].Pos = vb[1].Pos; vb[9].Pos.y += ss / 3.0f; vb[9].Pos.z -= ss;

	// Задний крест
	vb[10].Pos.x = Direction.x; vb[10].Pos.y = -ss / 2.0f; vb[10].Pos.z = Direction.z + ss / 2.0f;
	vb[11].Pos.x = Direction.x; vb[11].Pos.y =  ss / 2.0f; vb[11].Pos.z = Direction.z + ss / 2.0f;
	vb[12].Pos.x = -ss / 2.0f; vb[12].Pos.y = Direction.y; vb[12].Pos.z = Direction.z + ss / 2.0f;
	vb[13].Pos.x =  ss / 2.0f; vb[13].Pos.y = Direction.y; vb[13].Pos.z = Direction.z + ss / 2.0f;

	// Шар
	static float x,y, angle, stAngl;
	angle = 0;
	stAngl = ( 2.0f * (float)D3DX_PI ) / (float)step;
	float Rad = ss / 2.0f;
	static D3DXVECTOR3 Pos;
	Pos = Direction;
	Pos.z += Rad;
	for ( UINT i = 0; i < step; i++ )
	{
		vb[( i * 2 ) + 14].Pos = D3DXVECTOR3( Pos.x + Rad * cos( angle ), Pos.y + Rad * sin( angle ), Pos.z );
		vb[( i * 2 ) + 14 + step * 2].Pos = D3DXVECTOR3( Pos.x + Rad * cos( angle ), Pos.y, Pos.z + Rad * sin( angle ) );
		vb[( i * 2 ) + 14 + step * 4].Pos = D3DXVECTOR3( Pos.x, Pos.y + Rad * cos( angle ), Pos.z + Rad * sin( angle ) );
		angle += stAngl;
		vb[( i * 2 ) + 15].Pos = D3DXVECTOR3( Pos.x + Rad * cos( angle ), Pos.y + Rad * sin( angle ), Pos.z );
		vb[( i * 2 ) + 15 + step * 2].Pos = D3DXVECTOR3( Pos.x + Rad * cos( angle ), Pos.y, Pos.z + Rad * sin( angle ) );
		vb[( i * 2 ) + 15 + step * 4].Pos = D3DXVECTOR3( Pos.x, Pos.y + Rad * cos( angle ), Pos.z + Rad * sin( angle ) );
	}

	// Применение трансформаций
	D3DXMatrixScaling( &Mat, mSignScale, mSignScale, mSignScale );
	Mat = *WorldMat * Mat;
	for ( UINT i = 0; i < PointCount; i++ )
	{
		D3DXVec3TransformCoord( &vb[i].Pos, &vb[i].Pos, &Mat );
		vb[i].Col = D3DXVECTOR4( 1.0f, 1.0f, 0.0f, 1.0f );
	}

	// Копируем данные массива в буффер
	static D3D11_MAPPED_SUBRESOURCE MappedResource;
	if ( S_OK == SystemRedKid.GetDeviceContext()->Map( g_pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &MappedResource ) )
	{
		memcpy( MappedResource.pData, vb, sizeof( VERTEX_LINE ) * PointCount );
		SystemRedKid.GetDeviceContext()->Unmap( g_pVertexBuffer, NULL );
	}
	else
		return S_FALSE;

	return S_OK;
}