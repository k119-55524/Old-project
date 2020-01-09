#pragma once

#include "header.h"
#include "LogFile.h"
#include "BufferUAV.h"

class CBufferRead
{
public:
	CBufferRead( void );
	~CBufferRead( void );
	void Release( void );

	HRESULT Create( UINT StructureByteStride, UINT Count );
	void CopyResource( CBufferUAV* inBuf );
	ID3D11Buffer* GetBuffer( void ) { return g_pBuffer; };

private:
	ID3D11Buffer* g_pBuffer;
};

