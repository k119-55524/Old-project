#pragma once

#include "Bone.h"
#include "LogFile.h"
#include "BufferConstant.h"

class CScelet
{
	friend class CResourceSceletMesh;
	friend class CObjectPhysXSceletMesh;

public:
	CScelet( void );
	~CScelet( void );
	void Release( void );
	HRESULT ReadFromStream( std::ifstream &stream );
	inline UINT GetBoneCount( void ) { return m_NumBones; };
	CBone* GetBoneByID( int ID );
	inline void SetupBindPose( void )
	{
		if ( m_RootBone )
			m_RootBone->SetupBindPose();
	};
	inline void AnimateHierarhy( void )
	{
		if ( m_RootBone )
			m_RootBone->AnimateHierarhy( NULL );
	}
	inline HRESULT SetupCBInGPU( UINT Slot )
	{
		V_RETURN( c_CBScelTransf.of_CPU_in_GPU_Copy( m_BonesTransform ) );
		c_CBScelTransf.CSSetConstantBuffers( Slot );

		return S_OK;
	};

	inline void operator = ( CScelet& s )
	{
		Release();

		m_NumBones = s.GetBoneCount();

		CBone* Bone;
		CBone* sBone;
		for ( UINT i = 0; i < m_NumBones; i++ )
		{
			sBone = s.GetBoneByID( i );

			Bone = NULL;
			Bone = new CBone( i, sBone->GetParentID() );
			if ( Bone == NULL )
			{
				Release();
				throw 1;
			}

			this->AddBone( Bone );
		}

		if ( S_OK != BindScelet() )
		{
			Release();
			throw 1;
		}
	}

protected:
	void AddBone( CBone* bone );
	inline HRESULT BindScelet( void )
	{
		DEL_ARRAY( m_BonesTransform );

		UINT i;

		m_BonesTransform = new BONE_ANIMATION[m_NumBones];
		if ( m_BonesTransform == NULL )
		{
			LogFile.OutputLogFileErrorString( _T( "Memory allocation failure." ), __FILE__, __FUNCTION__, __LINE__  );
			return S_FALSE;
		}

		for ( i = 0; i < m_NumBones; i++ )
		{
			D3DXQuaternionIdentity( &m_BonesTransform[i].Rotations );
			m_BonesTransform[i].Offsets = D3DXVECTOR4( 0.0f, 0.0f, 0.0f, 1.0f );
		}

		CBone* bone;
		for ( i = 0; i < m_NumBones; ++i )
		{
			if ( bone = GetBoneByID( i ) )
				bone->SetTransform( m_BonesTransform + i );
		}

		return c_CBScelTransf.Create( sizeof(BONE_ANIMATION) * m_NumBones );
	};

	UINT m_NumBones;
	CBone*	m_RootBone;

private:
	LPBONE_ANIMATION	m_BonesTransform;
	CBufferConstant		c_CBScelTransf;
};
