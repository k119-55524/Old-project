#pragma once

class CBone
{
public:
	CBone( /*LPCTSTR name,*/ int id, int parentID );
	~CBone(void);
	void Release( void );
	inline int GetID( void ) { return m_ID; };
	inline int GetParentID(void) { return m_ParentID; };
	void LinkChildBone( CBone* bone );
	CBone* FindInChildrensByID( int ID );
	inline CBone* GetNext( void ) { return m_Next; };
	inline void SetTransform( LPBONE_ANIMATION transform ) { m_Transform = transform; };
	inline LPBONE_ANIMATION BoneTransformInverse( LPBONE_ANIMATION bt )
	{
		LPBONE_ANIMATION bti = new BONE_ANIMATION;

		D3DXQuaternionInverse( &bti->Rotations, &bt->Rotations);
		D3DXQuaternionNormalize( &bti->Rotations, &bti->Rotations );
		bti->Offsets = -bt->Offsets;

		return bti;
	};
	inline void SetupBindPose( void )
	{
		if ( m_Transform )
			this->SetBindPoseInv( *BoneTransformInverse( m_Transform ) );

		CBone* child = m_Child;
		while ( child )
		{
			child->SetupBindPose();
			child = child->GetNext();
		}
	};
	inline void SetBindPoseInv( const BONE_ANIMATION& m ) {	m_BindPoseInv = m; };
	inline void AnimateHierarhy( const LPBONE_ANIMATION parentTransform )
	{
		if ( parentTransform )
		{
			(*m_Transform) = Mod( *m_Transform, *parentTransform );
		}

		CBone* child = m_Child;
		while ( child )
		{
			child->AnimateHierarhy( m_Transform );
			child = child->GetNext();
		}

		(*m_Transform) = Mod2( m_BindPoseInv, *m_Transform );
	};

protected:
	//LPCTSTR	m_Name;
	int		m_ID;
	int		m_ParentID;
	CBone*	m_Parent;
	CBone*	m_Next;
	CBone*	m_Child;
	LPBONE_ANIMATION m_Transform;
	BONE_ANIMATION m_BindPoseInv;

	inline BONE_ANIMATION Mod( BONE_ANIMATION& in, BONE_ANIMATION& t )
	{
		BONE_ANIMATION	ret;
		ret.Rotations = in.Rotations * t.Rotations;
		ret.Offsets = MulQuatVec4( t.Rotations, in.Offsets ) + t.Offsets;

		return ret;
	};
	inline BONE_ANIMATION Mod2( BONE_ANIMATION& in, BONE_ANIMATION& t )
	{
		BONE_ANIMATION	ret;
		ret.Rotations = in.Rotations * t.Rotations;
		ret.Offsets = MulQuatVec4( ret.Rotations, in.Offsets ) + t.Offsets;

		return ret;
	};
	inline D3DXVECTOR4 MulQuatVec4( D3DXQUATERNION	Rotations, D3DXVECTOR4& Offsets )
	{
		// original code from DooM 3 SDK
		float xxzz = Rotations.x*Rotations.x - Rotations.z*Rotations.z;
		float wwyy = Rotations.w*Rotations.w - Rotations.y*Rotations.y;

		float xw2 = Rotations.x*Rotations.w*2.0f;
		float xy2 = Rotations.x*Rotations.y*2.0f;
		float xz2 = Rotations.x*Rotations.z*2.0f;
		float yw2 = Rotations.y*Rotations.w*2.0f;
		float yz2 = Rotations.y*Rotations.z*2.0f;
		float zw2 = Rotations.z*Rotations.w*2.0f;

		return D3DXVECTOR4(
			(xxzz + wwyy)*Offsets.x	+ (xy2 + zw2)*Offsets.y		+ (xz2 - yw2)*Offsets.z,
			(xy2 - zw2)*Offsets.x	+ (Rotations.y*Rotations.y+Rotations.w*Rotations.w-Rotations.x*Rotations.x-Rotations.z*Rotations.z)*Offsets.y	+ (yz2 + xw2)*Offsets.z,
			(xz2 + yw2)*Offsets.x	+ (yz2 - xw2)*Offsets.y		+ (wwyy - xxzz)*Offsets.z, 1.0f	);
	};
	inline D3DXVECTOR3 MulQuatVec3( D3DXQUATERNION	Rotations, D3DXVECTOR3& Offsets )
	{
		// original code from DooM 3 SDK
		float xxzz = Rotations.x*Rotations.x - Rotations.z*Rotations.z;
		float wwyy = Rotations.w*Rotations.w - Rotations.y*Rotations.y;

		float xw2 = Rotations.x*Rotations.w*2.0f;
		float xy2 = Rotations.x*Rotations.y*2.0f;
		float xz2 = Rotations.x*Rotations.z*2.0f;
		float yw2 = Rotations.y*Rotations.w*2.0f;
		float yz2 = Rotations.y*Rotations.z*2.0f;
		float zw2 = Rotations.z*Rotations.w*2.0f;

		return D3DXVECTOR3(
			(xxzz + wwyy)*Offsets.x	+ (xy2 + zw2)*Offsets.y		+ (xz2 - yw2)*Offsets.z,
			(xy2 - zw2)*Offsets.x	+ (Rotations.y*Rotations.y+Rotations.w*Rotations.w-Rotations.x*Rotations.x-Rotations.z*Rotations.z)*Offsets.y	+ (yz2 + xw2)*Offsets.z,
			(xz2 + yw2)*Offsets.x	+ (yz2 - xw2)*Offsets.y		+ (wwyy - xxzz)*Offsets.z	);
	};
};
