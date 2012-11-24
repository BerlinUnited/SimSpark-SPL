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

#include "sceneeffector.h"
#include "sceneaction.h"
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/gamecontrolserver/actionobject.h>
#include <oxygen/agentaspect/agentaspect.h>

using namespace boost;
using namespace oxygen;
using namespace zeitgeist;
using namespace std;

SceneEffector::SceneEffector() : oxygen::Effector()
{
}

SceneEffector::~SceneEffector()
{
}

void SceneEffector::PrePhysicsUpdateInternal(float /*deltaTime*/)
{
    if ( mAction.get() == 0 )
        return;

    boost::shared_ptr<SceneAction> sceneAction =
        shared_dynamic_cast<SceneAction>(mAction);
    mAction.reset();

    if (sceneAction.get() == 0)
        {
            GetLog()->Error()
                << "(SceneEffector) ERROR: cannot realize "
                << "an unknown ActionObject\n";
            return;
        }

    boost::shared_ptr<AgentAspect> aspect =GetAgentAspect();

    if (aspect.get() == 0)
        {
            GetLog()->Error()
                << "(SceneEffector) ERROR: cannot get AgentAspect\n";
            return;
        }

    aspect->ImportScene(sceneAction->GetScene(),
        sceneAction->GetSceneParameters());

}

boost::shared_ptr<ActionObject>
SceneEffector::GetActionObject(const Predicate& predicate)
{
    if (predicate.name != GetPredicate())
        {
            GetLog()->Error() << "(SceneEffector) ERROR: invalid predicate"
                              << predicate.name << "\n";
            return boost::shared_ptr<ActionObject>();
        }

    string scene;
    if (! predicate.GetValue(predicate.begin(), scene))
        {
            GetLog()->Error()
                << "ERROR: (SceneEffector) scene filename expected\n";
            return boost::shared_ptr<ActionObject>();
        };

    boost::shared_ptr<ParameterList> parameters(
        new ParameterList(predicate.parameter));
    parameters->Pop_Front();

    return boost::shared_ptr<ActionObject>(
        new SceneAction(GetPredicate(), scene, parameters));
}
