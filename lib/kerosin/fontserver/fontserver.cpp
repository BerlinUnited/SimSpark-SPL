#include "fontserver.h"
#include "font.h"
#include <boost/scoped_array.hpp>
#include <salt/fileclasses.h>
#include <zeitgeist/fileserver/fileserver.h>
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/scriptserver/scriptserver.h>
#include <kerosin/openglserver/openglserver.h>

using namespace kerosin;
using namespace std;
//------------------------------------------------------------------------------------------------
// FontServer implementation
//------------------------------------------------------------------------------------------------

FontServer::FontServer()
{
	FT_Init_FreeType(&mFreeTypeLib);
}

FontServer::~FontServer()
{
	mFonts.clear();
}

boost::shared_ptr<kerosin::Font> FontServer::GetFont(const std::string &name, unsigned int size)
{
	boost::shared_ptr<kerosin::Font> theFont = FindFont(name, size);

	if(theFont.get() == NULL)
	{
		// we don't have a cached entry, so we have to create
		// a new font object and insert it into our registry
		theFont.reset(new Font(*this));
		if(LoadFont(name, size, theFont) == true)
		{
			// insert the value in our registry
			mFonts.push_back(theFont);
			return theFont;
		}
		else
		{
			return boost::shared_ptr<kerosin::Font>();
		}
	}

	return theFont;
}

boost::shared_ptr<kerosin::Font> FontServer::FindFont(const std::string &name, unsigned int size) const
{
	//GetLog()->Normal() << "FontServer: Finding " << name << " " << size << endl;
	for (TFontList::const_iterator i = mFonts.begin(); i != mFonts.end(); ++i)
	{
		//GetLog()->Normal() << "FontServer: Comparing " << (*i)->GetName() << " " << (*i)->GetSize() << endl;
		if ((*i)->GetName().compare(name) == 0 && (*i)->GetSize() == size)
		{
			return (*i);
		}
	}

	return boost::shared_ptr<kerosin::Font>();
}

bool FontServer::LoadFont(const std::string &name, unsigned int size, boost::shared_ptr<kerosin::Font> &font)
{
	GetLog()->Normal() << "FontServer: Loading font file -> " << name << endl;
	salt::RFile *file = GetFile()->Open(name.c_str());

	if (file == NULL)
	{
		// try with prefixed fontPath
		std::string fontPath;
		if(GetScript()->GetVariable("System.fontPath", fontPath))
		{
			file = GetFile()->Open((fontPath+name).c_str());
		}
	}

	if (!file) return false;

	unsigned int fileSize = file->Size();
	boost::scoped_array<unsigned char> buffer(new unsigned char[fileSize]);
	file->Read(buffer.get(), fileSize);
	delete file;

	FT_Face face;
	int error = FT_New_Memory_Face( mFreeTypeLib,
                                buffer.get(),	
                                fileSize,
                                0,
                                &face );

	if (error == FT_Err_Unknown_File_Format)
	{
		GetLog()->Error() << "ERROR: Unknown file format" << endl;
	}
	else if (error)
	{
		GetLog()->Error() << "ERROR: Could not create face" << endl;
	}

	if (error)
	{
		FT_Done_Face(face);
		return false;
	}

	bool ret = font->Init(name, size, face);
	FT_Done_Face(face);
	return ret;
}

void FontServer::Begin()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0,0,-1);
	glColor4f(1,1,1,1);
}

void FontServer::End()
{
	glDisable(GL_BLEND);
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}
