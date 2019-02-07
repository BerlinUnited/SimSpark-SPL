/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Copyright (C) 2010 RoboCup Soccer Server 3D Maintenance Group
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
#ifndef LINE_H
#define LINE_H

#include "../soccernode/soccernode.h"

class Line : public SoccerNode
{
public:
    Line();
    virtual ~Line();

    salt::Vector3f& BeginPoint() { return mBeginPoint; }
    salt::Vector3f& EndPoint() { return mEndPoint; }
    const salt::Vector3f& BeginPoint() const { return mBeginPoint; }
    const salt::Vector3f& EndPoint() const { return mEndPoint; }

protected:

  salt::Vector3f mBeginPoint;
  salt::Vector3f mEndPoint;
};

DECLARE_CLASS(Line);

#endif // LINE_H
