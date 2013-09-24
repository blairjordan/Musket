#include <iostream>
#include <fstream>
#include <string>

#include "3dMath.h"
#include "bitmap.h"
#include "main.h"
#include "sound.h"
//#include <windows.h>

using namespace std;

extern int main(int argc, char* argv[]);

char readConfigFile(string profileName);		// return the configuration file
char fullPath[256] = {0};						// directory of configuration file	

int WIND_WIDTH = 800; 							// Window width
int WIND_HEIGHT = 800;							// Window height
int FS_WIDTH = 800; 							// FullScreen width
int FS_HEIGHT = 600; 							// FullScreen height 
int SCREEN_DEPTH = 32;							// Bits per pixel 
bool g_bFullScreen;								// Full screen mode

CBitmap loadingBitmap;

/***************************************************************************************
*																					   *
* Function: WinMain																	   *
* Parameters: HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int iShow     *
* Description: Registers and creates the window with the global config variables.	   *
*																					   *
***************************************************************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int iShow) {

	char*  argv = cmdLine;

	readConfigFile( argv );	// Read config file and set corresponding globals

	HWND hWnd;

	g_bFullScreen = true;	// TODO: DELETE

	hWnd = CreateGLWindow("eCentral Tafe",FS_WIDTH,FS_HEIGHT,0,g_bFullScreen,hInstance);
	if(hWnd == NULL) return TRUE;
	
	Initialize(hWnd);

	return MainLoop();

}

HWND CreateGLWindow(LPSTR strWindowName, int wndWidth, int wndHeight, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance) {
	
	HWND hWnd;
    WNDCLASS wndClass;

	// Register window style

	memset(&wndClass, 0,sizeof(WNDCLASS));	
	wndClass.style =			CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc =		WinProc;
	wndClass.hInstance =		hInstance;
	wndClass.hIcon =			LoadIcon(NULL, IDI_APPLICATION);	
	wndClass.hCursor =			LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground =	(HBRUSH) (COLOR_WINDOW+1);
	wndClass.lpszClassName =	WIND_CLASS;
    wndClass.cbClsExtra   = 0;
    wndClass.cbWndExtra   = 0;
	wndClass.hbrBackground  = NULL;

	RegisterClass(&wndClass);							// Register window class

	if( bFullScreen && !dwStyle) { 						// Check full screen mode option

		dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

		ToggleFullScreen();								// Go to full screen
		ShowCursor(FALSE);								// Hide the cursor
	}
	else if (!dwStyle)
		dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	
	g_hInstance = hInstance;
	
	RECT rWindow;
	rWindow.left	= 0;
	rWindow.right	= wndWidth;
	rWindow.bottom	= wndHeight;
	rWindow.top		= 0;

	AdjustWindowRect(&rWindow, dwStyle, false);

	hWnd = CreateWindow(WIND_CLASS,
		strWindowName,
		dwStyle,
		0,
		0,
		rWindow.right - rWindow.left, 
		rWindow.bottom - rWindow.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	if(!hWnd) return NULL;	

		g_hDC = GetDC(g_hWnd);

	// Display the window
	ShowWindow(hWnd,SW_SHOWNORMAL);	
	UpdateWindow(hWnd);

	SetFocus(hWnd);

	return hWnd;
	
}


void ToggleFullScreen() {
	
	DEVMODE dmSettings;
	memset(&dmSettings,0,sizeof(dmSettings));
	
	if(!EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&dmSettings)) {
		MessageBox(NULL, "Could Not Enum Display Settings", "Error", MB_OK);
		return;
	}
	
	dmSettings.dmPelsWidth	= FS_WIDTH;					// Selected Screen Width
	dmSettings.dmPelsHeight	= FS_HEIGHT;				// Selected Screen Height
	dmSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	int result = ChangeDisplaySettings(&dmSettings,CDS_FULLSCREEN);	
	
	if(result != DISP_CHANGE_SUCCESSFUL) {
		MessageBox(NULL, "Display Mode Not Compatible", "Error", MB_OK);
		PostQuitMessage(0);
	}
}

bool bSetupPixelFormat(HDC hDC) 
{ 
    PIXELFORMATDESCRIPTOR pfd; 
    int pixelformat; 
 
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);			// Set the size of the structure
    pfd.nVersion = 1;
	
	// Pass in the appropriate OpenGL flags
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; 
    
	pfd.dwLayerMask  = PFD_MAIN_PLANE;					// Standard layer mask
    pfd.iPixelType   = PFD_TYPE_RGBA;					// Set the pixel type to RGBA
    pfd.cColorBits   = SCREEN_DEPTH;					// Set the color bits
    pfd.cDepthBits   = SCREEN_DEPTH;					// Set the depth bits
    pfd.cAccumBits   = 0;								// Set bitplanes
    pfd.cStencilBits = 0;								// Set stencil bits

	// Determines a pixel format that best matches the one passed in from the device
    if ( (pixelformat = ChoosePixelFormat(hDC, &pfd)) == FALSE ) 
    { 
        MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK); 
        return FALSE; 
    } 
 
	// Sets the pixel format extracted from above
    if (SetPixelFormat(hDC, pixelformat, &pfd) == FALSE) 
    { 
        MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK); 
        return FALSE; 
    } 
 
    return TRUE;
}


void SizeOpenGLScreen(int wndWidth, int wndHeight)			// Initialize The GL Window
{
	if (wndHeight==0)										// Prevent a dividend of 0
	{
		wndHeight=1;
	}

	if (g_bFullScreen == true) {
		glViewport(0,0,FS_WIDTH,FS_HEIGHT);
	}
	else if (g_bFullScreen == false){
		glViewport(0,0,wndWidth,wndHeight);				// Make viewport the whole window
	}

	glMatrixMode(GL_PROJECTION);						// Select projection matrix
	glLoadIdentity();									// Reset the projection matrix

	// Calculate the window's aspect ratio
	gluPerspective(35.0f,(GLfloat)wndWidth/(GLfloat)wndHeight, 1.0f, 550.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	
	glLoadIdentity();									// Reset The Modelview Matrix

	g_hDC = GetDC(g_hWnd);
	BitBlt(g_hDC,0,0,loadingBitmap.getWidth(),loadingBitmap.getHeight(),
	loadingBitmap.getHDC(),0,0,SRCCOPY);

	loadingBitmap.~CBitmap();
}

/* Not Complete Needs SetupPixelFormat */
void InitializeOpenGL( int wndWidth, int wndHeight) {

	g_hDC = GetDC(g_hWnd);
	
	int bitmapWidth = loadingBitmap.getWidth();
	int bitmapHeight = loadingBitmap.getHeight();

	// Try and load the image
	loadingBitmap.loadBMP("loadingScreenSmall.bmp",g_hDC);		
	
	// TODO: error check 	if(bitmap.loadBMP(IMAGE_NAME,win_hdc) == false)
	// Will do that when I come across this again - Blair

	// Display the bitmap at the current window width and height

	//if (g_bFullScreen = true){ 
	//	bitmapWidth = FS_WIDTH;
	//	bitmapHeight = FS_HEIGHT;
	//}
	//else{
	//	bitmapWidth = WIND_WIDTH;
	//	bitmapHeight = WIND_HEIGHT;
	//}

		
	if (!bSetupPixelFormat(g_hDC))
        PostQuitMessage (0);

	g_hRC = wglCreateContext(g_hDC);
	wglMakeCurrent(g_hDC, g_hRC);

	glEnable(GL_DEPTH_TEST);

	SizeOpenGLScreen(wndWidth, wndHeight);

}

void DeInitialize() {
	if (g_hRC) {											
	
		wglMakeCurrent(NULL, NULL);						// Frees rendering memory and sets everything back to normal
		wglDeleteContext(g_hRC);						// Delete OpenGL rendering context	
	}
	
	if (g_hDC) 
		ReleaseDC(g_hWnd, g_hDC);						// Release device context handle from memory
		
	if(g_bFullScreen) {									// In full screen?
	
		ChangeDisplaySettings(NULL,0);					// If so, switch back to the desktop
		ShowCursor(TRUE);								// Show mouse pointer
	}

	UnregisterClass(WIND_CLASS, g_hInstance);			// Free the window class

	PostQuitMessage (0);								// Post a QUIT message to the window
}

/***************************************************************************************
*																					   *
* Function: readConfigFile															   *
* Parameters: string profileName													   *
* Description: Determines the path of the configuration file using the profile name    *
* passed in. Uses the path to read the configuration set for that profile into global  *
* variables for screen depth, window width/ height, full screen mode, and sound on/off *
*																					   *
***************************************************************************************/
char readConfigFile(string profileName){

	// TODO: we can derive the true directory from argv[0] later on

   char directory [48] = "C:\\game\\Profiles\\";	
   ifstream inFile;
   
   int i = 0;							// Position in the fullPath array
   
   // Concatenate the directory, profile name and extension

   for (i = 0; directory[i]; i++){
	   fullPath[i] = directory[i];
   }

   for (int k = 0; profileName[k]; k++){
	   fullPath[i] = profileName[k];
	   i++;
   }

   fullPath[i] = '.'; i++;				// Add the file extension to the char array
   fullPath[i] = 'c'; i++;
   fullPath[i] = 'f'; i++;
   fullPath[i] = 'g';


   inFile.open(fullPath);				// Open the path we just created
 
   if(inFile.fail()){					// Failed to open the designated file
		MessageBox(NULL, "Could not find configuration file", 
						 "Config Not Found", MB_OK);
   }


   unsigned short int fullScreenOn = 0;

   inFile >> SCREEN_DEPTH;						// read in the color depth
   inFile >> WIND_WIDTH;						// the screens horizontal resolution
   inFile >> WIND_HEIGHT;						// the screens vertical resolution
   inFile >> fullScreenOn;						// full screen?


    /* debugging shit 

    char mbBuffer[255];

	sprintf(mbBuffer, "SCREEN_DEPTH = %i", SCREEN_DEPTH);
	MessageBox(NULL,mbBuffer,"TEST",MB_OK);

	sprintf(mbBuffer, "WIND_WIDTH = %i", WIND_WIDTH);
	MessageBox(NULL,mbBuffer,"TEST",MB_OK);

	sprintf(mbBuffer, "WIND_HEIGHT = %i", WIND_HEIGHT);
	MessageBox(NULL,mbBuffer,"TEST",MB_OK);


	sprintf(mbBuffer, "fullScreenOn = %i", fullScreenOn);
	MessageBox(NULL,mbBuffer,"TEST",MB_OK);

	end debugging shit */
    
   if (fullScreenOn == 1){g_bFullScreen = true;}
   else if (fullScreenOn == 0){g_bFullScreen = false;}


   inFile.close();

   return fullPath[256];
} 




void CreateTexture(UINT textureArray[], LPSTR strFileName, int textureID)
{
	// TODO: read in jpegs

	AUX_RGBImageRec *pBitmap = NULL;
	
	if(!strFileName)									// Return from the function if no file name was passed in
		return;

	pBitmap = auxDIBImageLoad(strFileName);				// Load the bitmap and store the data

	// Generate a texture with the associative texture ID stored in the array
	glGenTextures(1, &textureArray[textureID]);

	// This sets the alignment requirements for the start of each pixel row in memory.
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

	// Bind the texture to the texture arrays index and initiate the texture
	glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);

	// Build Mipmaps (builds different versions of the picture for distances - looks better)
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data);

	// todo: Change MIN filter to GL_LINEAR_MIPMAP_NEAREST if the computer chugs, or set in global config.
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

	// Free the bitmap data loaded since openGL stored it as a texture

	if (pBitmap)
	{
		if (pBitmap->data)								// If there is texture data
		{
			free(pBitmap->data);						// Free the texture data, no longer required
		}

		free(pBitmap);									// Free the bitmap structure
	}

// Enable Fog for Faster Object Rendering & so Distant Objects Fade Gently into View
		
	glEnable(GL_FOG);{						//	Turn on Fog
		GLfloat fogColor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
		glFogi (GL_FOG_MODE, GL_EXP2);
		glFogfv (GL_FOG_COLOR, fogColor);				
		glFogf (GL_FOG_DENSITY, 0.001f);	// How Dense Will The Fog Be
		glFogf (GL_FOG_START, 1.0f);		// Fog Start Distance from Camera
		glFogf (GL_FOG_END, 10.0f);			// The Viewing Distance
	}

}
