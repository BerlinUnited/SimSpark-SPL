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
#ifndef UTILITY_CONVARCALLBACKCONSOLE_H
#define UTILITY_CONVARCALLBACKCONSOLE_H

#include "convarcallback.h"

/*! \class ConVarCallBackConsole  
  $Id: convarcallbackconsole.h,v 1.2 2002/08/21 08:18:39 fruit Exp $

    ConVarCallBackConsole

    The console's call back functionality.

    Allows the change of the message interests of the console,
    i.e. which message types should be displayed.

    HISTORY:
    The console subsystem was taken from a student project at the AI
    Research Group, Koblenz University. Original development by Marco
    Koegler <koegler@uni-koblenz.de>, Marcus Rollmann
    <rollmark@uni-koblenz.de>, Alexander Fuchs <alexf@uni-koblenz.de>,
    et.al.
*/
class ConVarCallBackConsole : public ConVarCallBack
{
public:
    //! the call back method
    virtual void callBack(int index);
};

#endif                          // UTILITY_CONVARCALLBACKCONSOLE_H
