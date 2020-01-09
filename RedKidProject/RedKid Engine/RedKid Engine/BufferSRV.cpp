#include "BufferSRV.h"

CBufferSRV::CBufferSRV(void)
{
	g_pBufferSRV	= NULL;
}

CBufferSRV::~CBufferSRV(void)
{
	Release();
}

void CBufferSRV::Release( void )
{
	RELEASE( g_pBufferSRV );
}

HRESULT CBufferSRV::Create( ID3D11Buffer* buff )
{
	static HRESULT res;

	#if defined( DEBUG ) || defined( _DEBUG )
		if ( buff == NULL )
		{
			LogFile.OutputLogFileErrorString( L"NULL buff.", __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}
	#endif

	Release();

	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
	buff->GetDesc( &bd );

	static D3D11_SHADER_RESOURCE_VIEW_DESC DescSRV;
	ZeroMemory( &DescSRV, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC) );
	DescSRV.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	DescSRV.Format = DXGI_FORMAT_R32_TYPELESS;
	DescSRV.BufferEx.FirstElement = 0;
	DescSRV.BufferEx.NumElements = bd.ByteWidth / 4;
	DescSRV.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
	V_RETURN( SystemRedKid.GetDevice()->CreateShaderResourceView( buff, &DescSRV, &g_pBufferSRV ) );

	return S_OK;
}