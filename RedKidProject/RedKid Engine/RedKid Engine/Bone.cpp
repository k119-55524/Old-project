#include "header.h"
#include "Bone.h"

CBone::CBone( /*LPCTSTR name,*/ int id, int parentID )
{
	//m_Name = name;
	m_ID = id;
	m_ParentID = parentID;

	m_Parent = NULL;
	m_Next = NULL;
	m_Child = NULL;

	m_Transform = NULL;

	D3DXQuaternionIdentity( &m_BindPoseInv.Rotations );
	m_BindPoseInv.Offsets = D3DXVECTOR4( 0.0f, 0.0f, 0.0f, 1.0f );
}

CBone::~CBone(void)
{
	Release();
}

void CBone::Release( void )
{
	m_ID = 0;
	m_ParentID = 0;

	m_Parent = NULL;
	DEL( m_Next );
	DEL( m_Child );
	D3DXQuaternionIdentity( &m_BindPoseInv.Rotations );
	m_BindPoseInv.Offsets = D3DXVECTOR4( 0.0f, 0.0f, 0.0f, 1.0f );
}

void CBone::LinkChildBone( CBone* bone )
{
	CBone* oldChild = m_Child;
	m_Child = bone;
	m_Child->m_Next = oldChild;
	m_Child->m_Parent = this;
}

CBone* CBone::FindInChildrensByID( int ID )
{
	if ( !m_Child )
		return NULL;

	CBone* child = m_Child;
	CBone* found = NULL;
	while ( child )
	{
		if ( child->GetID() == ID )
			return child;

		found = child->FindInChildrensByID( ID );
		if ( found )
			break;
		child = child->GetNext();
	}

	return found;
}
