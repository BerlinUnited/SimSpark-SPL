/* -*- mode: c++ -*-
   
   this file is part of Soccer Server 3D
   Copyright (C) 2002 Soccer Server Maintainance Group

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
 
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#ifndef UTILITY_CONSOLEBASEVIEW_H
#define UTILITY_CONSOLEBASEVIEW_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/*! \class ConsoleBaseView 
  $Id: consolebaseview.h,v 1.1 2002/08/21 14:11:16 fruit Exp $

    ConsoleBaseView

    A view of a console, which only has to provide a draw method.
    An instance should be registered to the console with 
    Console::RegisterView.  

    HISTORY:
    The console subsystem was taken from a student project at the AI
    Research Group, Koblenz University. Original development by Marco
    Koegler <koegler@uni-koblenz.de>, Markus Rollmann
    <rollmark@uni-koblenz.de>, Alexander Fuchs <alexf@uni-koblenz.de>,
    et.al.
*/
class ConsoleBaseView
{
public:
    //! will be called by the console if a representation is wanted
    virtual void draw() = 0;
};

#endif                          // UTILITY_CONSOLEBASEVIEW_H
