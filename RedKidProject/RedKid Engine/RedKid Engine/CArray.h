#pragma once

#include "header.h"
#include "LogFile.h"

template <class DataT> class CArray
{
public:
	CArray( void )
	{
		a = NULL;
		count = 0;
		step = 1;
	}
	CArray( UINT Step )
	{
		a = NULL;
		count = 0;
		step = Step;
	}
	~CArray( void )
	{
		DEL_ARRAY( a );
	}

	inline void ReleaseLP( void );

	inline HRESULT Add( DataT Data );
	inline HRESULT Add( DataT Data, UINT &index );
	inline DataT &operator[]( UINT i );
	inline UINT GetCount( void ) { return count; };

private:
	DataT	*a;
	UINT	count;
	UINT	step;
};

// Удаление массива указателей
template <class DataT> void CArray<DataT>::ReleaseLP( void )
{
	for ( UINT i = 0; i < count; i++ )
		if ( a[i] != NULL )
			DEL( a[i] );

	DEL_ARRAY(a);
	count = 0;
}

template <class DataT> DataT &CArray<DataT>::operator[]( UINT i )
{
	#if defined( DEBUG ) || defined( _DEBUG )
		if ( i >= count )
		{
			// Ошибка индексации массива
			LogFile.OutputLogFileErrorString( _T( "Error index CArrayStack." ), __FILE__, __FUNCTION__, __LINE__ );
			exit( -1 );
		}
	#endif

	return a[i];
}

template <class DataT> HRESULT CArray<DataT>::Add( DataT Data )
{
	UINT index = 0;

	while( index < count )
	{
		if ( a[ index ] == NULL )
		{
			a[ index ] = Data;
			return S_OK;
		}
		index++;
	}

	DataT* b = new DataT[ count + step ];
	if ( !b )
		return S_FALSE;

	UINT i;
	for ( i = 0; i < count; i++ )
			b[ i ] = a[ i ];

	for ( i = count; i < count + step; i++ )
		b[ i ]=NULL;

	b[ index ] = Data;

	count += step;
	delete [] a;

	a = b;

	return S_OK;
}

template <class DataT> HRESULT CArray<DataT>::Add( DataT Data, UINT &index )
{
	index = 0;

	while( index < count )
	{
		if ( a[ index ] == NULL )
		{
			a[ index ] = Data;
			return S_OK;
		}
		index++;
	}

	DataT* b = new DataT[ count + step ];
	if ( !b )
		return S_FALSE;

	UINT i;
	for ( i = 0; i < count; i++ )
			b[ i ] = a[ i ];

	for ( i = count; i < count + step; i++ )
		b[ i ] = NULL;

	b[ index ] = Data;

	count += step;
	delete [] a;

	a = b;

	return S_OK;
}

template <class DataT> class CArrayStack
{
public:
	CArrayStack( void )
	{
		a		= NULL;
		count	= 0;
		size	= 0;
		step	= 1;
		cur		= 0;
	}
	CArrayStack( UINT s )
	{
		a		= NULL;
		count	= 0;
		size	= 0;
		step	= s;
		cur		= 0;
	}
	~CArrayStack( void )
	{
		DEL_ARRAY( a );
	}

	void ReleaseLP( void );

	inline UINT GetCount( void ) { return count; };	// Общий размер массива с данными
	inline DataT &operator[]( UINT i );
	HRESULT Add( DataT Data );
	HRESULT Add( DataT Data, UINT &Index );
	void ResetAdd( void )
	{
		size = 0;
		cur = 0;
	}
	bool ResetGet( void )
	{
		if ( size == 0 )
			return false;

		cur = 0;
		return true;
	}
	bool Get( DataT &Data )
	{
		if ( cur < size )
		{
			Data = a[cur];
			cur++;
			return true;
		}

		return false;
	}
	UINT GetSize( void ) { return size; };	// Колличество используемых объектов(вершина стэка)
private:
	DataT	*a;
	UINT	count;
	UINT	size;
	UINT	step;
	UINT	cur;
};

template <class DataT> DataT &CArrayStack<DataT>::operator[]( UINT i )
{
	#if defined( DEBUG ) || defined( _DEBUG )
		if ( i >= count )
		{
			// Ошибка индексации массива
			LogFile.OutputLogFileErrorString( _T( "Error index CArrayStack." ), __FILE__, __FUNCTION__, __LINE__ );
			exit( -1 );
		}
	#endif

	return a[i];
}

template <class DataT> HRESULT CArrayStack<DataT>::Add( DataT Data )
{
	if ( cur >= count )
	{
		DataT* b = new DataT[ count + step ];
		if ( !b )
			return S_FALSE;

		for ( UINT i = 0; i < count; i++ )
				b[ i ] = a[ i ];

		count += step;
		delete [] a;
		a = b;
	}

	a[cur] = Data;
	cur++;
	size++;

	return S_OK;
}

template <class DataT> HRESULT CArrayStack<DataT>::Add( DataT Data, UINT &Index )
{
	if ( cur >= count )
	{
		DataT* b = new DataT[ count + step ];
		if ( !b )
			return S_FALSE;

		for ( UINT i = 0; i < count; i++ )
				b[ i ] = a[ i ];

		count += step;
		delete [] a;
		a = b;
	}

	a[cur] = Data;
	Index = cur;
	cur++;
	size++;

	return S_OK;
}

// Удаление массива указателей
template <class DataT> void CArrayStack<DataT>::ReleaseLP( void )
{
	for ( UINT i = 0; i < count; i++ )
		if ( a[i] != NULL )
			DEL( a[i] );

	DEL_ARRAY(a);
	cur = 0;
	count = 0;
}