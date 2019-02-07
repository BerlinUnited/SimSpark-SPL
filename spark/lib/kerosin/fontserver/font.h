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
#ifndef KEROSIN_FONT_H
#define KEROSIN_FONT_H

#include <string>
#include <salt/vector.h>
#include <kerosin/kerosin_defines.h>
#include <kerosin/openglserver/openglwrapper.h>
#include "glyph.h"

namespace kerosin
{

class FontServer;

/** Font allows the use of a 'texture'-based font. The font is loaded
    from Fluid Studios Font Files, which can be generated with a small
    utility from any Windows font. Care has to be taken, that the
    resulting font will fit in a 256x256 texture! Our font will only
    contain characters for the ASCII value range 32-128 ... this
    should cover the major alphanumeric characters!

        NOTE:

        HISTORY:
                08.10.01 - MK
                        - Initial version
                12.10.01 - MK
                        - This now supports fonts generated with Fluid Studios Font Generator
                16.10.01 - MK
                        - Calculates minimum texture size required
                        - Only needs one pixel in-between characters when placing them in the texture
                16.10.01 - MR
                        - Added support to calculate the width of a string printed with this font
                02.10.02 - MK
                        - Moved to Kerosin (major changes)

        TODO:
                - better texture usage

        TOFIX:
                - ImageServer activation hack (see FIXME in cpp file)

 */
class KEROSIN_API Font
{
private:
    struct GlyphMetric
    {
        unsigned int   mByteWidth;
        unsigned int   mByteHeight;
        unsigned int   mXOffset;
        unsigned int   mYOffset;
        unsigned int   mAdvance;
        salt::Vector2f mTC1;
        salt::Vector2f mTC2;
    };
public:
    Font(FontServer &fontServer);
    ~Font();

    bool    Init(const std::string &name, unsigned int size, FT_Face face);
    bool    Bind(int vRows = -1);
    void    Dump();
    void    DrawString(float x, float y, const char *string);
    void    Printf(float x, float y, const char *format, ...);
    void    RowPrintf(float x, float row, const char *format, ...);

    /**!  calculates the width of a string, printed with this
          font. Set numChar to a value between 1 and strlen(string) to
          calculate intermediate string lengths. A value of -1 (which
          is the default value) calculates the width if the whole
          string
    */
    float   GetStringWidth(const char* string, int numChar = -1);

    //! returns the height in pixels of a row
    float GetRowHeight();

    //! returns the name of the font
    const std::string& GetName() const;

    //! returns the size of the font
    unsigned int GetSize() const;

private:
    //! the metrics of all glyphs
    GlyphMetric  mMetrics[96];

    //! OpenGL Texture ID
    GLuint mTexID;

    //! height (in pixels) of a row
    unsigned int mRowHeight;

    //! font name
    std::string mName;

    //! size of font
    unsigned int mSize;

    //! reference to the fontserver
    FontServer& mFontServer;
};

} // namespace kerosin

#endif //KEROSIN_FONT_H
