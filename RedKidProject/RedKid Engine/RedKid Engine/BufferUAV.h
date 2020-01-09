#pragma once

#include "header.h"
#include "LogFile.h"

class CBufferUAV
{
public:
	CBufferUAV(void);
	~CBufferUAV(void);
	void Release( void );

	HRESULT Create( UINT StructureByteStride, UINT Count );
	inline ID3D11Buffer* &GetBuffer( void ) { return g_pBuffer; };
	inline ID3D11UnorderedAccessView* GetBufferUAV( void ) { return g_pBufferUAV; };

private:
	ID3D11Buffer* g_pBuffer;
	ID3D11UnorderedAccessView*  g_pBufferUAV;
};

