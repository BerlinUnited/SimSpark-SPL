/*
	ImageServer - Global Interface For All Image-Related Functionality

	What the ImageServer does:
		- Load/Save images
		- Create images with different formats
		- Conversion between formats

	NOTE:

	HISTORY:
		14.07.01 - MK
			- Initial version
		29.07.01 - MK
			- Uses classserver
		29.08.01 - MK
			- Doesn't use classserver anymore :(
			- Switched to DevIL for image loading needs, since the task of supporting
			  all major formats would have been too time consuming
			- Cleaned up the interface of the imageserver quite a bit
		11.10.01 - MK
			- Made singleton functionality more secure
		02.10.02 - MK
			- Moved to Kerosin

	TODO:
		- Image creation
		- Image conversion
		- Pixel-level access

	TOFIX:
*/

#ifndef IMAGESERVER_H__
#define IMAGESERVER_H__

#include <il/il.h>
#include <zeitgeist/leaf.h>

namespace kerosin
{

class Image;

class ImageServer : public zeitgeist::Leaf
{
public:
	ImageServer();

	// load/save
	boost::shared_ptr<Image>	Load(const char *inName, ILenum inType = IL_TYPE_UNKNOWN);					// interpret the file with the filter associated with inExt
	bool	Save(const boost::shared_ptr<Image> &inImage, const char *inName, ILenum inType = IL_TYPE_UNKNOWN);	// interpret the file with the filter associated with inExt

private:
	// some internal error checking
	bool	HandleErrors();
};

DECLARE_CLASS(ImageServer);

} // namespace kerosin

#endif //IMAGESERVER_H__