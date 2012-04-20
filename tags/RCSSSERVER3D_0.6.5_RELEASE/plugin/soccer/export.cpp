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

#include <zeitgeist/zeitgeist.h>
#include "beameffector/beameffector.h"
#include "catcheffector/catcheffector.h"
#include "createeffector/createeffector.h"
#include "driveeffector/driveeffector.h"
#include "initeffector/initeffector.h"
#include "initeffector/singlematiniteffector.h"
#include "initeffector/staticmeshiniteffector.h"
#include "kickeffector/kickeffector.h"
#include "pantilteffector/pantilteffector.h"
#include "sayeffector/sayeffector.h"
#include "soccercontrolaspect/soccercontrolaspect.h"
#include "gamestateaspect/gamestateaspect.h"
#include "gamestateaspect/gamestateitem.h"
#include "ballstateaspect/ballstateaspect.h"
#include "soccerruleaspect/soccerruleaspect.h"
#include "agentstate/agentstate.h"
#include "sexpmonitor/sexpmonitor.h"
#include "internalsoccermonitor/internalsoccerrender.h"
#include "internalsoccermonitor/internalsoccerinput.h"
#include "trainercommandparser/trainercommandparser.h"
#include "fieldflag/fieldflag.h"
#include "ball/ball.h"
#include "visionperceptor/visionperceptor.h"
#include "restrictedvisionperceptor/restrictedvisionperceptor.h"
#include "gamestateperceptor/gamestateperceptor.h"
#include "soccernode/soccernode.h"
#include "agentstateperceptor/agentstateperceptor.h"
#include "hearperceptor/hearperceptor.h"
#include "gametimeperceptor/gametimeperceptor.h"
#include "agentintegration/soccerbotbehavior.h"
#include "hmdp_effector/hmdpeffector.h"
#include "hmdp_effector/hmdpperceptor.h"
#include "line/line.h"
#include "agentcollisionhandler/agentcollisionhandler.h"

ZEITGEIST_EXPORT_BEGIN()
        ZEITGEIST_EXPORT(SoccerControlAspect);
        ZEITGEIST_EXPORT(GameStateAspect);
        ZEITGEIST_EXPORT(GameStateItem);
        ZEITGEIST_EXPORT(BallStateAspect);
        ZEITGEIST_EXPORT(SoccerRuleAspect);
        ZEITGEIST_EXPORT(BeamEffector);
        ZEITGEIST_EXPORT(CatchEffector);
        ZEITGEIST_EXPORT(CreateEffector);
        ZEITGEIST_EXPORT(DriveEffector);
        ZEITGEIST_EXPORT(InitEffector);
        ZEITGEIST_EXPORT(SingleMatInitEffector);
        ZEITGEIST_EXPORT(StaticMeshInitEffector);
        ZEITGEIST_EXPORT(KickEffector);
        ZEITGEIST_EXPORT(PanTiltEffector);
        ZEITGEIST_EXPORT(SayEffector);
        ZEITGEIST_EXPORT(SoccerNode);
        ZEITGEIST_EXPORT(ObjectState);
        ZEITGEIST_EXPORT(AgentState);
        ZEITGEIST_EXPORT(TrainerCommandParser);
        ZEITGEIST_EXPORT(SexpMonitor);
        ZEITGEIST_EXPORT(InternalSoccerRender);
        ZEITGEIST_EXPORT(InternalSoccerInput);
        ZEITGEIST_EXPORT(FieldFlag);
        ZEITGEIST_EXPORT(Ball);
        ZEITGEIST_EXPORT(AgentStatePerceptor);
        ZEITGEIST_EXPORT(GameStatePerceptor);
        ZEITGEIST_EXPORT(HearPerceptor);
        ZEITGEIST_EXPORT(RestrictedVisionPerceptor);
        ZEITGEIST_EXPORT(VisionPerceptor);
        ZEITGEIST_EXPORT(GameTimePerceptor);
        ZEITGEIST_EXPORT(SoccerbotBehavior);
        ZEITGEIST_EXPORT(HMDPPerceptor);
        ZEITGEIST_EXPORT(HMDPEffector);
        ZEITGEIST_EXPORT(Line);
        ZEITGEIST_EXPORT(AgentCollisionHandler);
ZEITGEIST_EXPORT_END()

