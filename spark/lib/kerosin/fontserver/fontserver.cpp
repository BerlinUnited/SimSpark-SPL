/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id$

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#include "fontserver.h"
#include "font.h"
#ifndef Q_MOC_RUN
#include <boost/scoped_array.hpp>
#endif
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

boost::shared_ptr<kerosin::Font> FontServer::GetFont(const string &name, unsigned int size)
{
    boost::shared_ptr<kerosin::Font> theFont = FindFont(name, size);

    if(theFont.get() != 0)
        {
            return theFont;
        }

    // we don't have a cached entry, so we have to create
    // a new font object and insert it into our registry
    theFont.reset(new Font(*this));

    if(! LoadFont(name, size, theFont))
        {
            return boost::shared_ptr<kerosin::Font>();
        }

    // insert the value in our registry
    mFonts.push_back(theFont);
    return theFont;
}

boost::shared_ptr<kerosin::Font> FontServer::FindFont(const string &name,
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

    return boost::shared_ptr<kerosin::Font>();
}

bool FontServer::LoadFont(const string &name, unsigned int size,
                          boost::shared_ptr<kerosin::Font> &font)
{
    boost::shared_ptr<salt::RFile> file = GetFile()->OpenResource(name);

    if (file.get() == 0)
        {
            GetLog()->Error() << "(FontServer) ERROR: font file '"
                              << name << "' not found\n";
            return false;
        }

    unsigned int fileSize = file->Size();
    scoped_array<unsigned char> buffer(new unsigned char[fileSize]);
    file->Read(buffer.get(), fileSize);

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
