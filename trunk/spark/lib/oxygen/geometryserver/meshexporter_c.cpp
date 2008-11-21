/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of simspark
   Tue May 9 2006
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2007 RoboCup Soccer Server 3D Maintenance Group
   $Id: meshexporter_c.cpp,v 1.1 2008/02/22 07:52:15 hedayat Exp $

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

   MeshExporter

   NOTE: This class serves as an (abstract) exporter, to register 
         meshes with other graphic engines (possibly also for exporting)

   HISTORY:
       30/03/07 - OO  - Initial version

*/
#include "meshexporter.h"

using namespace oxygen;

void 
CLASS(MeshExporter)::DefineClass()
{
    DEFINE_BASECLASS(zeitgeist/Leaf);
}
