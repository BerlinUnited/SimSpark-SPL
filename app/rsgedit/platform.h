/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: platform.h,v 1.2 2008/02/27 09:22:39 rollmark Exp $

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
#ifndef RSGEDIT_PLATFORM_H
#define RSGEDIT_PLATFORM_H

#include <wx/string.h>
#include <wx/font.h>

class Platform
{
public:
    static wxString GetDefaultFileOpenPath();

    static wxString GetDefaultAgentOpenPath();
    static wxString GetDefaultAgentOpenWildcard();

protected:
    static wxString GetHomeDirectory();
};

#endif // RSGEDIT_PLATFORM_H
