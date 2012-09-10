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
#ifndef KEROSIN_GLBASE_H
#define KEROSIN_GLBASE_H

namespace kerosin
{
class RGBA
{
public:
    union
    {
        float v[4];
        struct
        {
            float r;
            float g;
            float b;
            float a;
        } c;
    } data;

public:
    RGBA()
    {
        data.c.r = 1.0f;
        data.c.g = 1.0f;
        data.c.b = 1.0f;
        data.c.a = 1.0f;
    }

    RGBA(float rr, float gg, float bb, float aa)
    {
        data.c.r = rr;
        data.c.g = gg;
        data.c.b = bb;
        data.c.a = aa;
    }

    float& r() { return data.c.r; }
    float& g() { return data.c.g; }
    float& b() { return data.c.b; }
    float& a() { return data.c.a; }

    const float& r() const { return data.c.r; }
    const float& g() const { return data.c.g; }
    const float& b() const { return data.c.b; }
    const float& a() const { return data.c.a; }

    float* v() { return data.v; }
    const float* v() const { return data.v; }

    operator float* () { return data.v; }
    operator const float*() const { return data.v; }
};

};

#endif // KEROSIN_GLBASE_H
