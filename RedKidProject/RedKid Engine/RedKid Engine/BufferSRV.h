#pragma once

#include "header.h"
#include "System.h"
#include "LogFile.h"

class CBufferSRV
{
public:
	CBufferSRV(void);
	~CBufferSRV(void);
	void Release( void );

	HRESULT Create( ID3D11Buffer* buff );
	inline ID3D11ShaderResourceView* GetSRV( void ) { return g_pBufferSRV; };

private:
	ID3D11ShaderResourceView*   g_pBufferSRV;
};

