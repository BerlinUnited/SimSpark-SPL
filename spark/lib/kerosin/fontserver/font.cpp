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
#include "font.h"
#include <stdio.h>
#include <stdarg.h>
#include <salt/gmath.h>
#ifndef Q_MOC_RUN
#include <boost/scoped_array.hpp>
#endif
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/scriptserver/scriptserver.h>
#include "fontserver.h"

#if defined(_WIN32) && !defined(APIENTRY) && !defined(__CYGWIN__)
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#endif


using namespace salt;

kerosin::Font::Font(FontServer &fontServer)
  : mTexID(0), mRowHeight(0), mSize(0), mFontServer(fontServer)
{
}

kerosin::Font::~Font()
{
}

bool 
kerosin::Font::Init(const std::string &name, unsigned int size, FT_Face face)
{
    mName = name;
    mSize = size;
    if (mTexID != 0)
    {
        glDeleteTextures(1, &mTexID);
        mTexID = 0;

        GLenum error = glGetError();
        if (error)
        {
            mFontServer.GetLog()->Debug()
                << "(Font) ERROR: glGetError() reports error " << error
                << " after deleting my font texture\n";
            return false;
        }
    }

    int dpiHRes, dpiVRes;
    bool getConfig =
        (
         mFontServer.GetScript()->GetVariable("Viewport.DpiHRes", dpiHRes) &&
         mFontServer.GetScript()->GetVariable("Viewport.DpiVRes", dpiVRes)
         );

    if (! getConfig)
    {
        mFontServer.GetLog()->Debug()
            << "(Font) ERROR: cannot read Viewport dpi values from ScriptServer\n";
        return false;
    }

    FT_Set_Char_Size(
                     face,        /* handle to face object           */
                     0,           /* char_width in 1/64th of points  */
                     64*size,     /* char_height in 1/64th of points */
                     dpiHRes,     /* horizontal device resolution    */
                     dpiVRes      /* vertical device resolution      */
                     );

    // x resolution of font texture
    unsigned int x = 256;
    // y resolution of font texture ... to be determined
    unsigned int y = 0;
    // the current maximum height
    unsigned int curMaxHeight = 0;
    // we also want to determine the (maximum) height of a row
    mRowHeight = 0;

    // here we will store the maximum height of each row
    std::vector<unsigned int> heights;

    // first pass to determine dimensions
    Glyph glyph;

    for(int i=32; i<128; ++i)
    {
        if (! glyph.LoadGlyph(face, i))
        {
            mFontServer.GetLog()->Debug()
                << "(Font) ERROR: LoadGlyph() failed in first pass\n";
            return false;
        }

        curMaxHeight = gMax(curMaxHeight, glyph.mByteHeight);

        if(x+glyph.mByteWidth > 256)
        {
            // move to next row
            x = glyph.mByteWidth+1;
            y += curMaxHeight+1;
            mRowHeight = gMax(mRowHeight, curMaxHeight);
            heights.push_back(curMaxHeight);
            curMaxHeight = 0;
        } else
        {
            x+= glyph.mByteWidth+1;
        }
    }

    // calculate the texture height (has to be power of two)
    unsigned int requiredHeight = 1;

    y+=1;
    while(requiredHeight < y)
    {
        requiredHeight*=2;
    }

    // setup image buffer now we create a texture that is big enough for
    // this data
    const unsigned int bufferSize = 256*requiredHeight;
    boost::scoped_array<unsigned char> imageBuffer(new unsigned char[bufferSize]);

    // clear the image
    memset(imageBuffer.get(), 0x00, bufferSize*sizeof(unsigned char));

    // copy glyphs to the imageBuffer
    x=0;
    y=0;
    curMaxHeight = 0;

    // loop through our characters
    for (int i=32; i<128; ++i)
    {
        if (glyph.LoadGlyph(face, i) == false)
        {
            mFontServer.GetLog()->Debug()
                << "(Font) ERROR: LoadGlyph() failed in second pass\n";
            return false;
        }

        curMaxHeight = gMax(curMaxHeight, glyph.mByteHeight);

        if(x+glyph.mByteWidth+1>256)
        {
            x=0;
            y += curMaxHeight+1;    // height of a row of characters plus 2 pixels space
            curMaxHeight = 0;
        }

        if (glyph.mByteWidth*glyph.mByteHeight > 0)
        {
            for (unsigned int copyY = 0; copyY < glyph.mByteHeight; ++copyY)
            {
                for (unsigned int copyX = 0; copyX < glyph.mByteWidth; ++copyX)
                {
                    unsigned int cto = (y+1+copyY)*256 + x+1+copyX;
                    if (cto < bufferSize)
                    {
                        imageBuffer[(y+1+copyY)*256 + x+1+copyX] = glyph.mData[copyY*glyph.mByteWidth + copyX];
                    }
                    else
                    {
                        static int warning = 0;
                        if (i != warning)
                        {
                            warning = i;
                            mFontServer.GetLog()->Warning() << "(Font) WARNING: Init font: skipping character no. " << i << "\n";
                        }
                    }
                }
            }
            mMetrics[i-32].mByteWidth     = glyph.mByteWidth;
            mMetrics[i-32].mByteHeight    = glyph.mByteHeight;
            mMetrics[i-32].mXOffset       = glyph.mXOffset;
            mMetrics[i-32].mYOffset       = glyph.mYOffset;
            mMetrics[i-32].mAdvance       = glyph.mAdvance;
            mMetrics[i-32].mTC1.Set((x+0.5f)/256.0f, (y+0.5f)/(float)requiredHeight);
            mMetrics[i-32].mTC2.Set((x+1.5f+glyph.mByteWidth)/256.0f, (y+1.5f+glyph.mByteHeight)/(float)requiredHeight);
        }

        x += glyph.mByteWidth+1;
    }

    // Spacing of a blank should be similar to 'i'
    mMetrics[' '-32].mAdvance  = mMetrics['i'-32].mAdvance;

    glGenTextures(1, &mTexID);

    GLenum error = glGetError();
    if (error)
    {
        mFontServer.GetLog()->Debug()
            << "(Font) ERROR: glGetError() reports error " << error
            << " after generating my font texture()\n";
        mTexID = 0;
        return false;
    }

    glBindTexture(GL_TEXTURE_2D, mTexID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA8, 256, requiredHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, imageBuffer.get());
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    error = glGetError();
    if (error)
    {
        mFontServer.GetLog()->Debug()
            << "(Font) ERROR: glGetError() reports error " << error
            << " after binding my font texture()\n";
        return false;
    }

    return true;
}

bool 
kerosin::Font::Bind(int vRows)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, mTexID);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    //    int xRes;
    //    int yRes;

    //if (!mFontServer.GetScript()->GetVariable("Viewport.xRes", xRes)) return false;
    //if (!mFontServer.GetScript()->GetVariable("Viewport.yRes", yRes)) return false;

    if (vRows == -1)
    {
        glOrtho(0, 1024, 768, 0, 1, 1000);
    }
    else
    {
        glOrtho(0, 1024*(mRowHeight*vRows/(float)768), mRowHeight*vRows, 0, 1, 1000);
    }

    return true;
}

void 
kerosin::Font::Dump()
{
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(0,0);
    glTexCoord2f(1, 0);
    glVertex2f(255, 0);
    glTexCoord2f(1, 1);
    glVertex2f(255, 255);
    glTexCoord2f(0, 1);
    glVertex2f(0, 255);
    glEnd();
}

void 
kerosin::Font::DrawString(float x, float y, const char *string)
{
    const char *c = string;

    float curx;
    float cury;
    y += mRowHeight;
    while (*c != 0)
    {
        int i = *c-32;            // rebase the index
        if (i>=0 && i<96)
        {
          GlyphMetric &metric = mMetrics[i];

          curx = x;
          cury = y;
          curx += metric.mXOffset;
          cury -= metric.mYOffset;
          glBegin(GL_QUADS);
          glTexCoord2f(metric.mTC1.x(), metric.mTC1.y());
          glVertex2f(curx,cury);
          glTexCoord2f(metric.mTC2.x(), metric.mTC1.y());
          glVertex2f(curx+metric.mByteWidth, cury);
          glTexCoord2f(metric.mTC2.x(), metric.mTC2.y());
          glVertex2f(curx+metric.mByteWidth, cury+metric.mByteHeight);
          glTexCoord2f(metric.mTC1.x(), metric.mTC2.y());
          glVertex2f(curx, cury+metric.mByteHeight);
          glEnd();
          x += (float)metric.mAdvance;
        }
        ++c;
    }
}

void 
kerosin::Font::Printf(float x, float y, const char *format, ...)
{
    char buffer[4096];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    DrawString(x, y, buffer);
}

void 
kerosin::Font::RowPrintf(float x, float row, const char *format, ...)
{
    char buffer[4096];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    DrawString(x, row*mRowHeight, buffer);
}

float 
kerosin::Font::GetStringWidth(const char* string, int numChar)
{
    if (numChar == -1) numChar = strlen(string);

    const char *c = string;
    float  len  = 0;
    int chCount = 0;

    while ((*c != 0) && (chCount < numChar))
    {
        int i = *c-32;            // rebase the index
        if (i>=0 && i<96)
        {
            GlyphMetric &metric = mMetrics[i];
            len += metric.mAdvance;
        }
        ++c;
        ++chCount;
    }

    return len;
}

float 
kerosin::Font::GetRowHeight()
{
    return (float) mRowHeight;
}

const std::string& 
kerosin::Font::GetName() const
{
    return mName;
}

unsigned int 
kerosin::Font::GetSize() const
{
    return mSize;
}
