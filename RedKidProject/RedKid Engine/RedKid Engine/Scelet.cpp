#include "header.h"
#include "Bone.h"
#include "Scelet.h"

CScelet::CScelet(void)
{
	m_NumBones = 0;
	m_RootBone = NULL;
	m_BonesTransform = NULL;
}

CScelet::~CScelet(void)
{
	Release();
}

void CScelet::Release( void )
{
	c_CBScelTransf.Release();
	m_NumBones = 0;
	DEL( m_RootBone );
	DEL_ARRAY( m_BonesTransform );
}

HRESULT CScelet::ReadFromStream( std::ifstream &stream )
{
	std::string txt;
	stream >> txt;
	if ( txt != "Bones_Count:" )
		return S_FALSE;

	stream >> m_NumBones;

	std::string name;
	int id, parentID;
	CBone* bone;
	for ( UINT i = 0; i < m_NumBones; ++i )
	{
		stream >> name >> id >> parentID;

		bone = NULL;
		bone = new CBone( /*wcstring,*/ id, parentID );
		if ( bone == NULL )
			return S_FALSE;
		this->AddBone( bone );
	}

	return BindScelet();
}

void CScelet::AddBone( CBone* bone )
{
	if ( !bone )
		return;

	if ( !m_RootBone )
	{
		m_RootBone = bone;
		return;
	}

	CBone* parent = this->GetBoneByID( bone->GetParentID() );
	if ( parent )
		parent->LinkChildBone( bone );
}

CBone* CScelet::GetBoneByID( int ID )
{
	if ( !m_RootBone )
		return NULL;

	if ( m_RootBone->GetID() == ID )
		return m_RootBone;

	return m_RootBone->FindInChildrensByID( ID );
}

