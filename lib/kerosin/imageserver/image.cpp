#include "image.h"

using namespace kerosin;

// constructor
Image::Image()
{
	// let's create a DevIL ID for this image
	ilGenImages(1, &mId);
}

// destructor
Image::~Image()
{
	// free the image with DevIL
	ilDeleteImages(1, &mId);
}

void Image::Bind()
{
	ilBindImage(mId);
}

ILuint Image::Width()
{
	Bind();
	return ilGetInteger(IL_IMAGE_WIDTH);
}


ILuint Image::Height()
{
	Bind();
	return ilGetInteger(IL_IMAGE_HEIGHT);
}


ILuint Image::Depth()
{
	Bind();
	return ilGetInteger(IL_IMAGE_DEPTH);
}

ILuint Image::BitsPP()
{
	Bind();
	return ilGetInteger(IL_IMAGE_BITS_PER_PIXEL );
}

ILuint Image::BytesPP()
{
	Bind();
	return ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL );
}

ILuint Image::Type()
{
	Bind();
	return ilGetInteger(IL_IMAGE_TYPE);
}

ILuint Image::Format()
{
	Bind();
	return ilGetInteger(IL_IMAGE_FORMAT);
}

ILubyte* Image::Data()
{
	Bind();
	return ilGetData();
}

bool Image::HasAlpha()
{
	Bind();
	ILuint format = Format();

	switch(format)
	{
	case IL_RGB:
	case IL_BGR:
		return false;
		break;
	case IL_RGBA:
	case IL_BGRA:
		return true;
		break;
	default:
		return false;
	}
}


bool Image::Create(int w, int h, int b, void *data)
{
	Bind();
	
	if(b==3)
	{
		ilTexImage(w, h, 1, b, IL_RGB, IL_UNSIGNED_BYTE, data);
	}
	else
	{
		ilTexImage(w, h, 1, b, IL_RGBA, IL_UNSIGNED_BYTE, data);
	}
	return true;
}
