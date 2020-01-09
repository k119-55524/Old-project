#include "System.h"
#include "BufferUAV.h"

CBufferUAV::CBufferUAV(void)
{
	g_pBuffer		= NULL;
	g_pBufferUAV	= NULL;
}

CBufferUAV::~CBufferUAV(void)
{
	Release();
}

void CBufferUAV::Release( void )
{
	RELEASE( g_pBuffer );
	RELEASE( g_pBufferUAV );
}

HRESULT CBufferUAV::Create( UINT StructureByteStride, UINT Count )
{
	#if defined( DEBUG ) || defined( _DEBUG )
		if ( StructureByteStride == 0 )
		{
			LogFile.OutputLogFileErrorString( L"Error StructureByteStride == 0", __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}

		if ( Count == 0 )
		{
			LogFile.OutputLogFileErrorString( L"Error Count == 0", __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}
	#endif

	Release();

	static D3D11_BUFFER_DESC DescBuffer;
	ZeroMemory( &DescBuffer, sizeof(D3D11_BUFFER_DESC) );
	DescBuffer.ByteWidth = StructureByteStride * Count;
	DescBuffer.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_VERTEX_BUFFER;
	DescBuffer.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
	V_RETURN( SystemRedKid.GetDevice()->CreateBuffer( &DescBuffer, NULL, &g_pBuffer ) );

	static D3D11_UNORDERED_ACCESS_VIEW_DESC	BufferDescUAV;
	ZeroMemory( &BufferDescUAV, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC) );
	BufferDescUAV.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	BufferDescUAV.Buffer.FirstElement = 0;
	BufferDescUAV.Format = DXGI_FORMAT_R32_TYPELESS;
	BufferDescUAV.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
	BufferDescUAV.Buffer.NumElements = DescBuffer.ByteWidth / 4;
	V_RETURN( SystemRedKid.GetDevice()->CreateUnorderedAccessView( g_pBuffer, &BufferDescUAV, &g_pBufferUAV ) );

	return S_OK;
}