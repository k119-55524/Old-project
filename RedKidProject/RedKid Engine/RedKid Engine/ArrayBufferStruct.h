#pragma once

#include "header.h"
#include "LogFile.h"

#define MaxLight 100

template <class DataT> class CArrayBufferStruct
{
public:
	CArrayBufferStruct( void );
	CArrayBufferStruct( UINT s );
	~CArrayBufferStruct( void );
	void Release( void );

	inline void ResetAdd( void ) { AddCur = 0; Count = 0; };
	HRESULT Add( DataT* &d );
	HRESULT Add( DataT &d );

	inline UINT GetCount( void ) { return Count; };
	inline void CSSetConstantBuffers( UINT slot ) {	SystemRedKid.GetDeviceContext()->CSSetConstantBuffers( slot, 1, &g_pBuffer );	};
	inline void PSSetConstantBuffers( UINT slot ) {	SystemRedKid.GetDeviceContext()->PSSetConstantBuffers( slot, 1, &g_pBuffer );	};

	HRESULT CopyDataOfCPUInGPU( void );

private:
	UINT	Size;	// Реальный размер массива
	UINT	Count;	// Сколько элементов
	UINT	Step;
	UINT	AddCur;
	DataT*	Data;

	ID3D11Buffer*				g_pBuffer;

	HRESULT ResizeBuffer( void );
};

template <class DataT> HRESULT CArrayBufferStruct<DataT>::CopyDataOfCPUInGPU( void )
{
	if ( g_pBuffer == NULL )
		return S_OK;

	static D3D11_MAPPED_SUBRESOURCE MappedResource;

	V_RETURN( SystemRedKid.GetDeviceContext()->Map( g_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource ) );
	memcpy( MappedResource.pData, Data, Count * sizeof( DataT ) );
	SystemRedKid.GetDeviceContext()->Unmap( g_pBuffer, 0 );

	return S_OK;
}

template <class DataT> CArrayBufferStruct<DataT>::CArrayBufferStruct( void )
{
	Size = 0;
	Count = 0;
	Step = 1;
	AddCur = 0;
	Data = NULL;

	g_pBuffer		= NULL;
	//g_pBufferSRV	= NULL;
}

template <class DataT> CArrayBufferStruct<DataT>::CArrayBufferStruct( UINT s )
{
	Size = 0;
	Count = 0;
	Step = s;
	AddCur = 0;
	Data = NULL;

	g_pBuffer		= NULL;
}

template <class DataT> CArrayBufferStruct<DataT>::~CArrayBufferStruct( void )
{
	Release();
}

template <class DataT> void CArrayBufferStruct<DataT>::Release( void )
{
	DEL_ARRAY( Data );
	RELEASE( g_pBuffer );
}

template <class DataT> HRESULT CArrayBufferStruct<DataT>::ResizeBuffer( void )
{
	if ( Size >= MaxLight )
		return S_FALSE;

	DataT* n = NULL;
	n = new DataT[ Size + Step ];
	if ( n == NULL )
	{
		Release();
		LogFile.OutputLogFileErrorString( L"Memory allocation failure.", __FILE__, __FUNCTION__, __LINE__  );
		return S_FALSE;
	}

	for ( UINT i = 0; i < Size; i ++ )
		n[i] = Data[i];

	DEL_ARRAY( Data );
	Data = n;

	Size += Step;

	RELEASE( g_pBuffer );

	UINT byteWidth = sizeof(DataT) * Size;
	if ( byteWidth % 16 > 0 )
			byteWidth = ( ( byteWidth / 16 ) + 1 ) * 16;

	static D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof( D3D11_BUFFER_DESC ) );
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.ByteWidth = byteWidth;
	V_RETURN( SystemRedKid.GetDevice()->CreateBuffer( &bd, NULL, &g_pBuffer ) );

	return S_OK;
}

template <class DataT> HRESULT CArrayBufferStruct<DataT>::Add( DataT* &d )
{
	if ( AddCur >= Size )
		V_RETURN( ResizeBuffer() );

	d = &Data[AddCur];
	AddCur++;
	Count++;

	return S_OK;
}

template <class DataT> HRESULT CArrayBufferStruct<DataT>::Add( DataT &d )
{
	if ( AddCur >= Size )
		V_RETURN( ResizeBuffer() );

	Data[AddCur] = d;
	AddCur++;
	Count++;

	return S_OK;
}