/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: perfectvisionperceptor.h,v 1.1.2.1 2003/12/08 14:56:30 fruit Exp $

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
#ifndef OXYGEN_PERFECTVISIONPERCEPTOR_H
#define OXYGEN_PERFECTVISIONPERCEPTOR_H

#include "perceptor.h"
#include <oxygen/sceneserver/sceneserver.h>

namespace oxygen
{

class PerfectVisionPerceptor : public oxygen::Perceptor
{
public:
    PerfectVisionPerceptor();
    ~PerfectVisionPerceptor();
    /*! \return true, if valid data is available and false otherwise.
    */
    bool Percept(BaseParser::TPredicate& predicate);

private:
    boost::shared_ptr<SceneServer> mSceneServer;
};

DECLARE_CLASS(PerfectVisionPerceptor);

} // namespace oxygen

#endif //OXYGEN_PERFECTVISIONPERCEPTOR_H
