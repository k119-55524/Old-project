#pragma once

#include "header.h"
#include "LogFile.h"

class CBufferConstant
{
public:
	CBufferConstant(void);
	~CBufferConstant(void);
	void Release( void );

	HRESULT Create( UINT byteWidth );
	void CSSetConstantBuffers( UINT slot );
	void PSSetConstantBuffers( UINT slot );
	HRESULT of_CPU_in_GPU_Copy( void* CPUData );
	HRESULT of_CPU_in_GPU_Copy( void* CPUData, UINT count );

private:
	ID3D11Buffer*	g_pBuffer;
	UINT ByteWidth;
	UINT RealByteWidth;
};

