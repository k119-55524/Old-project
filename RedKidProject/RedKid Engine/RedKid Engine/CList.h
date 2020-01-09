#pragma once

#include "header.h"
#include "LogFile.h"

template <class DataT> class CListObj
{
public:
	CListObj(void);
	//~CListObj(void);
	inline void GetInfo( DataT &c ) { c=info; };
	inline CListObj<DataT>* GetNext(void) { return next; };
	inline CListObj<DataT>* GetPrior(void) { return prior; };

	CListObj<DataT>*			next;
	CListObj<DataT>*			prior;
	DataT						info;
};

template <class DataT> CListObj<DataT>::CListObj(void)
{
	info		= NULL;
	next		= NULL;
	prior		= NULL;
};

//template <class DataT> CListObj<DataT>::~CListObj(void)
//{
//	DEL( info );
//};

template <class DataT> class CList : public CListObj <DataT>
{

public:
	CList(void);
	~CList(void);
	void ReleaseLP( void );

	HRESULT Add( DataT c );
	void ClearAll(void);
	inline UINT GetSize( void ) { return size; };
	HRESULT FindAndCutObject( DataT val );
	HRESULT CutObject( CListObj<DataT> *p );
	inline bool FindStart( void )
	{
		if ( size == 0 )
			return false;

		findCur = start;

		return true;
	};
	inline bool FindNext( void )
	{
		if ( findCur->next != NULL )
		{
			findCur = findCur->next;
			return true;
		}
		return false;
	};
	inline DataT GetFindObject( void ) { return findCur->info; };
	inline void GetFindObject( DataT &fData ) { fData = findCur->info; };
	//inline void GetFindObject( DataT *fData ) { fData = &(findCur->info); };

protected:
	CListObj<DataT>	*start, *end, *cur, *findCur;
	UINT size;
};

template <class DataT> CList<DataT>::CList(void)
{
	start	= NULL;
	end		= NULL;
	cur		= NULL;
	findCur	= NULL;
	size	= 0;
}

template <class DataT> CList<DataT>::~CList(void)
{
	CListObj<DataT>	*p, *p1;

	p=start;

	while (p)
	{
		p1 = p->next;
		delete(p);
		p=p1;
	}

	start = end = cur = NULL;
	size = 0;
}

template <class DataT> void CList<DataT>::ReleaseLP( void )
{
	CListObj<DataT>	*p, *p1;

	p=start;

	while (p)
	{
		delete p->info;

		p1 = p->next;

		delete(p);
		p=p1;
	}

	start = end = cur = NULL;
	size = 0;
}

template <class DataT> HRESULT CList<DataT>::Add(DataT c)
{
	CListObj<DataT>	*p;

	p = new CListObj<DataT>;
	if (!p)
	{
		LogFile.OutputLogFileErrorString( L"Memory allocation failure.", __FILE__, __FUNCTION__, __LINE__  );
		return S_FALSE;
	}

	p->info = c;
	//p->info->SetPointContayner( p );

	if (size == 0)
		end = start = cur = p;
	else
	{
		p->prior = end;
		end->next = p;
		end = p;
	}

	size++;

	return S_OK;
}

template <class DataT> void CList<DataT>::ClearAll(void)
{
	CListObj<DataT>	*p, *p1;

	p=start;

	while ( p )
	{
		p1 = p->next;
		delete( p );
		p=p1;
	}

	start = end = cur = NULL;
	size = 0;
}

template <class DataT> HRESULT CList<DataT>::FindAndCutObject( DataT val )
{
	CListObj<DataT>	*p;

	p = start;

	while ( p )
	{
		if ( p->info == val )
		{
			if ( size == 1 )
			{
				delete p;
				start = end = cur = findCur = NULL;
				size = 0;
				return S_OK;
			}
			if ( p->next != NULL )
				p->next->prior = p->prior;
			else
			{
				p->prior->next = NULL;
				end = p->prior;
			}
			if ( p->prior != NULL )
				p->prior->next = p->next;
			else
			{
				p->next->prior = NULL;
				start = p->next;
			}

			delete p;
			size--;
			return S_OK;
		}
		p = p->next;
	}

	return S_FALSE;
}

template <class DataT> HRESULT CList<DataT>::CutObject( CListObj<DataT> *p )
{
		if ( size == 1 )
		{
			//delete p;
			start = end = cur = findCur = NULL;
			size = 0;
			return S_OK;
		}
		if ( p->next != NULL )
			p->next->prior = p->prior;
		else
		{
			p->prior->next = NULL;
			end = p->prior;
		}
		if ( p->prior != NULL )
			p->prior->next = p->next;
		else
		{
			p->next->prior = NULL;
			start = p->next;
		}
		// Только удаляем из списка, без удаления объекта
		//delete p;
		size--;
		return S_OK;
}





