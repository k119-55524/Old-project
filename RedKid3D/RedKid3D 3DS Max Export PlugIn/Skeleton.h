#pragma once

#include "header.h"
#include "WndStaticText.h"

extern CWndStaticText cwNameText;
extern CWndStaticText cwFaceText;

class Bone
{
public:
	Bone();
	virtual ~Bone();

	void SetMaxNode( IGameNode* maxNode );
	void SetMaxBoneID( int ID );
	void SetName( const std::string& name );
	void SetID( int ID );
	void SetParentID( int parentID );

	IGameNode*         GetMaxNode( void ) const;
	int                GetMaxBoneID( void ) const;
	const std::string& GetName( void ) const;
	int                GetID( void ) const;
	int                GetParentID( void ) const;

	void  LinkChildBone( Bone* childBone );
	Bone* GetParent( void ) const;
	Bone* GetChild( void ) const;
	Bone* GetNext( void ) const;

protected:

	IGameNode*		m_MaxNode;
	int				m_MaxBoneID;
	std::string		m_Name;
	int				m_ID;
	int				m_ParentID;

	Bone*			m_Parent;
	Bone*			m_Next;
	Bone*			m_Child;
};

class Skeleton
{
public:
	Skeleton() {}
	virtual ~Skeleton() {}

	int   Build( IGameObject* obj );
	int   GetNumBones( void ) const;
	Bone* GetRawBone( int i ) const;
	Bone* GetRootBone( void ) const;
	int   GetBoneIDByMaxID( int maxBoneID ) const;

	void WriteFile( std::ofstream& ExpFile );

protected:

	void  AddMaxBone( IGameNode* boneNode, int maxBoneID );
	Bone* FindBoneByMaxID( int maxBoneID ) const;
	void  LinkBones( void );
	void  LinkBone( Bone* bone );

	void WriteBone( std::ofstream& ExpFile, Bone* bone );
	typedef std::vector<Bone*>	bonesVec;

	bonesVec		m_Bones;
	Bone*			m_RootBone;
};

extern BOOL fExp;

class AnimTrack
{
public:
	BOOL Build( IGameObject* obj, Skeleton* skeleton, int animStart, int animEnd )
	{
		int i, j, time;

		const int numAnimFrames = ( animEnd - animStart ) + 1;
		const int numBones = skeleton->GetNumBones();

		IGameNode* node;
		Bone* bone, * parent;

		m_AnimFrames.reserve( numAnimFrames );
		for ( i = 0; i < numAnimFrames; ++i )
		{
			time = i * GetTicksPerFrame();
			AnimFrame frame( numBones );

			for ( j = 0; j < numBones; ++j )
			{
				bone = skeleton->GetRawBone( j );
				node = bone->GetMaxNode();
				GMatrix transform = node->GetWorldTM( time );

				if ( bone->GetParentID() != -1 )
				{
					parent = bone->GetParent();
					node = parent->GetMaxNode();
					GMatrix parentTM = node->GetWorldTM( time );
					if ( -1 == parentTM.Parity() )
					{
						parentTM = parentTM.Inverse();
						GMatrix m = parentTM;
						memset( &parentTM, 0, sizeof( parentTM ) );
						parentTM -= m;
					}
					else
						parentTM = parentTM.Inverse();

					transform *= parentTM;
				}

				frame.AddBoneTransform( transform, bone->GetID() );
			}

			m_AnimFrames.push_back( frame );
		}

		return TRUE;
	}

	BOOL WriteFile( std::ofstream& ExpFile, Skeleton* skeleton, HWND DlgExpProcess )
	{
		TCHAR Text[128];

		cwNameText.SetText( "Out scelet animation." );
		sprintf( Text, "%d frames.", this->GetNumFrames() );
		cwFaceText.SetText( Text );

		int i, j;

		ExpFile << "[Start_Scelet_Animation_Block]\n";
		ExpFile << "NUM_BONES:\n";
		ExpFile << skeleton->GetNumBones() << "\n";
		ExpFile << "PER_FRAME:\n";
		ExpFile << GetFrameRate() << "\n";
		ExpFile << "FRAME:\n";
		ExpFile << this->GetNumFrames() << std::endl;
		MSG msg;
		for ( i = 0; i < this->GetNumFrames(); ++i )
		{
			//////////////////////////////////////////////////////////////////
			if( PeekMessage( &msg, DlgExpProcess, 0, 0, PM_REMOVE ) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}

			if ( !fExp )
				return S_FALSE;

			//////////////////////////////////////////////////////////////////

			const AnimFrame& frame = this->GetAnimFrame( i );
			for ( j = 0; j < frame.GetNumTransform(); ++j )
			{
				const GMatrix& mat = frame.GetBoneTransform( j );
				Point3 offset = mat.Translation();
				Quat rotation = mat.Rotation();

				ExpFile << offset.x << " " << offset.y << " " << offset.z << std::endl;
				ExpFile << rotation.x << " " << rotation.y << " " << rotation.z << " " << rotation.w << std::endl;
			}
		}

		ExpFile << "[End_Scelet_Animation_Block]\n";

		return S_OK;
	}
	int GetNumFrames( void ) const
	{
		return (int)m_AnimFrames.size();
	}
	const AnimFrame& GetAnimFrame( int i ) const
	{
		return m_AnimFrames[i];
	}

protected:
	typedef std::vector<AnimFrame>	framesVec;
	framesVec		m_AnimFrames;
};
