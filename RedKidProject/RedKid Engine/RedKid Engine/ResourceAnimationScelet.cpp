
#include "ResourceAnimationScelet.h"

CResourceAnimationScelet::CResourceAnimationScelet( void )
{
	Type = RES_SCELET_ANIM;

	AnimationFrames = NULL;
	cPerFrame = 0;
}

CResourceAnimationScelet::~CResourceAnimationScelet(void)
{
	Release();
}

void CResourceAnimationScelet::Release(void)
{
	Name = NULL;
	NumBone = NumFrame = 0;
	cPerFrame = 0;

	if ( AnimationFrames != NULL )
	{
		for ( UINT i = 0; i < NumFrame; i++ )
		{
			delete &AnimationFrames[i];
		}
		AnimationFrames = NULL;
	}
}

HRESULT CResourceAnimationScelet::CreateAnimationOfTXTFile( TCHAR* FileName, UINT nB )
{
	TCHAR mESS[100];
	wsprintf( mESS, _T( "Start CreateAnimationOfTXTFile: %s." ), FileName );
	LogFile.OutputLogFileString( mESS );

	std::ifstream file;
	UINT cBone = 0;
	UINT cFrame = 0;

	try
	{
		Release();
		file.open( FileName, std::ios::in );
		if ( !file )
		{
			LogFile.OutputLogFileErrorString( _T( "Error open file animation." ), __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}

		// Сравниваем заголовок
		std::string txt;

		file >> txt;
		while ( txt != "[Start_Scelet_Animation_Block]" )
		{
			file >> txt;
			if ( file == "" )
			{
				file.close();
				wsprintf( mESS, _T( "Error end file: %s.\nNo create scelet animation." ), FileName );
				LogFile.OutputLogFileString( mESS );
				return S_FALSE;
			}
		}

		file >> txt;
		if ( txt != "NUM_BONES:" )
		{
			LogFile.OutputLogFileErrorString( _T( "Not the correct data in a file." ), __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}

		file >> cBone;
		if ( cBone != nB )
		{
			file.close();
			wsprintf( mESS, _T( "Different quantity of bones(file: %s)." ), FileName );
			LogFile.OutputLogFileString( mESS );
			return S_FALSE;
		}

		file >> txt;
		if ( txt != "PER_FRAME:" )
		{
			LogFile.OutputLogFileErrorString( _T( "Not the correct data in a file." ), __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}
		file >> cPerFrame;

		file >> txt;
		if ( txt != "FRAME:" )
		{
			LogFile.OutputLogFileErrorString( _T( "Not the correct data in a file." ), __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}
		file >> cFrame;
		if ( cBone == 0 || cFrame == 0 )
		{
			file.close();
			LogFile.OutputLogFileErrorString( _T( "Not the correct data in a file." ), __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}

		// Выделяем память под кадры
		AnimationFrames = new CAnimationFrame[cFrame];
		if ( AnimationFrames == NULL )
		{
			Release();
			file.close();
			LogFile.OutputLogFileErrorString( _T( "Error at memory allocation." ), __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}

		// Читаем кадры
		CAnimationFrame* anim = NULL;
		for ( UINT i = 0; i < cFrame; i++ )
		{
			anim = new CAnimationFrame();
			if ( anim == NULL )
			{
				Release();
				file.close();
				LogFile.OutputLogFileErrorString( _T( "Error at memory allocation." ), __FILE__, __FUNCTION__, __LINE__ );
				return S_FALSE;
			}
			if ( S_OK != anim->LoadFrameInTextFile( file, cBone ) )
			{
				Release();
				file.close();
				LogFile.OutputLogFileErrorString( _T( "Error load frame animation." ), __FILE__, __FUNCTION__, __LINE__ );
				return S_FALSE;
			}
			AnimationFrames[i] = *anim;
		}
	}
	catch ( ... )
	{
		Release();
		file.close();
		wsprintf( mESS, _T( "Unknown error CreateAnimationOfTXTFile: %s." ), FileName );
		LogFile.OutputLogFileString( mESS );
		return S_FALSE;
	}

	Name = FileName;
	NumBone = cBone;
	NumFrame = cFrame;

	file.close();

	wsprintf( mESS, _T( "End CreateAnimationOfTXTFile: %s." ), FileName );
	LogFile.OutputLogFileString( mESS );

	return S_OK;
}

