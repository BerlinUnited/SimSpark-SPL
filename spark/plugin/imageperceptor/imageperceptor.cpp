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

#include "imageperceptor.h"
#include <zeitgeist/logserver/logserver.h>

using namespace oxygen;
using namespace boost;
using namespace zeitgeist;
using namespace salt;
using namespace std;

ImagePerceptor::ImagePerceptor() : oxygen::Perceptor()
{
}

ImagePerceptor::~ImagePerceptor()
{
}

void ImagePerceptor::OnLink()
{
  mRender = shared_dynamic_cast<ImageRender > (GetCore()->New("ImageRender"));

  mCamera = shared_dynamic_cast<Camera > (GetCore()->New("oxygen/Camera"));
  if (0 != mCamera.get())
  {
    AddChildReference(mCamera);
  } else
  {
    GetLog()->Error()
      << "(ImagePerceptor) ERROR: can not create camera\n";
  }

  mRender->SetCamera(mCamera);

  RegisterCachedPath(mRenderControl, "/sys/server/simulation/RenderControl");

  if (mRenderControl.expired())
  {
    GetLog()->Error()
      << "(ImagePerceptor) ERROR: RenderControl not found\n";
  } else
  {
    mRenderControl->AddChildReference(mRender);
  }
}

bool ImagePerceptor::Percept(boost::shared_ptr<PredicateList> predList)
{
  mRender->RequestRender();

  int size = mRender->GetDataSize();
  if (size == 0)
    return false;

  Predicate &predicate = predList->AddPredicate();
  predicate.name = "IMG";
  predicate.parameter.Clear();

  ParameterList &sizeElement = predicate.parameter.AddList();
  sizeElement.AddValue(std::string("s"));
  sizeElement.AddValue(mRender->GetWidth());
  sizeElement.AddValue(mRender->GetHeight());

  ParameterList &dataElement = predicate.parameter.AddList();
  dataElement.AddValue(std::string("d"));
  const char* data = mRender->GetData();
  string datacode = mB64Encoder.encode(data, size);
  dataElement.AddValue(datacode);

  return true;
}

void ImagePerceptor::SetViewport(unsigned int x, unsigned int y, unsigned int w, unsigned int h)
{
  mCamera->SetViewport(x, y, w, h);
}

void ImagePerceptor::SetFOV(float fov)
{
  mCamera->SetFOV(fov);
}

void ImagePerceptor::SetZNear(float zNear)
{
  mCamera->SetZNear(zNear);
}

void ImagePerceptor::SetZFar(float zFar)
{
    mCamera->SetZFar(zFar);
}
