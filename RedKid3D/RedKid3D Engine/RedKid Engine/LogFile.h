#pragma once

#include "header.h"

class CLogFile
{
public:
	inline void ClearLogFile( void )
	{
		std::ofstream LogFile;

		try
		{
			// —брасываем лог файл
			LogFile.open( LogFileName, std::ios::out | std::ios::trunc );
			LogFile << "Log file.\n";
			LogFile.close();
		}
		catch( ... )
		{
			LogFile.close();
		}
	};
	inline void OutputLogFileErrorString( const wstring &ErrStr, char* file, char* func, UINT line )
	{
		std::wofstream LogFile;

		TCHAR* wFile = GetUniStr( file );
		TCHAR* wFunc = GetUniStr( func );

		try
		{
			#if defined( DEBUG ) || defined( _DEBUG )
				TCHAR __eRR[512];
				wsprintf( __eRR, L"%s\n\nFile: %s.\n\nLine: %d, function: %s.\n", ErrStr.c_str(), wFile, line, wFunc );
				OutputDebugString( __eRR );
				MessageBox( NULL, __eRR, L"Error!", MB_OK | MB_ICONSTOP | MB_TASKMODAL );
			#endif

			if ( !fOutErrorMessage )
				return;

			LogFile.open( LogFileName, std::ios::out | std::ios::app );
			LogFile << "ERROR: File - " << wFile << ".\n";
			LogFile << "  - Error line: " << line << ". Func: " << wFunc << ".\n";
			LogFile << "  - Error text: " << ErrStr;
			LogFile << "\n";
			LogFile.close();
		}
		catch( ... )
		{
			LogFile.close();		
		}
	};
	inline void OutputLogFileString( const wstring &Str )
	{
		if ( !fOutMessage )
			return;

		std::wofstream LogFile;
		try
		{
			LogFile.open( LogFileName, std::ios::out | std::ios::app );
			LogFile << "Message: " << Str << "\n";
			LogFile.close();
			#if defined( DEBUG ) || defined( _DEBUG )
				OutputDebugString( ( L"-->" + Str + L"\n" ).c_str() );
			#endif
		}
		catch( ... )
		{
			LogFile.close();		
		}
	}

	inline void SetOutMessageFlag( bool f ) { fOutMessage = f; };
	inline void SetOutErrorMessageFlag( bool f ) { fOutErrorMessage = f; };

private:
	TCHAR* GetUniStr( char* str )
	{
		TCHAR* wcstring;
		size_t newsize = strlen( str ) + 1;
		wcstring = new TCHAR[newsize];
		size_t convertedChars = 0;
		mbstowcs_s( &convertedChars, wcstring, newsize, str, _TRUNCATE );
		return wcstring;
	};

	static bool fOutMessage;
	static bool fOutErrorMessage;
};

