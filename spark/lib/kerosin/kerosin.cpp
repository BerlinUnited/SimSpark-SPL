/* -*- mode: c++; c-basic-indent: 4; indent-tabs-mode: nil -*-

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
#include "kerosin.h"
#include <zeitgeist/scriptserver/scriptserver.h>

using namespace kerosin;
using namespace zeitgeist;

Kerosin::Kerosin(zeitgeist::Zeitgeist &zg)
{
    zg.GetCore()->RegisterClassObject(new CLASS(SoundServer), "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(ImageServer), "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(FontServer), "kerosin/");

    zg.GetCore()->RegisterClassObject(new CLASS(InputServer), "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(InputSystem), "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(InputItem),   "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(InputDevice), "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(InputControl), "kerosin/");

    zg.GetCore()->RegisterClassObject(new CLASS(OpenGLServer), "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(OpenGLSystem), "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(BaseRenderServer), "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(RenderServer), "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(RenderControl), "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(RenderNode), "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(TextureServer), "kerosin/");

    zg.GetCore()->RegisterClassObject(new CLASS(MaterialServer), "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(Material), "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(Material2DTexture), "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(MaterialExternal), "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(MaterialSolid), "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(MaterialExporter), "kerosin/");

    // scene graph
    zg.GetCore()->RegisterClassObject(new CLASS(SingleMatNode), "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(Axis), "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(Light), "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(StaticMesh), "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(Box), "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(Capsule), "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(Cylinder), "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(Sphere), "kerosin/");

    // load default setting
    zg.GetCore()->GetRoot()->GetScript()->RunInitScript
        (
         "kerosin.rb",
         "lib/kerosin"
         );
}
