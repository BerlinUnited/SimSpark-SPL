/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: perceptor.h,v 1.4.2.1 2003/12/25 13:15:13 rollmark Exp $

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
#ifndef OXYGEN_PERCEPTOR_H
#define OXYGEN_PERCEPTOR_H

#include <oxygen/sceneserver/basenode.h>
#include <oxygen/gamecontrolserver/baseparser.h>

namespace oxygen
{

class Perceptor : public oxygen::BaseNode
{
public:
    /*!
      This is called by agents to trigger the percept event implemented by
      this perceptor. The perceptor can return data through the predicate
      which is passed as a parameter.
      \return true, if valid data is available and false otherwise.
    */
    virtual bool Percept(Predicate& predicate) = 0;
};

DECLARE_ABSTRACTCLASS(Perceptor);

} // namespace oxygen

#endif //OXYGEN_PERCEPTOR_H
