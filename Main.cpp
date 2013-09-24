/*****************************************************************************
*																			 *
* File: Main.cpp															 *
* Author: B. Jordan, D. Elkins, E. Wood							 			 *
* Date Modified: 28/10/06													 *
* Description: This file contains the main loop and callback functions, as	 *
* well as the window initialisation and OpenGL scene rendering functions.	 *
*																			 *
*****************************************************************************/

// 28th Oct- Still need antialiasing
//			 The amount of vertices in the model has been reduced. Objects such
//			 as the poles may require more edges. Requires testing.

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")
#pragma comment(lib, "OpenAL32.lib")
#pragma comment(lib, "ALut.lib")
#pragma comment(lib, "winmm.lib")

#include "main.h"
#include "loadModel.h"
#include "sound.h"
#include "camera.h"

#define FILE_NAME  "model.3ds"				    // Stage geometry

HDC			g_hDC;								// Global Device Context Handle
HGLRC		g_hRC;								// Global Rendering Context Handle
HWND		g_hWnd;								// Global Window Handle
HINSTANCE	g_hInstance;						// Global Window Instance
RECT		g_WindowRect;						// Global Window Rectangle
CCamera		g_Camera;							// Global Camera Object
int			FRAME_RATE = 42;					// Frame Rate

GLuint	interfaceTexture[3];					// Interface mask textures
GLuint	transparencyTexture[2];					// Falloff mask textures
UINT g_Texture[MAX_TEXTURES] = {0};				// Holds texture information

CLoadModel g_Load3ds;							// Instantiate model object
t3DModel g_3DModel;								// Instantiate model structure

int   g_ViewMode	  = GL_TRIANGLES;			// set normal drawing mode
bool  g_bLighting     = true;					// Turn lighting on initially

/***************************************************************************************
*																					   *
* Function: MainLoop																   *
* Parameters: HWND hWnd																   *
* Description: Initializes the game window.											   *
*																					   *
***************************************************************************************/
void Initialize(HWND hWnd) {

	g_hWnd = hWnd;
	GetClientRect(g_hWnd, &g_WindowRect);
	InitializeOpenGL(g_WindowRect.right, g_WindowRect.bottom);

	// Create the interface textures
	CreateTexture(interfaceTexture, "interfaceMask.bmp", 0);
	CreateTexture(interfaceTexture, "interface.bmp", 1);

	// Create the falloff transperancy textures
	CreateTexture(transparencyTexture, "fallMask.bmp", 0);
	CreateTexture(transparencyTexture, "falloff.bmp", 1);

	g_Load3ds.Import3DS(&g_3DModel, FILE_NAME);			// Load 3ds file into model structure

	for(int i = 0; i < g_3DModel.numOfMaterials; i++)	// Go through all the materials
	{
		if(strlen(g_3DModel.pMaterials[i].strFile) > 0)	// Check if filename exists
		{
			CreateTexture(g_Texture, g_3DModel.pMaterials[i].strFile, i);
		}
		g_3DModel.pMaterials[i].texureId = i;			// Set the texture ID for material
	}

	initializeAudio();

	g_Camera.SetCameraPosition(10, 4, 12,   9, 4, 12,   0, 1, 0);
	g_Camera.SetCollisionRadius(5);

	glCullFace(GL_BACK);								// Don't draw the back sides of polygons
	glEnable(GL_CULL_FACE);								// Turn on culling

	glShadeModel(GL_SMOOTH);

	//static GLfloat	LightAmb[] = {0.01f, 0.01f, 0.01f, 1.0f};			// Ambient Light
	//static GLfloat	LightDif[] = {1.02f, 1.02f, 1.02f, 1.00f};			// Diffuse Light
	//static GLfloat	LightPos[] = {0.0f, 40.0f, 2.0f, 1.0f};				// Light Position

	//glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmb);			// Set The Ambient Lighting For Light0
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDif);			// Set The Diffuse Lighting For Light0
	//glLightfv(GL_LIGHT0, GL_POSITION, LightPos);			// Set The Position For Light0

	glEnable(GL_LIGHT0);								// Enable Light 0
	glEnable(GL_LIGHTING);								// Enable Lighting

	glEnable(GL_COLOR_MATERIAL);						// Allow color

	float fogColor[4] = {0.0, 0.0, 0.0, 1.0f};

	glFogi(GL_FOG_MODE, GL_EXP2);						// Set The Fog Mode
	glFogfv(GL_FOG_COLOR, fogColor);					// Set The Fog Color
	glFogf(GL_FOG_DENSITY, 0.005f);						// Set How Dense Will The Fog Be
	glHint(GL_FOG_HINT, GL_DONT_CARE);					// Set The Fog's calculation accuracy
	glFogf(GL_FOG_START, 620);							// Set The Fog's Start Depth
	glFogf(GL_FOG_END, 650.0f);							// Set The Fog's End Depth

	glEnable(GL_FOG);									// Enable fog (turn it on)

	PerspectiveMode();
	glEnable(GL_DEPTH_TEST);

}

/***************************************************************************************
*																					   *
* Function: LockFrameRate															   *
* Parameters: int frame_rate														   *
* Description: Gets the elapsed time between the the current frame and last frame.     *
*																					   *
***************************************************************************************/
bool LockFrameRate(int frame_rate) {

	static float lastTime = 0.0f;

	float currentTime = GetTickCount() * 0.001f;	// Get current time in seconds (milliseconds * .001 = seconds)

	// Get the elapsed time by subtracting the current time from the last time
	// If the desired frame rate amount of seconds has passed -- return true (ie Blit())
	if((currentTime - lastTime) > (1.0f / frame_rate))
	{
		lastTime = currentTime;						// Reset the last time	
		return true;
	}

	return false;
}

/***************************************************************************************
*																					   *
* Function: MainLoop																   *
* Parameters: -																		   *
* Description: Handles the main game loop.											   *
*																					   *
***************************************************************************************/
WPARAM MainLoop() {

	MSG msg;

	while(1) {												// Infinate loop
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {		// Check if there was a message
			if(msg.message == WM_QUIT)						// If the message wasnt to quit
				break;
            TranslateMessage(&msg);							// Find out what the message does
            DispatchMessage(&msg);							// Execute the message
        }
		else if(LockFrameRate(FRAME_RATE)) {				// If it is time to render
			g_Camera.Tick();								// Update the camera
			RenderGLScene();

		}
	}
	

	DeInitialize();

	return (msg.wParam);
}

/***************************************************************************************
*																					   *
* Function: Draw3DSGrid																   *
* Parameters: -																		   *
* Description: Draws a grid. ( 100 lines on the y axis, 100 lines on the x axis. )	   *
*			   Gives an idea of where we are on a plane.							   *
*																					   *
***************************************************************************************/
void Draw3DSGrid()
{
	glColor4f(0.0f, 1.0f, 0.0f, 0.9f);

	// Draw a 1x1 grid along the X and Z axis
	for(GLfloat i = -50.0f; i <= 50.0f; i += 1.0f)
	{

	glBegin(GL_LINES);

			glVertex3f(-50, 0, i);
			glVertex3f(50, 0, i);

			glVertex3f(i, 0, -50);
			glVertex3f(i, 0, 50);

	glEnd();
	}
}

/***************************************************************************************
*																					   *
* Function: DrawModel																   *
* Parameters: -																		   *
* Description: Draws model objects and binds its textures to it.					   *
*																					   *
***************************************************************************************/
void DrawModel(){

	// Go through objects
	for(int i = 0; i < g_3DModel.numOfObjects; i++)
	{
		// Make sure use case objects are valid
		if(g_3DModel.pObject.size() <= 0) break;

		// Get the current object that we are displaying
		t3DObject *pObject = &g_3DModel.pObject[i];


			// If the object is transparent, we need to draw it twice with our falloff map
			if (pObject->bIsTrans == true){
				
					glEnable(GL_BLEND);		

					glBlendFunc(GL_DST_COLOR,GL_ZERO);
					glBindTexture(GL_TEXTURE_2D,  transparencyTexture[1]);

						DrawObject(&g_3DModel.pObject[i]);	

					glBlendFunc(GL_ONE,GL_ONE);
					glBindTexture(GL_TEXTURE_2D,  transparencyTexture[0]);
					
						DrawObject(&g_3DModel.pObject[i]);

					glDisable(GL_BLEND);


				}

			else{

				// If the object isn't transparent, draw it once
				DrawObject(&g_3DModel.pObject[i]);
			}
	}
}


/***************************************************************************************
*																					   *
* Function: DrawInterface															   *
* Parameters: -																	       *
* Description: Draws a 2D quad with interface mask texture and a 2D quad with the	   *
*			   interface texture and blends them together to create a mask with		   *
*			   different levels of transperancy.									   *
*																					   *
***************************************************************************************/
void DrawInterface(){

	glDisable(GL_DEPTH_TEST);

	// Enable blending mode
	glEnable(GL_BLEND);		

	glBlendFunc(GL_DST_COLOR,GL_ZERO);
	glBindTexture(GL_TEXTURE_2D,  interfaceTexture[0]);

	// Draw a 2D quad with the interface mask texture
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);	glVertex2f(0, 0);
		glTexCoord2f(0.0f, 0.0f);	glVertex2f(0,WIND_HEIGHT);
		glTexCoord2f(1.0f, 0.0f);	glVertex2f(WIND_WIDTH, WIND_HEIGHT);
		glTexCoord2f(1.0f, 1.0f);	glVertex2f(WIND_WIDTH, 0);
	glEnd();	

	glBlendFunc(GL_ONE,GL_ONE);
	glBindTexture(GL_TEXTURE_2D,  interfaceTexture[1]);
	
	// Draw a 2D quad with the interface texture
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);	glVertex2f(0, 0);
		glTexCoord2f(0.0f, 0.0f);	glVertex2f(0, WIND_HEIGHT);
		glTexCoord2f(1.0f, 0.0f);	glVertex2f(WIND_WIDTH, WIND_HEIGHT);
		glTexCoord2f(1.0f, 1.0f);	glVertex2f(WIND_WIDTH, 0);
	glEnd();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

}

/***************************************************************************************
*																					   *
* Function: WinProc																	   *
* Parameters: HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam						   *
* Description: Windows procedure, handles all messages for this program				   *
*																					   *
***************************************************************************************/
LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	
    LONG    lRet = 0; 
    PAINTSTRUCT    ps;

    switch (msg)
	{ 
    case WM_SIZE:										// If the window is resized
		if(!g_bFullScreen)								// Do this is we are not in full screen
		{
			SizeOpenGLScreen(LOWORD(lParam),HIWORD(lParam));// LoWord=Width, HiWord=Height
			GetClientRect(hWnd, &g_WindowRect);					// Get the window rectangle
		}
        break; 
 
	case WM_PAINT:										// If we need to repaint the scene
		BeginPaint(hWnd, &ps);							// Init the paint struct		
		EndPaint(hWnd, &ps);							// EndPaint, Clean up
		break;

	case WM_KEYDOWN:

		switch(wParam) {								// Check if we hit a key
			case VK_ESCAPE:								// If we hit the escape key
				PostQuitMessage(0);						// Send a QUIT message to the window
				break;
		}
		break;

    case WM_CLOSE:										// If the window is being closes
        PostQuitMessage(0);								// Send a QUIT Message to the window
        break; 
     
    default:											// Return by default
        lRet = DefWindowProc (hWnd, msg, wParam, lParam); 
        break; 
    } 
 
    return lRet;										// Return by default
}

/***************************************************************************************
*																					   *
* Function: RenderGLScene															   *
* Parameters: -																	       *
* Description: Cleans the depth buffer, screen and matrix. Sets Initial camera view	   *
*			   and executes drawing functions. Swaps buffers.						   *
*																					   *
***************************************************************************************/
void RenderGLScene() {

	//glEnable(GL_MULTISAMPLE_ARB);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear the screen and the depth buffer
	
	glLoadIdentity();									// Reset the matrix

	// Enable light sources
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	
	

	g_Camera.Look();

	//Draw3DSGrid();
	DrawModel();	

	OrthoMode(0, 0, WIND_WIDTH, WIND_HEIGHT);			// Switch to 2D mode
	
	// Disable light sources
	glDisable(GL_LIGHT0);								
	glDisable(GL_LIGHTING);

	DrawInterface();									// Create the interface


	PerspectiveMode();									// Switch back to 3d mode

	glFlush ();											// Flush The GL Rendering Pipeline

	SwapBuffers(g_hDC);									// Swap front-back buffers

	//glDisable(GL_MULTISAMPLE_ARB);
}

/***************************************************************************************
*																					   *
* Function: OrthoMode																   *
* Parameters: -																	       *
* Description: Sets up a orthographic (2D) view.									   *
*																					   *
***************************************************************************************/
void OrthoMode(int left, int top, int right, int bottom)
{


	// Switch to our projection matrix so that we can change it to ortho mode, not perspective.
	glMatrixMode(GL_PROJECTION);						

	// Push on a new matrix so that we can just pop it off to go back to perspective mode
	glPushMatrix();									
	
	// Reset the current matrix to our identify matrix
	glLoadIdentity();								

	//Pass in our 2D ortho screen coordinates.like so (left, right, bottom, top).  The last
	// 2 parameters are the near and far planes.
	glOrtho( left, right, bottom, top, 0, 1 );	
	
	// Switch to model view so that we can render the scope image
	glMatrixMode(GL_MODELVIEW);								

	// Initialize the current model view matrix with the identity matrix
	glLoadIdentity();										
}




/***************************************************************************************
*																					   *
* Function: PerspectiveMode															   *
* Parameters: -																	       *
* Description: Sets up a perspective view.											   *
*																					   *
***************************************************************************************/
void PerspectiveMode()										// Set Up A Perspective View
{
	// Enter into our projection matrix mode
	glMatrixMode( GL_PROJECTION );							

	// Pop off the last matrix pushed on when in projection mode (Get rid of ortho mode)
	glPopMatrix();											

	// Go back to our model view matrix like normal
	glMatrixMode( GL_MODELVIEW );							

	// We should be in the normal 3D perspective mode now
}




/***************************************************************************************
*																					   *
* Function: DrawObject																   *
* Parameters: t3DObject *pObject													   *
* Description: Draws objects passed in and binds its associated textures.			   *
*																					   *
***************************************************************************************/
void DrawObject(t3DObject *pObject){

		// Check to see if this object has a texture map, if so bind the texture to it.
		if ((pObject->bHasTexture) && (pObject->bIsTrans != true)) {
			
	
			// Turn on texture mapping and turn off color
			glEnable(GL_TEXTURE_2D);

			// Reset the color to normal again
			glColor3ub(255, 255, 255);

			// Bind the texture map to the object by it's materialID
			glBindTexture(GL_TEXTURE_2D, g_Texture[pObject->materialID]);


		} 

		glBegin(g_ViewMode);					// Begin drawing with our selected mode (triangles or lines)

			// Go through all of the faces (polygons) of the object and draw them
			for(int j = 0; j < pObject->numOfFaces; j++)
			{


				// Go through each corner of the triangle and draw it.
				for(int whichVertex = 0; whichVertex < 3; whichVertex++)
				{
					// Get the index for each point of the face
					int index = pObject->pFaces[j].vertIndex[whichVertex];
			
					// Give OpenGL the normal for this vertex.
					glNormal3f(pObject->pNormals[ index ].x, pObject->pNormals[ index ].y, pObject->pNormals[ index ].z);
				
					// If the object has a texture associated with it, give it a texture coordinate.
					if(pObject->bHasTexture) {

						// Make sure there was a UVW map applied to the object or else it won't have tex coords.
						if(pObject->pTexVerts) {
							glTexCoord2f(pObject->pTexVerts[ index ].x, pObject->pTexVerts[ index ].y);
						}
					} else {

						if(g_3DModel.pMaterials.size() && pObject->materialID >= 0) 
						{
							// Get and set the color that the object is, since it must not have a texture
							BYTE *pColor = g_3DModel.pMaterials[pObject->materialID].color;

							// Assign the current color to this model
							glColor3ub(pColor[0], pColor[1], pColor[2]);
						}
					}

					// Pass in the current vertex of the object (Corner of current face)
					glVertex3f(pObject->pVerts[ index ].x, pObject->pVerts[ index ].y, pObject->pVerts[ index ].z);
				}
			}

		glEnd();
	}
