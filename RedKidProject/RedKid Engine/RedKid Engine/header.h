#pragma once

#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN		// ��������� ����� ������������ ���������� �� ���������� Windows

#pragma warning(disable : 4996)
#pragma warning(disable : 4949)
#pragma warning(disable : 4265)

#ifndef WINVER                  // Minimum platform is Windows 7
#define WINVER 0x0601
#endif

#ifndef _WIN32_WINNT            // Minimum platform is Windows 7
#define _WIN32_WINNT 0x0601
#endif

#ifndef _WIN32_WINDOWS          // Minimum platform is Windows 7
#define _WIN32_WINDOWS 0x0601
#endif

#define DIRECTINPUT_VERSION 0x0800

// ����� ���������� Windows:
#include <windows.h>

// ����� ���������� C RunTime
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <assert.h>

#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

#include <D3D11.h>
#include <D3DX11tex.h>
#include <D3D11SDKLayers.h>
#include <D3DX10Math.h>
#include <D3dx11effect.h>
#include <dxerr.h>
#include <DXGI.h>
#include <dinput.h>

//////////////////////////////////////////////////////////////////////
// PhysX 3.1
#include <PxPhysX.h>
#include "PxTkStream.h"
#include <PhysXSample.h>
#include <PxPhysicsAPI.h>
#include <PxErrorCallback.h>
#include <SamplePreprocessor.h>
#include <PxAllocatorCallback.h>
#include <PhysXSampleApplication.h>

using namespace physx;
using namespace pxtask;
//using namespace physx::pubfnd3;
using namespace PxToolkit;

//
//////////////////////////////////////////////////////////////////////

extern class CLogFile LogFile;
extern class CSystem SystemRedKid;
extern class CRedKid RedKid;
extern HRESULT res;
extern float GetFloatRGB( UINT frgb );

class CObjectBase;
class CFrustumBox;

typedef HRESULT (CALLBACK *LPCALLBACKINICIALISE)( void );
typedef void (CALLBACK *LPCALLBACKPREFRAMERENDER)( float deltaTime );

const TCHAR WndClassName[] = _T( "RedMax3D.com. RedKid 3D engine." );
const string LogFileName = "RedKidEngine.log";

// ������ ������� �������� ���������
#define FORMAT_RENDER_BUFFER DXGI_FORMAT_R8G8B8A8_UNORM

// ������ �������� HDR ������� ���������
#define FORMAT_HDR_RENDER_BUFFER DXGI_FORMAT_R32G32B32A32_FLOAT
//#define FORMAT_HDR_RENDER_BUFFER DXGI_FORMAT_R8G8B8A8_UNORM

// ������ ������� ������� �������
#define FORMAT_DEPTH_BUFFER DXGI_FORMAT_R32_TYPELESS
//#define FORMAT_DEPTH_BUFFER DXGI_FORMAT_D32_FLOAT

// ������ ������� ������� ���  Bloom-�
#define FORMAT_BLOOM_BUFFER DXGI_FORMAT_R8G8B8A8_UNORM
//#define FORMAT_BLOOM_BUFFER DXGI_FORMAT_R32G32B32A32_FLOAT

#define RELEASE(p)	 { if (p) { p->Release(); p = NULL; } };
#define rELEASE(p)	 { if (p) { p->release(); p = NULL; } };
#define DEL(p)		 { if (p) { delete p; p = NULL; } };
#define DEL_ARRAY(p) { if (p) { delete [] p; p = NULL; } };
#define V_(x)		 { if( S_OK != ( x ) ) { LogFile.OutputLogFileErrorString( L"Error!!!", __FILE__, __FUNCTION__, __LINE__ ); } }
#define V_RETURN(x)	 { res = (x); if( S_OK != res ) { LogFile.OutputLogFileErrorString( L"Error!!!", __FILE__, __FUNCTION__, __LINE__ ); return res; } }
#define V_RELEASE(x) { res = (x); if( S_OK != res ) { LogFile.OutputLogFileErrorString( L"Error!!!", __FILE__, __FUNCTION__, __LINE__ ); Release(); return res; } }
#define F_COLOR( x ) ( 1.0f / 255.0f ) * x

// ��������� �������
enum eResourceCondition : unsigned int
{
	COND_CREATE = 0,	// ������ ��������� ������ � �� �������� ��������
	COND_DEL_NULL_LINK,	// ������� ��� ���������� ������ �� ������
};

// ��� �������
enum eTypeMaterial : unsigned int
{
	MAT_TEXTURE = 0,
};

// ��� ������
typedef enum eTypeCamera
{
	LANDOBJECT,
	AIRCRAFT
};

// ��� ������� � �����
typedef enum eTypeObject : unsigned int
{
	OBJ_PIVOT = 0,		// ������� �����
	OBJ_MESH,			// C����
	OBJ_LIGHTPOINT,		// �������� �������� �����
	OBJ_LIGHTDIRECTION	// ������������ �������� �����
};

// ��� �������
typedef enum eTypeResource : unsigned int
{
	RES_MATERIAL = 0,
	RES_MESH,
	RES_SCELET_MESH,
	RES_SCELET_ANIM,
	RES_TEXTURE,
	RES_SOUND,

	RES_PHYSX_TRIANGLE_STATIC_MESH,
	RES_PHYSX_CONVEX_DINAMIC_MESH,
};

// ��� ������������� �������
typedef enum eTypeInterface : unsigned int
{
	INTERFACE_TEXT = 0,
};

// ������������ ���������� ������ �������� �� �����
#define MAX_WEIGHTS	4

// ������� �������� ��� �����
typedef struct BONE_ANIMATION
{
	D3DXQUATERNION	Rotations;
	D3DXVECTOR4		Offsets;
} BONE_ANIMATION, *LPBONE_ANIMATION;

// ��� ����� � � �����
typedef struct VERTEXWEIGHT
{
	UINT BoneID;
	float Weigth;
} VERTEXWEIGHT, *LPVERTEXWEIGHT;

// ��������� ���������� ������ ������� � �������� � ���������
typedef struct VERTEX_INTERFACE
{
	D3DXVECTOR3 Pos;
	D3DXVECTOR2 Tex;
} VERTEX_INTERFACE, *LPVERTEX_INTERFACE;

// ��������� ���������� ������ ������� � �������� � ���������
typedef struct VERTEX_QUAD
{
	D3DXVECTOR3 Pos;
	D3DXVECTOR2 Tex;
} VERTEX_QUAD, *LPVERTEX_QUAD;

// ��������� ���������� ������ ������� � �������� � ���������
typedef struct VERTEX_LINE
{
	D3DXVECTOR3 Pos;
	D3DXVECTOR4 Col;
} VERTEX_LINE, *LPVERTEX_LINE;

// ��������� ���������� ������ ������� � ��������
typedef struct VERTEX_NORMAL_UV
{
	D3DXVECTOR3	Pos;
	D3DXVECTOR3 Normal;
	D3DXVECTOR2 UV;
} VERTEX_NORMAL_UV, *LPVERTEX_NORMAL_UV;

// ��������� ���������� ������ ������� � ��������, ���������� � ���������
typedef struct VERTEX_TBN_UV
{
	D3DXVECTOR3	Pos;
	D3DXVECTOR3 Normal;
	D3DXVECTOR3 Tangent;
	D3DXVECTOR3 BiNormal;
	D3DXVECTOR2 UV;
} VERTEX_TBN_UV, *LPVERTEX_TBN_UV;

// ��������� ���������� ������ ������� � �������� � ������ ������
typedef struct VERTEX_NORMAL_UV_BONE
{
	D3DXVECTOR3		Pos;
	D3DXVECTOR3		Normal;
	D3DXVECTOR2		UV;
	UINT			NumWeigth;
	VERTEXWEIGHT	Weigth[MAX_WEIGHTS];
} VERTEX_NORMAL_UV_BONE, *LPVERTEX_NORMAL_UV_BONE;

// ��������� ���������� ������ ������� � ��������, ���������� � ��������� � ������ ������
typedef struct VERTEX_TBN_UV_BONE
{
	D3DXVECTOR3		Pos;
	D3DXVECTOR3		Normal;
	D3DXVECTOR3		Tangent;
	D3DXVECTOR3		BiNormal;
	D3DXVECTOR2		UV;
	UINT			NumWeigth;
	VERTEXWEIGHT	Weigth[MAX_WEIGHTS];
} VERTEX_TBN_UV_BONE, *LPVERTEX_TBN_UV_BONE;

typedef struct _declspec(align(16)) LIGHT_DIRECTION
{
	D3DXVECTOR4	Color;
	D3DXVECTOR4	Direction;
} LIGHT_DIRECTION, *LPLIGHT_DIRECTION;

typedef struct _declspec(align(16)) LIGHT_POINT
{
	D3DXVECTOR3 Position;
	float		Radius;
	D3DXVECTOR4	Color;
} LIGHT_POINT, *LPLIGHT_POINT;

D3DXQUATERNION operator * ( D3DXQUATERNION& qIn, D3DXQUATERNION& q );

typedef class CMeshFormat
{
	friend class CMeshFormat;

public:
	CMeshFormat( void )
	{
		fNormal		= false;
		fTangBinor	= false;
		fUV			= false;
		fSceletData	= false;
	}
	inline void Release( void )
	{
		fNormal		= false;
		fTangBinor	= false;
		fUV			= false;
		fSceletData	= false;
	}

	void SetfNormal( bool f = true ) { fNormal = f; };
	void SetfTangBinor( bool f = true ) { fTangBinor = f; };
	void SetfUV( bool f = true ) { fUV = f; };
	void SetfSceletData( bool f = true ) { fSceletData = f; };

	bool GetfNormal( void ) { return fNormal; };
	bool GetfTangBinor( void ) { return fTangBinor; };
	bool GetfUV( void ) { return fUV; };
	bool GetfSceletData( void ) { return fSceletData; };

	inline void operator = ( const CMeshFormat& other )
	{
		fNormal = other.fNormal;
		fTangBinor = other.fTangBinor;
		fUV= other.fUV;
		fSceletData = other.fSceletData;
	}
	inline bool operator == ( const CMeshFormat& other ) const
	{
		return ( fNormal == other.fNormal && fTangBinor == other.fTangBinor && fUV == other.fUV && fSceletData == other.fSceletData );
	}
	inline bool operator != ( const CMeshFormat& other ) const
	{
		return ( fNormal != other.fNormal || fTangBinor != other.fTangBinor || fUV != other.fUV || fSceletData != other.fSceletData );
	}
private:
	bool fNormal;
	bool fTangBinor;
	bool fUV;
	bool fSceletData;
} MESHFORMAT, *LPMESHFORMAT;

PX_FORCE_INLINE PxSimulationFilterShader GetSampleFilterShader()
{
	return PxDefaultSimulationFilterShader; 
}