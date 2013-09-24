#ifndef	BITMAP_CLASS_H
#define	BITMAP_CLASS_H

#include <windows.h>
#include <stdio.h>

typedef unsigned char UCHAR;

class CBitmap
{
	public:

		// Constructor
		CBitmap():hbitmap(NULL),old_bmp(NULL),bmp_height(0),bmp_width(0),
				  bmp_channels(0),bmp_stride(0),surface_bits(NULL),bmp_hdc(NULL)
				  { GdiFlush(); }

		
		// Data Access Functions ******
		
			inline int getWidth()    const { return bmp_width; } 
			inline int getHeight()   const { return bmp_height; }
			inline int getChannels() const { return bmp_channels; }
			inline int getStride()   const { return bmp_stride; }
			inline HDC getHDC()      const { return bmp_hdc; }
		
		// ****** End of Data Access Functions

		// Initializes member variable "hbitmap"
		bool init(HDC hdc, int width, int height, int channels);

		bool loadBMP(char *file_name, HDC hdc); // Loads a bmp with specified file_name
		UCHAR* getLinePtr(int which_line);		// returns a pointer to a line of pixels specified by which_line
		~CBitmap();								// Destructor - Free memory
		

	private:

		HBITMAP hbitmap;     // Bitmap to be created
		HBITMAP old_bmp;     // Bitmap for holding the "old bitmap"

		int bmp_height;      // Height of CBitmap
		int bmp_width;       // Width of CBitmap
		int bmp_channels;	 // Number of channels in our CBitmap (either 3 or 4)
		
		int bmp_stride;		 // CBitmap's stride
		UCHAR *surface_bits; // A pointer for holding the address of our bitmap surface bits
		HDC bmp_hdc;		 // Compatible device context handle

		void freeMem();		 // Frees all memory associated with CBitmap
					   
};

#endif
