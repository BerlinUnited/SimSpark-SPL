/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Copyright (C) 2008 RoboCup Soccer Server 3D Maintenance Group
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

#ifndef IMAGEPERCEPTOR_H
#define IMAGEPERCEPTOR_H

#include <libb64/encode.h>
#include <oxygen/agentaspect/perceptor.h>
#include <oxygen/sceneserver/camera.h>
#include <kerosin/renderserver/rendercontrol.h>
#include "imagerender.h"

class ImagePerceptor : public oxygen::Perceptor
{
public:
	ImagePerceptor();
    virtual ~ImagePerceptor();

    //! \return true, if valid data is available and false otherwise.
    bool Percept(boost::shared_ptr<oxygen::PredicateList> predList);
    
    virtual void OnLink();

    void SetViewport(unsigned int x, unsigned int y, unsigned int w, unsigned int h);

    void SetFOV(float fov);

    void SetZNear(float zNear);

    void SetZFar(float zFar);
    
private:
    boost::shared_ptr<oxygen::Camera> mCamera;

    boost::shared_ptr<ImageRender> mRender;

    zeitgeist::Leaf::CachedPath<kerosin::RenderControl> mRenderControl;

    base64::Encoder mB64Encoder;
};

DECLARE_CLASS(ImagePerceptor);

#endif //IMAGEPERCEPTOR_H
