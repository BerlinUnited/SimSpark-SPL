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
using namespace boost;

FontServer::FontServer()
{
  FT_Init_FreeType(&mFreeTypeLib);
}

FontServer::~FontServer()
{
  mFonts.clear();
}

shared_ptr<kerosin::Font> FontServer::GetFont(const string &name, unsigned int size)
{
  shared_ptr<kerosin::Font> theFont = FindFont(name, size);

  if(theFont.get() != 0)
    {
      return theFont;
    }

  // we don't have a cached entry, so we have to create
  // a new font object and insert it into our registry
  theFont.reset(new Font(*this));

  if(! LoadFont(name, size, theFont))
    {
      return shared_ptr<kerosin::Font>();
    }

  // insert the value in our registry
  mFonts.push_back(theFont);
  return theFont;
}

shared_ptr<kerosin::Font> FontServer::FindFont(const string &name,
                                               unsigned int size) const
{
  for (TFontList::const_iterator i = mFonts.begin(); i != mFonts.end(); ++i)
    {
      if (
          ((*i)->GetName().compare(name) == 0) &&
          ((*i)->GetSize() == size)
          )
        {
          return (*i);
        }
    }

  return shared_ptr<kerosin::Font>();
}

bool FontServer::LoadFont(const string &name, unsigned int size,
                          shared_ptr<kerosin::Font> &font)
{
  salt::RFile *file = GetFile()->Open(name.c_str());

  if (file == 0)
    {
      // try with prefixed fontPath
      string fontPath;
      if(GetScript()->GetVariable("System.FontPath", fontPath))
        {
          file = GetFile()->Open((fontPath+name).c_str());
        }
    }

  if (! file)
    {
      GetLog()->Error() << "(FontServer) ERROR: font file '"
                        << name << "' not found\n";
      return false;
    }

  unsigned int fileSize = file->Size();
  scoped_array<unsigned char> buffer(new unsigned char[fileSize]);
  file->Read(buffer.get(), fileSize);
  delete file;

  FT_Face face;
  int error = FT_New_Memory_Face(mFreeTypeLib, buffer.get(), fileSize,
                                 0, &face);

  if (error == FT_Err_Unknown_File_Format)
    {
      GetLog()->Error() << "(FontServer) ERROR: Unknown file format\n";
    }
  else if (error)
    {
      GetLog()->Error() << "(FontServer) ERROR: Could not create face\n";
    }

  if (error)
    {
      FT_Done_Face(face);
      return false;
    }

  bool ok = font->Init(name, size, face);
  FT_Done_Face(face);

  return ok;
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
