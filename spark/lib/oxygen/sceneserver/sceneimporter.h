/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
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
#ifndef OXYGEN_SCENEIMPORTER
#define OXYGEN_SCENEIMPORTER

#include <zeitgeist/class.h>
#include <zeitgeist/leaf.h>
#include <salt/fileclasses.h>

namespace oxygen
{
class BaseNode;
class SceneDict;

class SceneImporter : public zeitgeist::Leaf
{
public:
    SceneImporter() : zeitgeist::Leaf(), mSceneDict(0) {}
    virtual ~SceneImporter() {};

    /** import a scene from a file */
    virtual bool ImportScene(const std::string& fileName,
                             boost::shared_ptr<BaseNode> root,
                             boost::shared_ptr<zeitgeist::ParameterList> parameter) = 0;

    /** import a scene from a string description */
    virtual bool ParseScene(const std::string& scene,
                            boost::shared_ptr<BaseNode> root,
                            boost::shared_ptr<zeitgeist::ParameterList> parameter) = 0;


    /** sets up the reference to the SceneDict instance */
    void SetSceneDict(SceneDict* dict) { mSceneDict = dict; }

protected:
    SceneDict* mSceneDict;
};

DECLARE_ABSTRACTCLASS(SceneImporter);

} // namespace oxygen

#endif // OXYGEN_SCENEIMPORTER
