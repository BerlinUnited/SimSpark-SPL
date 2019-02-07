/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
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
#ifndef MAIN_H__
#define MAIN_H__

#ifdef WIN32
#include <winsock2.h>
#endif

#include <wx/glcanvas.h>
#include <wx/app.h>
#include <boost/shared_ptr.hpp>

class SimSpark;
class Property;

class RsgEditApp: public wxApp
{
public:
    bool OnInit();
    boost::shared_ptr<SimSpark> GetSpark();
    boost::shared_ptr<Property> GetProperty();

protected:
    void OnFatalException();

protected:
    boost::shared_ptr<SimSpark> mSpark;
    boost::shared_ptr<Property> mProperty;
};

DECLARE_APP(RsgEditApp)

#endif // MAIN_H__
