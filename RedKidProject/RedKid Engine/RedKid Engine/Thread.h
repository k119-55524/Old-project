#pragma once

#include "header.h"

typedef DWORD (CALLBACK *LPCALLBACKTHREAD)( LPVOID Data );

class CThread
{
public:
	CThread( void );
	~CThread( void );

	HRESULT Create( LPCALLBACKTHREAD f, LPVOID D );
	inline void Start( void ) { fStart = true; };
	inline void WaitStart( void ) { while( fStart ) {}; };

protected:
	DWORD dwID;
	static LPCALLBACKTHREAD func;

	static DWORD WINAPI Thread( LPVOID lpv );
	static bool fRun;
	static bool fStart;
	static LPVOID Data;
};

