/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: visionperceptor.h,v 1.1.2.5 2004/02/02 18:26:27 fruit Exp $

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

#include <oxygen/agentaspect/perceptor.h>
#include <oxygen/sceneserver/sceneserver.h>
#include <oxygen/physicsserver/raycollider.h>
#include <soccer/agentstate/agentstate.h>
#include <soccer/soccertypes.h>

class VisionPerceptor : public oxygen::Perceptor
{
public:
    VisionPerceptor();
    virtual ~VisionPerceptor();

    //! \return true, if valid data is available and false otherwise.
    bool Percept(oxygen::Predicate& predicate);

    /** Set the noise parameters of the vision perceptor.
     *
     * This will always create new calibration error values.
     * The random noise added each step is normally distributed around 0.0.
     * The (fixed) calibration error is calculated once for each axis. It
     * is uniformly distributed between -cal_error_abs and cal_error_abs and
     * added to the "camera" coordinates.
     *
     * \param sigma_dist the sigma for the distance error distribution
     * \param sigma_phi the sigma for the horizontal angle error distribution
     * \param sigma_theta the sigma for the latitudal angle error distribution
     * \param cal_error_abs absolute value of the maximum calibration error
     *                      along each axis.
     */
    void SetNoiseParams(float sigma_dist, float sigma_phi,
                        float sigma_theta, float cal_error_abs);

    //! Turn noise off/on.
    void AddNoise(bool use_it)
    { mAddNoise = use_it; }

    //! change predicate name (for debugging purposes)
    void SetPredicateName(const std::string& my_name)
    { mPredicateName = my_name; }

    //! flip horizontal coordinates according to the side of the agent
    static salt::Vector3f FlipView(const salt::Vector3f& pos, TTeamIndex ti);

protected:
    /** constructs the internal ray collider */
    virtual bool ConstructInternal();

private:
    struct ObjectData
    {
        boost::shared_ptr<Leaf> mObj;

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


    //! return a reference to the active scene from SceneServer
    bool GetActiveScene(boost::shared_ptr<oxygen::Scene>& active_scene);

    //! a reference to the scene server
    boost::shared_ptr<oxygen::SceneServer> mSceneServer;
    //! a reference to the agent state
    boost::shared_ptr<AgentState> mAgentState;
    //! vision calibration error
    salt::Vector3f mError;

    //! the predicate name (default is "Vision")
    std::string mPredicateName;
    //! sigma for random measurement error (distance)
    float mSigmaDist;
    //! sigma for random measurement error (horizontal angle)
    float mSigmaTheta;
    //! sigma for random measurement error (latitudal angle)
    float mSigmaPhi;
    //! absolute maximum value of the calibration error
    float mCalErrorAbs;
    //! flag if we should noisify the data
    bool mAddNoise;

    //! ray collider to check occlusion
    boost::shared_ptr<oxygen::RayCollider> mRay;
};

DECLARE_CLASS(VisionPerceptor);

#endif //VISIONPERCEPTOR_H
