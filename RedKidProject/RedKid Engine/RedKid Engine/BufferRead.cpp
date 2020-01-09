#include "System.h"
#include "BufferRead.h"

CBufferRead::CBufferRead( void )
{
	g_pBuffer = NULL;
}

CBufferRead::~CBufferRead( void )
{
	Release();
}

void CBufferRead::Release( void )
{
	RELEASE( g_pBuffer );
}

HRESULT CBufferRead::Create( UINT StructureByteStride, UINT Count )
{
	static HRESULT res;

	#if defined( DEBUG ) || defined( _DEBUG )
		if ( StructureByteStride == 0 )
		{
			LogFile.OutputLogFileErrorString( L"Error argument CD3DBuffer::CreateBufferAndUAV : #StructureByteStride == 0", __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}

		if ( Count == 0 )
		{
			LogFile.OutputLogFileErrorString( L"Error argument CD3DBuffer::CreateBufferAndUAV : #Count == 0", __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}
	#endif

	Release();

	static D3D11_BUFFER_DESC DescBuffer;
	ZeroMemory( &DescBuffer, sizeof(D3D11_BUFFER_DESC) );
	DescBuffer.ByteWidth = StructureByteStride * Count;
	DescBuffer.Usage = D3D11_USAGE_STAGING;
	DescBuffer.StructureByteStride = StructureByteStride;
	DescBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	V_RETURN( SystemRedKid.GetDevice()->CreateBuffer( &DescBuffer, NULL, &g_pBuffer ) );

	return S_OK;
}

void CBufferRead::CopyResource( CBufferUAV* inBuf )
{
	SystemRedKid.GetDeviceContext()->CopyResource( g_pBuffer, inBuf->GetBuffer() );
};