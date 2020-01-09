#include "BufferConstant.h"
#include "System.h"

CBufferConstant::CBufferConstant( void )
{
	g_pBuffer = NULL;
	ByteWidth = 0;
	RealByteWidth = 0;
}

CBufferConstant::~CBufferConstant( void )
{
	Release();
}

void CBufferConstant::Release( void )
{
	ByteWidth = 0;
	RealByteWidth = 0;
	RELEASE( g_pBuffer );
}

// Создаём константный буффер
HRESULT CBufferConstant::Create( UINT byteWidth )
{
	#if defined( DEBUG ) || defined( _DEBUG )
		if ( byteWidth == 0 )
		{
			LogFile.OutputLogFileErrorString( L"Error argument CBufferConstant::Create : #ByteWidth == 0", __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}
	#endif

	Release();

	RealByteWidth = byteWidth;

	if ( byteWidth % 16 > 0 )
			byteWidth = ( ( byteWidth / 16 ) + 1 ) *16;

	static D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof( D3D11_BUFFER_DESC ) );
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.ByteWidth = byteWidth;
	if ( S_OK != SystemRedKid.GetDevice()->CreateBuffer( &bd, NULL, &g_pBuffer ) )
	{
		Release();
		LogFile.OutputLogFileErrorString( L"Error CreateBuffer.", __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}

	ByteWidth = byteWidth;

	return S_OK;
}

void CBufferConstant::CSSetConstantBuffers( UINT slot ) { SystemRedKid.GetDeviceContext()->CSSetConstantBuffers( slot, 1, &g_pBuffer ); };

void CBufferConstant::PSSetConstantBuffers( UINT slot ) { SystemRedKid.GetDeviceContext()->PSSetConstantBuffers( slot, 1, &g_pBuffer ); };

HRESULT CBufferConstant::of_CPU_in_GPU_Copy( void* CPUData )
{
	#if defined( DEBUG ) || defined( _DEBUG )
		if ( g_pBuffer == NULL )
		{
			LogFile.OutputLogFileErrorString( L"Error. NULL constan buffer.", __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}
	#endif

	static D3D11_MAPPED_SUBRESOURCE MappedResource;

	V_RETURN( SystemRedKid.GetDeviceContext()->Map( g_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource ) );
	memcpy( MappedResource.pData, CPUData, RealByteWidth );
	SystemRedKid.GetDeviceContext()->Unmap( g_pBuffer, 0 );

	return S_OK;
};

HRESULT CBufferConstant::of_CPU_in_GPU_Copy( void* CPUData, UINT count )
{
	static D3D11_MAPPED_SUBRESOURCE MappedResource;

	V_RETURN( SystemRedKid.GetDeviceContext()->Map( g_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource ) );
	memcpy( MappedResource.pData, CPUData, count );
	SystemRedKid.GetDeviceContext()->Unmap( g_pBuffer, 0 );

	return S_OK;
};