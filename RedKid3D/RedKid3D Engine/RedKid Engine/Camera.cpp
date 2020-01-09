#include "header.h"
#include "Camera.h"

CCamera::CCamera(void)
{
	Type = AIRCRAFT;

	pos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	up    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	//CamData.mYaw	= 0.0f;
	//CamData.mPitch	= 0.0f;
	//CamData.mRoll	= 0.0f;

	V = new D3DXMATRIX();
}

CCamera::~CCamera(void)
{
	delete V;
}

CCamera::CCamera( eTypeCamera cameraType )
{
	Type = cameraType;

	pos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	up    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	//CamData.mYaw	= 0.0f;
	//CamData.mPitch	= 0.0f;
	//CamData.mRoll	= 0.0f;

	V = new D3DXMATRIX();

	//CreateViewMatrix();
}

void CCamera::CreateViewMatrix( void )
{
     // Делаем оси камеры ортогональными
     D3DXVec3Normalize( &look, &look );

     D3DXVec3Cross( &up, &look, &right );
     D3DXVec3Normalize( &up, &up );

     D3DXVec3Cross( &right, &up, &look );
     D3DXVec3Normalize( &right, &right );

     // Строим матрицу вида:
     float x = -D3DXVec3Dot( &right, &pos );
     float y = -D3DXVec3Dot( &up, &pos );
     float z = -D3DXVec3Dot( &look, &pos );

     (*V)(0, 0) = right.x;
     (*V)(0, 1) = up.x;
     (*V)(0, 2) = look.x;
     (*V)(0, 3) = 0.0f;

     (*V)(1, 0) = right.y;
     (*V)(1, 1) = up.y;
     (*V)(1, 2) = look.y;
     (*V)(1, 3) = 0.0f;

     (*V)(2, 0) = right.z;
     (*V)(2, 1) = up.z;
     (*V)(2, 2) = look.z;
     (*V)(2, 3) = 0.0f;

     (*V)(3, 0) = x;
     (*V)(3, 1) = y;
     (*V)(3, 2) = z;
     (*V)(3, 3) = 1.0f;
}

// вращение относительно правого вектора
void CCamera::Pitch(float angle)
{
	D3DXMATRIX T;
	D3DXMatrixRotationAxis( &T, &right, angle );

    // Поворот векторов _up и _look относительно вектора _right
    D3DXVec3TransformCoord( &up, &up, &T );
    D3DXVec3TransformCoord( &look, &look, &T );
}

void CCamera::Yaw(float angle)
{
     D3DXMATRIX T;

     // Для наземных объектов выполняем вращение
     // вокруг мировой оси Y (0, 1, 0)
     if( Type == LANDOBJECT )
          D3DXMatrixRotationY( &T, angle );

     // Для летающих объектов выполняем вращение
     // относительно верхнего вектора
     if( Type == AIRCRAFT )
          D3DXMatrixRotationAxis( &T, &up, angle );

     // Поворот векторов _right и _look относительно
     // вектора _up или оси Y
     D3DXVec3TransformCoord( &right, &right, &T );
     D3DXVec3TransformCoord( &look, &look, &T );
}

void CCamera::Roll(float angle)
{
     // Вращение только для летающих объектов
     if( Type == AIRCRAFT )
     {
        D3DXMATRIX T;
        D3DXMatrixRotationAxis( &T, &look, angle );

        // Поворот векторов _up и _right относительно
        // вектора _look
        D3DXVec3TransformCoord( &right, &right, &T );

        D3DXVec3TransformCoord( &up, &up, &T );
     }
}

void CCamera::Walk(float units)
{
     // Для наземных объектов перемещение только в плоскости xz
     if( Type == LANDOBJECT )
          pos += D3DXVECTOR3( look.x, 0.0f, look.z ) * units;

     if( Type == AIRCRAFT )
          pos += look * units;
}

void CCamera::Strafe(float units)
{
     // Для наземных объектов перемещение только в плоскости xz
     if( Type == LANDOBJECT )
	 {
          pos += D3DXVECTOR3( right.x, 0.0f, right.z ) * units;
		  return;
	 }

     if( Type == AIRCRAFT )
          pos += right * units;
}
