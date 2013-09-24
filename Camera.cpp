/*****************************************************************************
*																			 *
* File: Camera.cpp															 *
* Author: B. Jordan, D. Elkins, E. Wood							 			 *
* Date Created: 26/10/06													 *
* Last Modified: 27/10/06													 *
* Description: Defines all of our camera operations- includes collision.	 *
*																			 *
*****************************************************************************/

// 28/10/06 - Still needs testing on different screen resolutions
//			- Made xRotation static

#include "main.h"
#include "camera.h"

// Camera movement speed
#define WalkSpeed	55.0f	

// FPS calculation function prototype
void CalculateFrameRate();

// Elapsed time between the current and last frame
float g_FrameInterval = 0.0f;


/***************************************************************************************
*																					   *
* Function: void Tick																   *
* Parameters: -																		   *
* Description:																		   *
*																					   *
***************************************************************************************/
void CCamera::Tick()
{

	CVector3 vCross = Cross(m_vView - m_vPosition, m_vUpVector);

	m_vStrafe = Normalize(vCross);

	SetViewByMouse();

	CheckMovement();

	CalculateFrameRate();
}

/***************************************************************************************
*																					   *
* Function: void Look																   *
* Parameters: -																		   *
* Description: Defines our viewing transformation.									   *
*																					   *
***************************************************************************************/
void CCamera::Look()
{
	gluLookAt(m_vPosition.x, m_vPosition.y, m_vPosition.z,	
			  m_vView.x,	 m_vView.y,     m_vView.z,	
			  m_vUpVector.x, m_vUpVector.y, m_vUpVector.z);
}

/***************************************************************************************
*																					   *
* Function: CCamera																	   *
* Parameters: -																		   *
* Description: The constructor for CCamera- Initializes camera member variables.	   *
*																					   *
***************************************************************************************/
CCamera::CCamera()
{
	CVector3 vPos	   = CVector3(0.0, 0.0, 0.0);
	CVector3 vView	   = CVector3(0.0, 1.0, 0.5);
	CVector3 vUp	   = CVector3(0.0, 0.0, 1.0);

	m_vPosition	= vPos;
	m_vView		= vView;
	m_vUpVector	= vUp;
}


/***************************************************************************************
*																					   *
* Function: void CCamera::PositionCamera											   *
* Parameters: float positionX, float positionY, float positionZ						   *
*			  float viewX,     float viewY,     float viewZ							   *
*			  float upVectorX, float upVectorY, float upVectorZ						   *
* Description: 	Assign the camera its position, field of view, and up vector.		   *
*																					   *
***************************************************************************************/
void CCamera::SetCameraPosition(float positionX, float positionY, float positionZ,
				  		        float viewX,     float viewY,     float viewZ,
							    float upVectorX, float upVectorY, float upVectorZ)
{
	CVector3 vPosition	= CVector3(positionX, positionY, positionZ);
	CVector3 vView		= CVector3(viewX, viewY, viewZ);
	CVector3 vUpVector	= CVector3(upVectorX, upVectorY, upVectorZ);

	m_vPosition = vPosition;
	m_vView     = vView;
	m_vUpVector = vUpVector;
}

/***************************************************************************************
*																					   *
* Function: void MoveCamera															   *
* Parameters: float speed															   *
* Description: Moves the camera forwards or backwards, depending on the speed past in. *
*																					   *
***************************************************************************************/
void CCamera::MoveCamera(float speed)
{
	// Get the current view vector (the direction we are looking)
	CVector3 vVector = m_vView - m_vPosition;
	vVector = Normalize(vVector);

	// Add the acceleration to the position and views X and Z
	m_vPosition.x += vVector.x * speed;
	m_vPosition.z += vVector.z * speed;
	m_vView.x += vVector.x * speed;
	m_vView.z += vVector.z * speed;
}

/***************************************************************************************
*																					   *
* Function: void MoveCamera															   *
* Parameters: float speed															   *
* Description: Adds the strafe vector to our position and view.						   *
*																					   *
***************************************************************************************/
void CCamera::StrafeCamera(float speed)
{	
	m_vPosition.x += m_vStrafe.x * speed;
	m_vPosition.z += m_vStrafe.z * speed;

	m_vView.x += m_vStrafe.x * speed;
	m_vView.z += m_vStrafe.z * speed;
}

/***************************************************************************************
*																					   *
* Function: void CheckMovement														   *
* Parameters: -																		   *
* Description: This handles key input fast than WinProc().							   *
*																					   *
***************************************************************************************/
void CCamera::CheckMovement()
{	
	float speed = WalkSpeed * g_FrameInterval;

	if(GetKeyState(VK_UP) & 0x80 || GetKeyState('W') & 0x80) {				

		MoveCamera(speed);				
	}

	if(GetKeyState(VK_DOWN) & 0x80 || GetKeyState('S') & 0x80) {			

		MoveCamera(-speed);				
	}

	if(GetKeyState(VK_LEFT) & 0x80 || GetKeyState('A') & 0x80) {			

		StrafeCamera(-speed);
	}

	if(GetKeyState(VK_RIGHT) & 0x80 || GetKeyState('D') & 0x80) {

	CVector3 GetCollisionOffset(CVector3 &vNormal, float radius, float distance);
		
		StrafeCamera(speed);
	}	
}

/***************************************************************************************
*																					   *
* Function: void RotateView  														   *
* Parameters: float angle, float x, float y, float z								   *
* Description: Rotates the view around the position. Uses axis-angle rotation.		   *
*																					   *
***************************************************************************************/
void CCamera::RotateView(float angle, float x, float y, float z)
{
	// TODO: turn into a 3dMath function

	CVector3 vNewView;

	// Store off the view vector
	CVector3 vView = m_vView - m_vPosition;		

	// Calculate the sine and cosine of the angle
	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);

	// Find the new x position for the new rotated point
	vNewView.x  = (cosTheta + (1 - cosTheta) * x * x)		* vView.x;
	vNewView.x += ((1 - cosTheta) * x * y - z * sinTheta)	* vView.y;
	vNewView.x += ((1 - cosTheta) * x * z + y * sinTheta)	* vView.z;

	// Find the new y position for the new rotated point
	vNewView.y  = ((1 - cosTheta) * x * y + z * sinTheta)	* vView.x;
	vNewView.y += (cosTheta + (1 - cosTheta) * y * y)		* vView.y;
	vNewView.y += ((1 - cosTheta) * y * z - x * sinTheta)	* vView.z;

	// Find the new z position for the new rotated point
	vNewView.z  = ((1 - cosTheta) * x * z - y * sinTheta)	* vView.x;
	vNewView.z += ((1 - cosTheta) * y * z + x * sinTheta)	* vView.y;
	vNewView.z += (cosTheta + (1 - cosTheta) * z * z)		* vView.z;

	// Add the newly rotated vector to position to set new rotated view of camera.
	m_vView = m_vPosition + vNewView;
}

/***************************************************************************************
*																					   *
* Function: void CalculateFrameRate													   *
* Parameters: -																		   *
* Description: Calculates the frame rate and intervals between frames.				   *
*																					   *
***************************************************************************************/
void CalculateFrameRate()
{
	static float framesPerSecond    = 0.0f;	
    static float lastTime			= 0.0f;	
	static char  strFrameRate[50] = {0};

	static float frameTime = 0.0f;

	// Get the current time in seconds
    float currentTime = timeGetTime() * 0.001f;				

	// Store the elapsed time between the current and last frame
 	g_FrameInterval = currentTime - frameTime;
	frameTime = currentTime;

    ++framesPerSecond;

    if( currentTime - lastTime > 1.0f )
    {
	    lastTime = currentTime;
		
        framesPerSecond = 0;
	}
}

void CCamera::SetViewByMouse()
{
	POINT mousePos;									// Window structure to hold the mouse position
	int middleX = WIND_WIDTH  >> 1;					// Get half the window width
	int middleY = WIND_HEIGHT >> 1;					// Get half the window height
	float angleY = 0.0f;							// Direction for looking up or down
	float angleZ = 0.0f;							// Y axis rotation (left and right)
	static float currentRotX = 0.0f;
	
	// Get the mouse's current X,Y position
	GetCursorPos(&mousePos);						
	
	// If the cursor remains in the middle, don't update the screen
	if( (mousePos.x == middleX) && (mousePos.y == middleY) ) return;

	// Set the mouse position to the middle of the current window
	SetCursorPos(middleX, middleY);							

	// Store the direction moved and scale it down
	angleY = (float)( (middleX - mousePos.x) ) / 500.0f;		
	angleZ = (float)( (middleY - mousePos.y) ) / 500.0f;		

	static float lastRotX = 0.0f;

 	lastRotX = currentRotX;
	
	// Store the current up/down rotation so that the camera to restrict the camera
	currentRotX += angleZ;
 
	// If the current rotation is greater than 1.0, it needs to be capped
	if(currentRotX > 1.0f)     
	{
		currentRotX = 1.0f;
		
		// Rotate by any remaining angle
		if(lastRotX != 1.0f) 
		{
			// Find the perpendicular vector to be used as the axis
			CVector3 vAxis = Cross(m_vView - m_vPosition, m_vUpVector);
			vAxis = Normalize(vAxis);
				
			// Rotate the camera by the remaining angle
			RotateView( 1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	}

	// Check if the rotation is below -1.0
	else if(currentRotX < -1.0f)
	{
		currentRotX = -1.0f;
		
		// Rotate by the remaining angle, if any
		if(lastRotX != -1.0f)
		{
			CVector3 vAxis = Cross(m_vView - m_vPosition, m_vUpVector);
			vAxis = Normalize(vAxis);
			
			RotateView( -1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	}

	// Else we can rotate the view around our position
	else 
	{	
		CVector3 vAxis = Cross(m_vView - m_vPosition, m_vUpVector);
		vAxis = Normalize(vAxis);
	
		RotateView(angleZ, vAxis.x, vAxis.y, vAxis.z);
	}

	// Always rotate the camera around the y-axis
	RotateView(angleY, 0, 1, 0);
}