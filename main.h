#ifndef _MAIN_H
#define _MAIN_H

#include <windows.h>
#include <gl/gl.h>								// OpenGL32 library
#include <gl/glu.h>								// Glu32 library
#include <gl/glaux.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "3dMath.h"
#include <iostream>
#include <string>
#include <crtdbg.h>
#include <vector>
#include <fstream>
#include <gl/wglext.h>							//WGL extensions
#include <gl/glext.h>							//GL extensions

using namespace std;

#define WIND_CLASS "eCentral OpenGL"			// Window class name
#define MAX_TEXTURES 200						// The maximum amount of textures to load

extern int WIND_WIDTH;							// Window width
extern int WIND_HEIGHT;							// Window height
extern int FS_WIDTH;							// FullScreen width
extern int FS_HEIGHT;							// FullScreen height
extern int SCREEN_DEPTH;						// Bits per pixel
extern int FRAME_RATE;							// Frames per second

extern bool g_bFullScreen;						// Full screen mode

extern HDC g_hDC;								// Global device context handle
extern HGLRC g_hRC;								// Global rendering context handle
extern HWND	g_hWnd;								// Global window handle
extern HINSTANCE g_hInstance;					// Global window instance handle
extern RECT g_WindowRect;						// Global window rectangular handle


// Camera ******************

// todo: move these into our 3dMath class
//#pragma warning( disable : 4430 ) -- not a good idea


// Face structure. Used for indexing into the vertex and texture coordinate arrays
struct tFace
{
	int vertIndex[3];		// Indicies for the vertices that make up triangle
	int coordIndex[3];		// Indicies for the texture coordinates to texture face
};

// Stores material information
struct tMaterialInfo
{
	char  strName[255];			// Texture name
	char  strFile[255];			// Texture map file name
	BYTE  color[3];				// RGB color of the object
	int   texureId;				// the texture ID			// FIX THIS VARIABLE NAME
};

// Holds model information
struct t3DObject
{
	int  numOfVerts;			// The number of verts in the model
	int  numOfFaces;			// The number of faces in the model
	int  numTexVertex;			// The number of texture coordinates
	int  materialID;			// The texture ID to use, which is the index into the texture array
	bool bHasTexture;			// This is TRUE if there is a texture map for this object
	bool bIsTrans;				// This is TRUE if the object is transparent
	char strName[255];			// Name of the object
	CVector3 *pVerts;			// The object's vertices
	CVector3 *pNormals;			// The object's normals
	CVector2 *pTexVerts;		// The texture's UV coordinates
	tFace *pFaces;				// The faces information of the object
};


// Holds model information
struct t3DModel 
{
	int numOfObjects;					// The number of objects in the model
	int numOfMaterials;					// The number of materials for the model
	vector<tMaterialInfo> pMaterials;	// The list of material information (Textures and colors)
	vector<t3DObject> pObject;			// The object list for our model
};


/****************************/
/*	      Init.cpp          */
/****************************/

HWND CreateGLWindow(LPSTR strWindowName, int wndWidth, int wndHeight, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance);	// Creates the OpenGL Window

// Program entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int iShow);

bool bSetupPixelFormat(HDC hDC);
void InitializeOpenGL( int wndWidth, int wndHeight );
void SizeOpenGLScreen(int wndWidth, int wndHeight);
void DeInitialize();
void ToggleFullScreen();
void CreateTexture(UINT textureArray[], LPSTR strFileName, int textureID);

/****************************/
/*	     Main.cpp   	    */
/****************************/

// Handle window procedures
LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Controls the programs main loop
WPARAM MainLoop();

void Initialize(HWND hWnd);
void RenderGLScene();
void PerspectiveMode();
void OrthoMode(int left, int top, int right, int bottom);
void SetCamera();
void DrawInterface();
void DrawObject(t3DObject *pObject);
void DrawModel();

/****************************/
/*	    Camera.cpp		    */
/****************************/
void KeyFunc();

#endif
