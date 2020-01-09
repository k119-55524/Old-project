#pragma once

#include "header.h"
#include "LogFile.h"

class CBufferSRV_UAV
{
public:
	CBufferSRV_UAV( void );
	~CBufferSRV_UAV( void );
	void Release( void );

	HRESULT Resize( UINT ByteStride, UINT Count );

	inline ID3D11Buffer* GetBuffer( void ) { return g_pBuffer; };
	inline ID3D11ShaderResourceView* GetBufferSRV( void ) { return g_pBufferSRV; };
	inline ID3D11UnorderedAccessView* GetBufferUAV( void ) { return g_pBufferUAV; };

protected:
	ID3D11Buffer*				g_pBuffer;
	ID3D11ShaderResourceView*   g_pBufferSRV;
	ID3D11UnorderedAccessView*  g_pBufferUAV;
};

