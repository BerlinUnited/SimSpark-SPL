/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: visionperceptor.h,v 1.1.2.3 2004/02/01 22:17:55 fruit Exp $

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
#ifndef VISIONPERCEPTOR_H
#define VISIONPERCEPTOR_H

#include <memory>
#include <salt/random.h>
#include <oxygen/agentaspect/perceptor.h>
#include <oxygen/sceneserver/sceneserver.h>
#include <oxygen/physicsserver/raycollider.h>

class VisionPerceptor : public oxygen::Perceptor
{
public:
    VisionPerceptor();
    virtual ~VisionPerceptor();

    //! \return true, if valid data is available and false otherwise.
    bool Percept(oxygen::Predicate& predicate);

    /** setup the member variables */
    virtual void OnLink();

private:
    struct ObjectData
    {
        TLeafList::iterator mObj;
        float mDist; float mTheta, mPhi;
        salt::Vector3f mRelPos;
        bool mVisible;

        ObjectData& operator=(const ObjectData& rhs)
        { mObj = rhs.mObj; mVisible = rhs.mVisible;
          mRelPos = rhs.mRelPos;
          mDist = rhs.mDist; mTheta = rhs.mTheta; mPhi = rhs.mPhi; }

        int operator==(const ObjectData& rhs) const
        { return mDist == rhs.mDist; }
        int operator<(const ObjectData& rhs) const
        { return mDist < rhs.mDist; }
    };

    void CheckOcclusion(const salt::Vector3f& my_pos,
                        std::list<ObjectData>& visible_objects) const;

    //! a reference to the scene server
    boost::shared_ptr<oxygen::SceneServer> mSceneServer;
    //! vision calibration error
    salt::Vector3f mError;
    //! random measurement error generator (x-coordinate)
    std::auto_ptr<salt::NormalRNG<> > mDistErrorRNG;
    //! random measurement error generator (y-coordinate)
    std::auto_ptr<salt::NormalRNG<> > mThetaErrorRNG;
    //! random measurement error generator (z-coordinate)
    std::auto_ptr<salt::NormalRNG<> > mPhiErrorRNG;
    //! flag if we should noisify the data
    bool mAddNoise;
    //! ray collider to check occlusion
    boost::shared_ptr<oxygen::RayCollider> mRay;
};

DECLARE_CLASS(VisionPerceptor);

#endif //VISIONPERCEPTOR_H
