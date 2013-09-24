#ifndef _CAMERA_H
#define _CAMERA_H

#include <mmsystem.h>		// required for the timeGetTime() function

class CCamera {

public:

	CCamera();	

	CVector3 Position() {	return m_vPosition;		}
	CVector3 View()		{	return m_vView;			}
	CVector3 UpVector() {	return m_vUpVector;		}
	CVector3 Strafe()	{	return m_vStrafe;		}
	
	void SetCameraPosition(float positionX, float positionY, float positionZ,
			 		       float viewX,     float viewY,     float viewZ,
						   float upVectorX, float upVectorY, float upVectorZ);

	void RotateView(float angle, float x, float y, float z);
	void MoveCamera(float speed);
	void StrafeCamera(float speed);
	void Look();
	void Tick();
	void CheckMovement();
	void SetViewByMouse();

	// Collision Functions
	
	void SetCollisionRadius(float radius) {	 m_radius = radius;	};

private:

	CVector3 m_vPosition;					
	CVector3 m_vView;
	CVector3 m_vUpVector;
	CVector3 m_vStrafe;		

	float m_radius;										

};


#endif