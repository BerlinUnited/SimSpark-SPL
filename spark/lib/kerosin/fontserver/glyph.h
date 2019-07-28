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
#ifndef KEROSIN_GLYPH_H
#define KEROSIN_GLYPH_H

// strange as this may seem
#include <ft2build.h>
#include FT_FREETYPE_H

#ifndef Q_MOC_RUN
#include <boost/shared_array.hpp>
#endif
#include <kerosin/kerosin_defines.h>

namespace kerosin
{

class KEROSIN_API Glyph
{
public:
    Glyph();

    bool LoadGlyph(FT_Face face, unsigned int charCode);

    unsigned int mByteWidth;
    unsigned int mByteHeight;
    unsigned int mXOffset;
    unsigned int mYOffset;
    unsigned int mAdvance;
    boost::shared_array<unsigned char> mData;
private:
    void Reset();
};

} //namespace kerosin

#endif //KEROSIN_GLYPH_H
