/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: perfectvisionperceptor.h,v 1.2.2.1 2003/12/25 13:18:14 rollmark Exp $

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
#ifndef PERFECTVISIONPERCEPTOR_H
#define PERFECTVISIONPERCEPTOR_H

#include <oxygen/agentaspect/perceptor.h>
#include <oxygen/sceneserver/sceneserver.h>

class PerfectVisionPerceptor : public oxygen::Perceptor
{
public:
    PerfectVisionPerceptor();
    virtual ~PerfectVisionPerceptor();

    //! \return true, if valid data is available and false otherwise.
    bool Percept(oxygen::Predicate& predicate);

private:
    boost::shared_ptr<oxygen::SceneServer> mSceneServer;
};

DECLARE_CLASS(PerfectVisionPerceptor);

#endif //PERFECTVISIONPERCEPTOR_H
