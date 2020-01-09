#include "System.h"
#include "BufferSRV_UAV.h"

CBufferSRV_UAV::CBufferSRV_UAV( void )
{
	g_pBuffer		= NULL;
	g_pBufferSRV	= NULL;
	g_pBufferUAV	= NULL;
}

CBufferSRV_UAV::~CBufferSRV_UAV( void )
{
	Release();
}

void CBufferSRV_UAV::Release( void )
{
	RELEASE( g_pBufferUAV );
	RELEASE( g_pBufferSRV );
	RELEASE( g_pBuffer );
}

HRESULT CBufferSRV_UAV::Resize( UINT ByteStride, UINT Count )
{
	#if defined( DEBUG ) || defined( _DEBUG )
		if ( ByteStride == 0 )
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
	DescBuffer.ByteWidth = ByteStride * Count;
	DescBuffer.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_VERTEX_BUFFER;
	DescBuffer.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
	V_RETURN( SystemRedKid.GetDevice()->CreateBuffer( &DescBuffer, NULL, &g_pBuffer ) );

	static D3D11_SHADER_RESOURCE_VIEW_DESC DescSRV;
	ZeroMemory( &DescSRV, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC) );
	DescSRV.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	DescSRV.Format = DXGI_FORMAT_R32_TYPELESS;
	DescSRV.BufferEx.FirstElement = 0;
	DescSRV.BufferEx.NumElements = DescBuffer.ByteWidth / 4;
	DescSRV.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
	V_RETURN( SystemRedKid.GetDevice()->CreateShaderResourceView( g_pBuffer, &DescSRV, &g_pBufferSRV ) );

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