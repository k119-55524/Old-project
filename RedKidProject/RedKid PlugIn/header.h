#pragma once

#include <Max.h>
#include <istdplug.h>
#include <iparamb2.h>
#include <iparamm2.h>
#include <IGame.h>
#include <IGameModifier.h>

#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>

#include "resource.h"

extern TCHAR *GetString(int id);

#define RM3D_Export_CLASS_ID Class_ID(0x3d2813d0, 0x7f6161c3)

class MeshData
{
public:
	MeshData( void )
	{
		VertexCount = 0;
		BoneCount = 0;
		fTextCoord = FALSE;
		StartSceletFrame = EndSceletFrame = 0;
		Mesh = NULL;
		Node = NULL;
	};
	~MeshData( void )
	{
		Release();
	};
	void Release( void )
	{
		Node->ReleaseIGameObject();
	};

	UINT VertexCount;
	UINT BoneCount;
	BOOL fTextCoord;
	int StartSceletFrame, EndSceletFrame;
	IGameMesh* Mesh;
	IGameNode* Node;
};

class VertexWeight
{
public:

	VertexWeight() : m_Weight(0), m_BoneID(-1) {}
	VertexWeight( float w, int b ) : m_Weight(w), m_BoneID(b) {}

	void SetWeight( float f )
	{
		m_Weight = f;
	}
	void SetBoneID( int i )
	{
		m_BoneID = i;
	}
	float GetWeight( void ) const
	{
		return m_Weight;
	}
	int GetBoneID( void ) const
	{
		return m_BoneID;
	}
	VertexWeight& operator = ( const VertexWeight& other )
	{
		m_Weight = other.m_Weight;
		m_BoneID = other.m_BoneID;
		return (*this);
	}

protected:
	float	m_Weight;
	int		m_BoneID;
};

class MeshVertex
{
public:
	MeshVertex( void ) {}
	MeshVertex( float x, float y, float z )
	{
		m_Pos		= Point3( x, y, z );
		m_Normal	= Point3( 0.0f, 0.0f, 0.0f );
		m_Tangent	= Point3( 0.0f, 0.0f, 0.0f );
		m_Binormal	= Point3( 0.0f, 0.0f, 0.0f );
		m_TexCoord	= Point2( 0.0f, 0.0f );
		m_ID		= -1;
	}
	void SetNormal( float x, float y, float z )
	{
		m_Normal = Point3( x, y, z );
	}
	void SetTangent( float x, float y, float z )
	{
		m_Tangent = Point3( x, y, z );
	}
	const Point3& GetTangent( void )
	{
		return m_Tangent;
	}
	void SetBinormal( float x, float y, float z )
	{
		m_Binormal = Point3( x, y, z );
	}
	const Point3& GetBinormal( void )
	{
		return m_Binormal;
	}
	void SetTexCoord( float u, float v )
	{
		m_TexCoord = Point2( u, v );
	}
	void SetID( int id )
	{
		m_ID = id;
	}
	void AddWeight( const VertexWeight& w )
	{
		m_Weights.push_back( w );
	}
	const Point3& GetPosition( void ) const
	{
		return m_Pos;
	}
	const Point3& GetNormal( void ) const
	{
		return m_Normal;
	}
	const Point2& GetTexCoord( void ) const
	{
		return m_TexCoord;
	}
	int GetID( void ) const
	{
		return m_ID;
	}
	const int GetNumWeights( void ) const
	{
		return (int)m_Weights.size();
	}
	const VertexWeight& GetWeight( int i ) const
	{
		return m_Weights[ i ];
	}
	inline bool operator == ( const MeshVertex& other ) const
	{
		return ( m_Pos == other.m_Pos && m_Normal == other.m_Normal && m_TexCoord == other.m_TexCoord &&
			m_Tangent == other.m_Tangent && m_Binormal == other.m_Binormal && m_ID == other.m_ID );
	}
	MeshVertex& operator = ( const MeshVertex& other )
	{
		m_Pos		= other.m_Pos;
		m_Normal	= other.m_Normal;
		m_Tangent	= other.m_Tangent;
		m_Binormal	= other.m_Binormal;
		m_TexCoord	= other.m_TexCoord;
		m_ID		= other.m_ID;
		m_Weights	= other.m_Weights;

		return (*this);
	}

private:
	typedef	std::vector<VertexWeight>	weightsVec;

	Point3			m_Pos;
	Point3			m_Normal;
	Point3			m_Tangent;
	Point3			m_Binormal;
	Point2			m_TexCoord;
	int				m_ID;
	weightsVec		m_Weights;
};

class MeshVertexList
{
public:
	MeshVertexList( UINT maxcount )
	{
		Count = 0;
		MaxCount = maxcount;

		m_VertexList = new MeshVertex[maxcount];
	}
	~MeshVertexList( void )
	{
		if ( m_VertexList != NULL )
			delete [] m_VertexList;
	}
	inline UINT Add( const MeshVertex& v )
	{
		UINT Index;
		for ( Index = 0; Index < Count; Index++ )
		{
			if ( m_VertexList[Index] == v )
				break;
		}

		if ( Index == Count )
		{
			m_VertexList[Index] = v;
			Count++;
		}

		return Index;
	}
	inline UINT GetCount( void ) const
	{
		return Count;
	}
	inline MeshVertex& operator [] ( int i )
	{
		return m_VertexList[ i ];
	}


private:
	UINT Count, MaxCount;
	MeshVertex* m_VertexList;
};

class AnimFrame
{
public:

	AnimFrame( int numBones )
	{
		m_BonesTransform.resize( numBones );
	}
	void AddBoneTransform( const GMatrix& transform, int boneID )
	{
		m_BonesTransform[boneID] = transform;
	}
	const GMatrix& GetBoneTransform( int i ) const
	{
		return m_BonesTransform[i];
	}
	int GetNumTransform( void ) const
	{
		return (int)m_BonesTransform.size();
	}

private:
	typedef	std::vector<GMatrix>	bonesTransformVec;
	bonesTransformVec		m_BonesTransform;
};
