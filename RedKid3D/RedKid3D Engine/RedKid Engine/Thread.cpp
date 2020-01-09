#include "Thread.h"

CThread::CThread(void)
{
	dwID = 0;
}

CThread::~CThread(void)
{
	fRun = false;
}

HRESULT CThread::Create( LPCALLBACKTHREAD f, LPVOID D )
{
	if ( f == NULL )
		return S_FALSE;

	func = f;
	fRun = true;
	Data = D;
	HANDLE hThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)Thread, (LPVOID)this, 0, &dwID );
	if ( hThread == NULL )
		return S_FALSE;

	CloseHandle(hThread);    //Нам не нужен дескриптор

	return S_OK;
}

LPCALLBACKTHREAD CThread::func = NULL;

bool CThread::fRun = false;

bool CThread::fStart = false;

LPVOID CThread::Data = NULL;

DWORD WINAPI CThread::Thread( LPVOID lpv )
{
	DWORD res;
	CThread* This = (CThread*)lpv;

	while ( This->fRun )
	{
		while( !This->fStart && This->fRun ) {}

		if ( !This->fRun )
			break;

		res = This->func( Data );
		This->fStart = false;
	}

	return res;
}
