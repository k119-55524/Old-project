#pragma once

#include "AABB.h"
#include "header.h"
#include "Plane.h"
#include "FrustumBox.h"

class CCamera
{
public:
	CCamera( void );
	CCamera( eTypeCamera camerDataT );
	~CCamera( void );

    void Strafe( float units );	// влево/вправо
    inline void Fly( float units ) // вверх/вниз
	{
		if( Type == AIRCRAFT )
			pos += up * units;
	};
    void Walk( float units );	// вперед/назад
    void Pitch(float angle);	// вращение относительно правого вектора
    void Yaw(float angle);		// вращение относительно верхнего вектора
    void Roll(float angle);		// вращение относительно вектора взгляда
    inline D3DXMATRIX* GetViewMatrix( void )					// Возвращает матрицу вида
	{
		CreateViewMatrix();

		// Чтобы не заваливалась камера
		// Мой код
		right.y = 0.0f;

		return V;
	}
	//inline D3DXMATRIX* GetWorldMatrix( void )
	//{
	//	static D3DXMATRIX g_World;

	//	D3DXMatrixRotationYawPitchRoll( &g_World, CamData.mYaw, CamData.mPitch, CamData.mRoll );

	//	return &g_World;
	//};
	//inline D3DXVECTOR4* GetPos( void )
	//{
	//	static D3DXVECTOR4 Pos;

	//	Pos = D3DXVECTOR4( pos.x, pos.y, pos.z, 10.0f );
	//	return &Pos;
	//};
    void SetCamerDataT( eTypeCamera camerDataT )	// Устанавливает тип камеры
	{
		Type = camerDataT;
	};
	inline void SetPosition( float x,float y,float z ){ pos.x = x; pos.y = y; pos.z = z; };
	//inline D3DXVECTOR3* GetPosition( void ){ return &CamData.pos;};
	//inline D3DXVECTOR3* GetRight( void ){ return &CamData.right; }; 
	//inline void SetRight( D3DXVECTOR3* right ){ CamData.right = *right; };
	//inline D3DXVECTOR3* GetUp( void ){ return &CamData.up; };
	//inline void SetUp( D3DXVECTOR3* up ){ CamData.up = *up; };
	//inline D3DXVECTOR3* GetLook( void ){ return &CamData.look; };
	//inline void SetLook( D3DXVECTOR3* look ){ CamData.look = *look; };

	// Извлекаем плоскости матрицы
	inline void InicialiseFrustumPlane( D3DXMATRIX *ProjMat )
	{
		D3DXMATRIX matComb;
		D3DXMatrixMultiply( &matComb, GetViewMatrix(), ProjMat );

		frustumPlanes[0].Normal.x = matComb._14 + matComb._11;
		frustumPlanes[0].Normal.y = matComb._24 + matComb._21;
		frustumPlanes[0].Normal.z = matComb._34 + matComb._31;
		frustumPlanes[0].Distance = matComb._44 + matComb._41;
		frustumPlanes[1].Normal.x = matComb._14 - matComb._11;
		frustumPlanes[1].Normal.y = matComb._24 - matComb._21;
		frustumPlanes[1].Normal.z = matComb._34 - matComb._31;
		frustumPlanes[1].Distance = matComb._44 - matComb._41;
		frustumPlanes[2].Normal.x = matComb._14 - matComb._12;
		frustumPlanes[2].Normal.y = matComb._24 - matComb._22;
		frustumPlanes[2].Normal.z = matComb._34 - matComb._32;
		frustumPlanes[2].Distance = matComb._44 - matComb._42;
		frustumPlanes[3].Normal.x = matComb._14 + matComb._12;
		frustumPlanes[3].Normal.y = matComb._24 + matComb._22;
		frustumPlanes[3].Normal.z = matComb._34 + matComb._32;
		frustumPlanes[3].Distance = matComb._44 + matComb._42;
		frustumPlanes[4].Normal.x = matComb._13;
		frustumPlanes[4].Normal.y = matComb._23;
		frustumPlanes[4].Normal.z = matComb._33;
		frustumPlanes[4].Distance = matComb._43;
		frustumPlanes[5].Normal.x = matComb._14 - matComb._13;
		frustumPlanes[5].Normal.y = matComb._24 - matComb._23;
		frustumPlanes[5].Normal.z = matComb._34 - matComb._33;
		frustumPlanes[5].Distance = matComb._44 - matComb._43;

		//frustumPlanes[0].Normalize();
		//frustumPlanes[1].Normalize();
		//frustumPlanes[2].Normalize();
		//frustumPlanes[3].Normalize();
		//frustumPlanes[4].Normalize();
		//frustumPlanes[5].Normalize();
	}
	inline bool TestPoint( D3DXVECTOR3 &point )	// Тестируем точку на предмет попадания в объём фрустума
	{
		if ( (   frustumPlanes[0].DistanceToPoint( point )	< 0.0f )
			|| ( frustumPlanes[1].DistanceToPoint( point )	< 0.0f )
			|| ( frustumPlanes[2].DistanceToPoint( point )	< 0.0f )
			|| ( frustumPlanes[3].DistanceToPoint( point )	< 0.0f )
			|| ( frustumPlanes[4].DistanceToPoint( point )	< 0.0f )
			|| ( frustumPlanes[5].DistanceToPoint( point )	< 0.0f ) )
			return false;

		return true;
	}
	inline bool TestCameraInsideBox(  CFrustumBox* Box  )	// Проверяем нахождение камеры в боксе
	{
		if ( pos.x < Box->GetMin()->x && pos.x > Box->GetMax()->x )
			if ( pos.y < Box->GetMin()->y && pos.y > Box->GetMax()->y )
				if ( pos.z < Box->GetMin()->z && pos.z > Box->GetMax()->z )
					return false;

		return true;
	}
	inline bool TestAllBoxInsideFrustum( CFrustumBox* Box )	// Проверяем попадание всего бокса в фрустум
	{
        D3DXVECTOR3 min = *Box->GetMin(), max = *Box->GetMax();
        static int p;
        for ( p = 0; p < 6; p++ )
        {
            if ( frustumPlanes[p].Normal.x * ( min.x ) + frustumPlanes[p].Normal.y * ( min.y ) + frustumPlanes[p].Normal.z * ( min.z )
				+ frustumPlanes[p].Distance < 0.0f ||
            frustumPlanes[p].Normal.x * ( max.x ) + frustumPlanes[p].Normal.y * ( min.y ) + frustumPlanes[p].Normal.z * ( min.z )
				+ frustumPlanes[p].Distance < 0.0f ||
            frustumPlanes[p].Normal.x * ( min.x ) + frustumPlanes[p].Normal.y * ( max.y ) + frustumPlanes[p].Normal.z * ( min.z )
				+ frustumPlanes[p].Distance < 0.0f ||
            frustumPlanes[p].Normal.x * ( max.x ) + frustumPlanes[p].Normal.y * ( max.y ) + frustumPlanes[p].Normal.z * ( min.z ) 
				+ frustumPlanes[p].Distance < 0.0f ||
            frustumPlanes[p].Normal.x * ( min.x ) + frustumPlanes[p].Normal.y * ( min.y ) + frustumPlanes[p].Normal.z * ( max.z )
				+ frustumPlanes[p].Distance < 0.0f ||
            frustumPlanes[p].Normal.x * ( max.x ) + frustumPlanes[p].Normal.y * ( min.y ) + frustumPlanes[p].Normal.z * ( max.z )
				+ frustumPlanes[p].Distance < 0.0f || 
			frustumPlanes[p].Normal.x * ( min.x ) + frustumPlanes[p].Normal.y * ( max.y ) + frustumPlanes[p].Normal.z * ( max.z )
				+ frustumPlanes[p].Distance < 0.0f ||
            frustumPlanes[p].Normal.x * ( max.x ) + frustumPlanes[p].Normal.y * ( max.y ) + frustumPlanes[p].Normal.z * ( max.z )
				+ frustumPlanes[p].Distance < 0.0f )
				return false;
        }

		return true;
	}
	inline bool TestBoxInsideFrustum( CFrustumBox* Box )	// Проверяем попадание бокса в фрустум
	{
        D3DXVECTOR3 min = *Box->GetMin(), max = *Box->GetMax();
        static int p;
        for ( p = 0; p < 6; p++ )
        {
            if ( frustumPlanes[p].Normal.x * ( min.x ) + frustumPlanes[p].Normal.y * ( min.y ) + frustumPlanes[p].Normal.z * ( min.z )
				+ frustumPlanes[p].Distance > 0.0f ) continue;
            if ( frustumPlanes[p].Normal.x * ( max.x ) + frustumPlanes[p].Normal.y * ( min.y ) + frustumPlanes[p].Normal.z * ( min.z )
				+ frustumPlanes[p].Distance > 0.0f ) continue;
            if ( frustumPlanes[p].Normal.x * ( min.x ) + frustumPlanes[p].Normal.y * ( max.y ) + frustumPlanes[p].Normal.z * ( min.z )
				+ frustumPlanes[p].Distance > 0.0f ) continue;
            if ( frustumPlanes[p].Normal.x * ( max.x ) + frustumPlanes[p].Normal.y * ( max.y ) + frustumPlanes[p].Normal.z * ( min.z ) 
				+ frustumPlanes[p].Distance > 0.0f ) continue;
            if ( frustumPlanes[p].Normal.x * ( min.x ) + frustumPlanes[p].Normal.y * ( min.y ) + frustumPlanes[p].Normal.z * ( max.z )
				+ frustumPlanes[p].Distance > 0.0f ) continue;
            if ( frustumPlanes[p].Normal.x * ( max.x ) + frustumPlanes[p].Normal.y * ( min.y ) + frustumPlanes[p].Normal.z * ( max.z )
				+ frustumPlanes[p].Distance > 0.0f ) continue;
            if ( frustumPlanes[p].Normal.x * ( min.x ) + frustumPlanes[p].Normal.y * ( max.y ) + frustumPlanes[p].Normal.z * ( max.z )
				+ frustumPlanes[p].Distance > 0.0f ) continue;
            if ( frustumPlanes[p].Normal.x * ( max.x ) + frustumPlanes[p].Normal.y * ( max.y ) + frustumPlanes[p].Normal.z * ( max.z )
				+ frustumPlanes[p].Distance > 0.0f ) continue;
				return false;
        }

		return true;
	}
	inline bool TestBoxInsideFrustum( CAABB* Box )	// Проверяем попадание бокса в фрустум
	{
        D3DXVECTOR3 min = *Box->GetMin(), max = *Box->GetMax();
        static int p;
        for ( p = 0; p < 6; p++ )
        {
            if ( frustumPlanes[p].Normal.x * ( min.x ) + frustumPlanes[p].Normal.y * ( min.y ) + frustumPlanes[p].Normal.z * ( min.z )
				+ frustumPlanes[p].Distance > 0.0f ) continue;
            if ( frustumPlanes[p].Normal.x * ( max.x ) + frustumPlanes[p].Normal.y * ( min.y ) + frustumPlanes[p].Normal.z * ( min.z )
				+ frustumPlanes[p].Distance > 0.0f ) continue;
            if ( frustumPlanes[p].Normal.x * ( min.x ) + frustumPlanes[p].Normal.y * ( max.y ) + frustumPlanes[p].Normal.z * ( min.z )
				+ frustumPlanes[p].Distance > 0.0f ) continue;
            if ( frustumPlanes[p].Normal.x * ( max.x ) + frustumPlanes[p].Normal.y * ( max.y ) + frustumPlanes[p].Normal.z * ( min.z ) 
				+ frustumPlanes[p].Distance > 0.0f ) continue;
            if ( frustumPlanes[p].Normal.x * ( min.x ) + frustumPlanes[p].Normal.y * ( min.y ) + frustumPlanes[p].Normal.z * ( max.z )
				+ frustumPlanes[p].Distance > 0.0f ) continue;
            if ( frustumPlanes[p].Normal.x * ( max.x ) + frustumPlanes[p].Normal.y * ( min.y ) + frustumPlanes[p].Normal.z * ( max.z )
				+ frustumPlanes[p].Distance > 0.0f ) continue;
            if ( frustumPlanes[p].Normal.x * ( min.x ) + frustumPlanes[p].Normal.y * ( max.y ) + frustumPlanes[p].Normal.z * ( max.z )
				+ frustumPlanes[p].Distance > 0.0f ) continue;
            if ( frustumPlanes[p].Normal.x * ( max.x ) + frustumPlanes[p].Normal.y * ( max.y ) + frustumPlanes[p].Normal.z * ( max.z )
				+ frustumPlanes[p].Distance > 0.0f ) continue;
				return false;
        }

		return true;
	}
protected:
	D3DXMATRIX* V;
	void CreateViewMatrix( void );

	eTypeCamera	Type;
	D3DXVECTOR3 right;		// Правый вектор
	D3DXVECTOR3 up;			// Верхний вектор(наклон камеры в право или в лево)
	D3DXVECTOR3 look;		// Направление взгляда
	D3DXVECTOR3 pos;		// Вектор положения(положение глаза)

	CPlane frustumPlanes[6];
};
