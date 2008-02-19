/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of simspark
   Wed May 9 2007
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: materialexternal.cpp,v 1.1 2008/02/19 22:49:23 hedayat Exp $

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
#include "materialexternal.h"

using namespace kerosin;

MaterialExternal::MaterialExternal()
    : MaterialSolid(),
      mName("")
{
}

MaterialExternal::~MaterialExternal()
{
}

void 
MaterialExternal::SetupMaterial()
{
  /* We don't do anything here. This overwrites the original SetupMaterial from 
     MaterialSolid, which calles OpenGl functions.
  */
}

void 
MaterialExternal::Bind()
{
  /* We don't do anything here. This overwrites the original Bind from 
     MaterialSolid, which calles OpenGl functions.
  */
}

const std::string& 
MaterialExternal::GetReference() const
{
    return mName;
}

void 
MaterialExternal::SetReference(const std::string& name)
{
    mName = name;
}
