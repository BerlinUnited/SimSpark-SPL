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
#ifndef VISIONPERCEPTOR_H
#define VISIONPERCEPTOR_H

#include <salt/random.h>
#include <oxygen/agentaspect/perceptor.h>
#include <oxygen/physicsserver/raycollider.h>
#include <oxygen/sceneserver/sceneserver.h>
#include <oxygen/sceneserver/transform.h>
#include <oxygen/agentaspect/agentaspect.h>
#include <agentstate/agentstate.h>

class VisionPerceptor : public oxygen::Perceptor
{
protected:
    typedef boost::shared_ptr<salt::NormalRNG<> > NormalRngPtr;

    struct ObjectData
    {
        boost::shared_ptr<ObjectState> mObj;

        float mTheta;  // angle in the X-Y (horizontal) plane
        float mPhi;    // latitude angle
        float mDist;  // distance between perceptor and object
        salt::Vector3f mRelPos;

        ObjectData& operator=(const ObjectData& rhs)
        {
            mObj     = rhs.mObj;
            mRelPos  = rhs.mRelPos;
            mTheta   = rhs.mTheta;
            mPhi     = rhs.mPhi;
            mDist    = rhs.mDist;

            return *this;
        }

        int operator==(const ObjectData& rhs) const
        { return mDist == rhs.mDist; }
        int operator<(const ObjectData& rhs) const
        { return mDist < rhs.mDist; }
    };

    typedef std::list<ObjectData> TObjectList;

public:
    VisionPerceptor();
    virtual ~VisionPerceptor();

    //! \return true, if valid data is available and false otherwise.
    bool Percept(boost::shared_ptr<oxygen::PredicateList> predList);

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

    //! Turn sensing of agent position on/off
    void SetSenseMyPos(bool sense);

    /** Turn noise off/on.
        \param add_noise flag if noise should be used at all.
    */
    void AddNoise(bool add_noise);

    /** Turn randomization off/on.
        \param random_noise flag if the measurement noise is randomized each step.
    */
    void UseRandomNoise(bool random_noise);

    //! Turn senses relative to the X-axis of the team off/on
    void SetStaticSenseAxis(bool static_axis);

protected:
    /** constructs the internal ray collider */
    virtual bool ConstructInternal();

    /** prepares a list of visible objects */
    void SetupVisibleObjects(TObjectList& visibleObjects);

    /** Percept implementation for a static relative axis */
    bool StaticAxisPercept(boost::shared_ptr<oxygen::PredicateList> predList);

    /** Percept implementation relative to the current orientation of
        the VisionPerceptor node
    */
    bool DynamicAxisPercept(boost::shared_ptr<oxygen::PredicateList> predList);

    /** Checks if the given object is occluded, seen from from my_pos */
    bool CheckOcclusion(const salt::Vector3f& my_pos, const ObjectData& od) const;

    /** constructs a sense entry for the given object in the given
        predicate*/
    void AddSense(oxygen::Predicate& predicate, ObjectData& od) const;

    /** applies noise to the setup ObjectData */
    void ApplyNoise(ObjectData& od) const;

    virtual void OnLink();
    virtual void OnUnlink();

protected:
    //! vision calibration error
    salt::Vector3f mError;

    //! true, if the absolute position of the agent is sensed.
    bool mSenseMyPos;

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
    //! flag if the error should be randomized each step
    bool mUseRandomNoise;

    /** flag if the senses are always relative to the X-axis of the
        team, default true
    */
    bool mStaticSenseAxis;

    //! ray collider to check occlusion
    boost::shared_ptr<oxygen::RayCollider> mRay;

    //! random number generator for distance errors
    NormalRngPtr mDistRng;
    //! random number generator for angle errors
    NormalRngPtr mThetaRng;
    //! random number generator for angle errors
    NormalRngPtr mPhiRng;

    boost::shared_ptr<oxygen::Scene> mActiveScene;
    //! a reference to the next transorm parent
    boost::shared_ptr<oxygen::Transform> mTransformParent;
    //! a reference to the scene server
    boost::shared_ptr<oxygen::SceneServer> mSceneServer;
    //! a reference to the agent aspect
    boost::shared_ptr<oxygen::AgentAspect> mAgentAspect;
    //! a reference to the agent state
    boost::shared_ptr<AgentState> mAgentState;
};

DECLARE_CLASS(VisionPerceptor);

#endif //VISIONPERCEPTOR_H
