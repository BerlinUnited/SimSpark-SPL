#ifndef IMAGE_H__
#define IMAGE_H__

/*	$Id: image.h,v 1.1 2003/04/30 11:29:33 fruit Exp $

	Image - A Wrapper for the DevIL Library

	NOTE:

	HISTORY:
		11.07.01 - MK
			- Initial version
		28.08.01 - MK
			- Added support for a palette
		29.08.01 - MK
			- Rewrite for DevIL
		03.09.01 - MK
			- OpenGL texture support

	TODO:
		- add RGB access
		- image creation
		

	TOFIX:
*/

#include <il/il.h>

namespace kerosin
{

class Image
{
public:
	// constructor/destructor
	Image();
	virtual ~Image();

	// this makes the image active
	void	Bind();

	// image information
	ILuint	Width();		// width
	ILuint	Height();		// height
	ILuint	Depth();		// depth (==1 for 2d images, >1 for 3d images)

	ILuint	BitsPP();		// bits per pixel
	ILuint	BytesPP();		// bytes per pixel

	ILuint	Type();			// format of pixels 
	ILuint	Format();		// byte format of image

	ILubyte*Data();

	bool	HasAlpha();		// does the format have an alpha channel
	bool	Create(int w, int h, int b, void *data = NULL);

	// the interface functions ... these *have* to be implemented by derived classes
	//virtual bool Create() = 0;
	//virtual void SetPixel(int x, int y, long color) const = 0;
	//virtual long GetPixel(int x, int y) const = 0;
	
	/*
	virtual long MakeCol(int a, int r, int g, int b) const = 0;
	virtual void GetCol(long col, int& a, int& r, int& g, int& b) const = 0;
	virtual int	 GetA(long col) const = 0;
	virtual int	 GetR(long col) const = 0;
	virtual int	 GetG(long col) const = 0;
	virtual int	 GetB(long col) const = 0;

	// accessors
	f_inline void SetWidth (int inWidth)	{	mWidth  = inWidth;	}
	f_inline void SetHeight(int inHeight)	{	mHeight = inHeight;	}
*/
protected:
	ILuint		mId;			// the DevIL ID which this image is bound to
};

} // namespace kerosin

#endif //IMAGE_H__