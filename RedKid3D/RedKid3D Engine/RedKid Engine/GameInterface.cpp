#include "GameInterface.h"

CGameInterface::CGameInterface( eTypeInterface t )
{
	Type = t;
	Texture = NULL;
	fInit = false;

	Text = NULL;
	mTextSize = 0;

	mArraySize = 0;
	aMeshArray = NULL;
	g_pVertexBuffer = NULL;

	Line = 0;
	xPointSize = yPointSize = ySymbSize = 0.0f;

	Pos = D3DXVECTOR3( -1.0f, 1.0f, 0.0f );
}

CGameInterface::~CGameInterface( void )
{
	Release();
}

void CGameInterface::Release( void )
{
	if ( Texture != NULL )
		Texture->DecrementLink();

	DEL_ARRAY( aMeshArray );
	RELEASE( g_pVertexBuffer );
}

HRESULT CGameInterface::Resize( UINT Width, UINT Height )
{
	xPointSize = 2.0f / (float)Width;
	yPointSize = 2.0f / (float)Height;
	ySymbSize = ( (float)Height / (float)Line ) * yPointSize;

	switch ( Type )
	{
	case INTERFACE_TEXT:
		{
			if ( mTextSize > 0 )
				V_RETURN( PrintText() );
			break;
		}
	}

	return S_OK;
}

HRESULT CGameInterface::InitTextInterface( UINT Width, UINT Height, UINT line, TCHAR* TDFName, CResourceTexture* Text )
{
	if ( Type != INTERFACE_TEXT )
		return S_FALSE;

	std::ifstream file;

	try
	{
		file.open( TDFName, std::ios::in );
		if ( !file )
		{
			TCHAR __eRR[1024];
			wsprintf( __eRR, L"No open mesh fntData file: %s.", TDFName );
			LogFile.OutputLogFileErrorString( __eRR, __FILE__, __FUNCTION__, __LINE__ );
			return S_FALSE;
		}
	}
	catch ( ... )
	{
		TCHAR __eRR[1024];
		wsprintf( __eRR, L"Error I/O(open fntData file: %s).", TDFName );
		LogFile.OutputLogFileErrorString( __eRR, __FILE__, __FUNCTION__, __LINE__ );
		return S_FALSE;
	}

	FontData[0] = 0;
	try
	{
		for ( UINT i  = 1; i < 95; i++ )
			file >> FontData[i]; 
	}
	catch( ... )
	{
		file.close();
		TCHAR __eRR[1024];
		wsprintf( __eRR, L"Error I/O(load fntData file: %s).", TDFName );
		LogFile.OutputLogFileErrorString( __eRR, __FILE__, __FUNCTION__, __LINE__ );
		file.close();
		return S_FALSE;
	}

	xPointSize = 2.0f / (float)Width;
	yPointSize = 2.0f / (float)Height;
	Line = line;
	ySymbSize = ( (float)Height / (float)Line ) * yPointSize;

	if ( Texture != NULL )
		Texture->DecrementLink();

	Texture = Text;
	Texture->IncrementLink();

	fInit = true;

	return S_OK;
}

void CGameInterface::Draw( void )
{
	static UINT stride = sizeof( VERTEX_INTERFACE );
	static UINT offset = 0;

	SystemRedKid.GetDiffuseTexture()->SetResource( Texture->GetSRV() );
	SystemRedKid.GetTechImterface()->GetPassByIndex( 0 )->Apply( 0, SystemRedKid.GetDeviceContext() );

	SystemRedKid.GetDeviceContext()->IASetVertexBuffers( 0, 1, &g_pVertexBuffer, &stride, &offset );
	SystemRedKid.GetDeviceContext()->Draw( mTextSize * 6, 0 );
}

HRESULT CGameInterface::SetText( char* t )
{
	if ( Type != INTERFACE_TEXT && !fInit )
		return S_FALSE;

	Text = t;
	mTextSize = strlen( t );

	V_RETURN( PrintText() );

	return S_OK;
}

HRESULT CGameInterface::SetTextLine( UINT line )
{
	if ( Type != INTERFACE_TEXT && !fInit )
		return S_FALSE;

	Pos.y = 1.0f - ySymbSize * line;

	return S_OK;
}

HRESULT CGameInterface::PrintText( void )
{

	static float xTexPSize;
	xTexPSize = 1.0f / Texture->GetWidth();

	// Создаём массив и буффер для хранения сетки
	if ( ( mTextSize ) * 6 > mArraySize )
	{
		DEL_ARRAY( aMeshArray );
		aMeshArray = new VERTEX_INTERFACE[ ( mTextSize ) * 6 ];
		if ( aMeshArray == NULL )
			return S_FALSE;

		mArraySize = ( mTextSize ) * 6;

		RELEASE( g_pVertexBuffer );
		D3D11_BUFFER_DESC bd;
		ZeroMemory( &bd, sizeof(bd) );
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof( VERTEX_INTERFACE ) * mTextSize * 6;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		if ( S_OK != SystemRedKid.GetDevice()->CreateBuffer( &bd, NULL, &g_pVertexBuffer ) )
			return S_FALSE;
	}

	char symb;
	UINT sIndex;
	D3DXVECTOR3 vec3 = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR2 vec2;
	float Relation;
	float xPos = Pos.x;
	for ( UINT i = 0; i< mTextSize; i++ )
	{
		symb = Text[i];

		if ( symb > ' ' && symb <= '~' )
		{
			sIndex = ( symb - 32 );
			// Высчитываем отношение сторон символа в текстуре
			Relation = ySymbSize * ( (float)( FontData[sIndex] - FontData[sIndex - 1] ) / (float)( Texture->GetHeight() ) );

			vec2.x = xTexPSize * FontData[sIndex - 1];
			vec2.y = 0.0f;
			aMeshArray[ (i * 6) ].Tex = vec2;
			vec2.x = xTexPSize * FontData[sIndex];
			vec2.y = 0.0f;
			aMeshArray[ (i * 6) + 1 ].Tex = vec2;
			vec2.x = xTexPSize * FontData[sIndex - 1];
			vec2.y = 1.0f;
			aMeshArray[ (i * 6) + 2 ].Tex = vec2;

			vec2.x = xTexPSize * FontData[sIndex];
			vec2.y = 0.0f;
			aMeshArray[ (i * 6) + 3 ].Tex = vec2;
			vec2.x = xTexPSize * FontData[sIndex];
			vec2.y = 1.0f;
			aMeshArray[ (i * 6) + 4 ].Tex = vec2;
			vec2.x = xTexPSize * FontData[sIndex - 1];
			vec2.y = 1.0f;
			aMeshArray[ (i * 6) + 5 ].Tex = vec2;
		}
		else
		{
			Relation = ySymbSize * 0.25f;

			vec2.x = 0.0f;
			vec2.y = 0.0f;
			aMeshArray[ (i * 6) ].Tex = vec2;
			vec2.x = 0.0f;
			vec2.y = 0.0f;
			aMeshArray[ (i * 6) + 1 ].Tex = vec2;
			vec2.x = 0.0f;
			vec2.y = 0.0f;
			aMeshArray[ (i * 6) + 2 ].Tex = vec2;

			vec2.x = 0.0f;
			vec2.y = 0.0f;
			aMeshArray[ (i * 6) + 3 ].Tex = vec2;
			vec2.x = 0.0f;
			vec2.y = 0.0f;
			aMeshArray[ (i * 6) + 4 ].Tex = vec2;
			vec2.x = 0.0f;
			vec2.y = 0.0f;
			aMeshArray[ (i * 6) + 5 ].Tex = vec2;
		}

		///////////////////////////////////////////////////////////
		vec3.x = xPos;
		vec3.y = Pos.y;
		aMeshArray[ (i * 6) ].Pos = vec3;

		vec3.x = xPos + Relation;
		vec3.y = Pos.y;
		aMeshArray[ (i * 6) + 1 ].Pos = vec3;

		vec3.x = xPos;
		vec3.y = Pos.y - ySymbSize;
		aMeshArray[ (i * 6) + 2 ].Pos = vec3;
		///////////////////////////////////////////////////////////
		vec3.x = xPos + Relation;
		vec3.y = Pos.y;
		aMeshArray[ (i * 6) + 3 ].Pos = vec3;

		vec3.x = xPos + Relation;
		vec3.y = Pos.y - ySymbSize;
		aMeshArray[ (i * 6) + 4 ].Pos = vec3;

		vec3.x = xPos;
		vec3.y = Pos.y - ySymbSize;
		aMeshArray[ (i * 6) + 5 ].Pos = vec3;

		xPos += Relation;
	}

	// Копируем данные массива в буффер
	static D3D11_MAPPED_SUBRESOURCE MappedResource;
	if ( S_OK == SystemRedKid.GetDeviceContext()->Map( g_pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &MappedResource ) )
	{
		memcpy( MappedResource.pData, aMeshArray, sizeof( VERTEX_INTERFACE ) * 6 * ( mTextSize ) );
		SystemRedKid.GetDeviceContext()->Unmap( g_pVertexBuffer, NULL );
	}
	else
		return S_FALSE;

	return S_OK;
}