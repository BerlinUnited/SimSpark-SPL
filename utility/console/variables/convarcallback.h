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
#ifndef _CONVARCALLBACK_H_
#define _CONVARCALLBACK_H_

/*! \class ConVarCallBack
 $Id: convarcallback.h,v 1.1 2002/08/12 17:13:10 fruit Exp $

    ConVarCallBack - Call Back Functionality for Console Variables

    This is the abstract base class of the classes used for the call
    back functionality of a console variable (ConVar).

    A console variable may store a pointer to the method CallBack of
    ConVarCallBack or a subclass of ConVarCallBack and an index.

    The functionality is thus parameterized by the index parameter and
    one ConVarCallBack object may provide arbitrary many
    functionalities.

    HISTORY:
    The console/variable subsystem was taken from a student project at
    the AI Research Group, Koblenz University. Original development by
    Alexander Fuchs <alexf@uni-koblenz.de>,
    Marco Koegler <koegler@uni-koblenz.de>, 
    Marcus Rollmann <rollmark@uni-koblenz.de>, et.al.
*/

class ConVarCallBack
{
public:
	virtual ~ConVarCallBack() {};
	
	/*! The entry point for all callback functionality.
	    Something should be done dependend on the index.
	*/
	virtual void callBack(int index) = 0;
};

#endif // _CONVARCALLBACK_H_
