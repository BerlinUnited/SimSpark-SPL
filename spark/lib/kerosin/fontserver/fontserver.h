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
#ifndef KEROSIN_FONTSERVER_H
#define KEROSIN_FONTSERVER_H

#include <zeitgeist/leaf.h>
#include <zeitgeist/class.h>
#include <kerosin/kerosin_defines.h>
// strange as this may seem
#include <ft2build.h>
#include FT_FREETYPE_H

namespace kerosin
{

class Font;

/*      The fontserver manages Font objects. It prevents a single font
        from being loaded several times by the runtime system.

        NOTE:

        HISTORY:
                15.10.01 - MK
                        - Initial version
                02.10.02 - MK
                        - Moved to Kerosin (major changes)

        TODO:

        TOFIX:
*/
class KEROSIN_API FontServer : public zeitgeist::Leaf
{
public:
    FontServer();
    ~FontServer();

    //! load a font with a given size
    boost::shared_ptr<Font> GetFont(const std::string &name,
                                    unsigned int size = 12);

    //! test if a specific font has been loaded
    boost::shared_ptr<Font> FindFont(const std::string &name,
                                     unsigned int size) const;

    //! setup opengl states for font rendering
    void Begin();

    //! reset opengl states after font rendering
    void End();

protected:
    bool LoadFont(const std::string &name, unsigned int size,
                  boost::shared_ptr<Font> &font);

private:
    typedef std::list<boost::shared_ptr<kerosin::Font> > TFontList;

    //! the registry of loaded fonts
    TFontList       mFonts;

    //! FreeType
    FT_Library  mFreeTypeLib;
};

DECLARE_CLASS(FontServer);

} //namespace kerosin

#endif //KEROSIN_FONTSERVER_H

