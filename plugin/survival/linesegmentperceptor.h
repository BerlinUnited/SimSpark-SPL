/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: linesegmentperceptor.h,v 1.2.2.1.2.1 2003/12/08 15:16:06 fruit Exp $

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
#ifndef LINESEGMENTPERCEPTOR_H__
#define LINESEGMENTPERCEPTOR_H__

#include <oxygen/agentaspect/perceptor.h>
#include <oxygen/gamecontrolserver/baseparser.h>

class LineSegmentPerceptor : public oxygen::Perceptor
{
public:
        LineSegmentPerceptor();

        void SetLineSegment(const salt::Vector3f& start, const salt::Vector3f& end);

        /*!
                Performs a line segment intersection test with all static meshes of the
                scene.
        */
        virtual bool Percept(oxygen::BaseParser::TPredicate& predicate);
private:
        //! starting point of line segment
        salt::Vector3f  mStart;
        //! end point of line segment
        salt::Vector3f  mEnd;
};

DECLARE_CLASS(LineSegmentPerceptor);

#endif // LINESEGMENTPERCEPTOR_H__
