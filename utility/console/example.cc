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
#include "console.h"
#include <forwarder.h>

#include <iostream>
#include <string>

int
main()
{
    flush(*Console::instance().getStream());
    Utility::smux.addStream(&cout);
    
    string s;
    long int i = 0;
    
    while (true) 
    {    
        Utility::smux.normal() << "console:/ " << i << ") " << flush;
        ++i;
        getline(std::cin,s);
        Console::instance().execute(s);
    }

    return 0;
}
