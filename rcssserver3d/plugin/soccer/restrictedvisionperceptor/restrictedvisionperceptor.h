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
#ifndef RESTRICTEDVISIONPERCEPTOR_H
#define RESTRICTEDVISIONPERCEPTOR_H

#include <salt/random.h>
#include <oxygen/agentaspect/perceptor.h>
#include <oxygen/physicsserver/raycollider.h>
#include <oxygen/sceneserver/sceneserver.h>
#include <oxygen/sceneserver/transform.h>
#include <oxygen/agentaspect/agentaspect.h>
#include <agentstate/agentstate.h>
#include "../line/line.h"
#include "../ball/ball.h"

class RestrictedVisionPerceptor : public oxygen::Perceptor
{
protected:
    typedef boost::shared_ptr<salt::NormalRNG<> > NormalRngPtr;

    struct ObjectData
    {
        boost::shared_ptr<ObjectState> mObj;

        float mTheta;  // angle in the X-Y (horizontal) plane
        float mPhi;    // latitude angle
        float mDist;  // distance between perceptor and object
        salt::Vector3f mRelPos; //position relative to perceptor
        //string name; // name of the object


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

    typedef std::map<boost::shared_ptr<oxygen::BaseNode>, TObjectList> TNodeObjectsMap;

    struct LineData
    {
      boost::shared_ptr<Line> mLine;
      ObjectData mBeginPoint;
      ObjectData mEndPoint;
    };

    typedef std::list<LineData> TLineList;

public:
    RestrictedVisionPerceptor();
    virtual ~RestrictedVisionPerceptor();

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

    //! Turn sensing of agent orientation on/off
    void SetSenseMyOrien(bool sense);

    //! Turn sensing of ball position on/off
    void SetSenseBallPos(bool sense);

    //! Turn sensing of lines on/off
    void SetSenseLine(bool sense);

    //! Turn sensing of lines' name on/off
    void SetSenseLineName(bool sense);

    /** Turn noise off/on.
        \param add_noise flag if noise should be used at all.
    */
    void AddNoise(bool add_noise);

    //! Turn senses relative to the X-axis of the team off/on
    void SetStaticSenseAxis(bool static_axis);

    //! Set the visible area of the perceptor
    void SetViewCones(unsigned int hAngle, unsigned int vAngle);

    /** Set the pan range.
     *  As long as the agents have no inherent direction and 0 degrees
     *  horizontally is always directed towards the opponent side, it
     *  does not make much sense to restrict the pan range other than
     *  to a fixed angle, which disables panning effectively.
     *  A range of 360 degrees (e.g. from -180 to 180) means no restriction.
     * @param lower lower bound (in degrees)
     * @param upper upper bound (in degrees)
     */
    void SetPanRange(int lower, int upper);
    /** Set the tilt range.
     *  Contrary to the pan range, it makes much sense to restrict the
     *  tilt range. However, a range of 360 degrees (e.g. from -180 to 180)
     *  means no restriction.
     * @param lower lower bound (in degrees)
     * @param upper upper bound (in degrees)
     */
    void SetTiltRange(int lower, int upper);

    /** Set the view angles
     * @param pan the horizontal view angle in degrees
     *        (0 degrees is the x-axis towards the opponent side)
     * @param tilt is the vertical view angle in degrees
     *        (0 degrees is the angle parallel to the floor)
     */
    void SetPanTilt(float pan, float tilt);
    /** Add values to the current view angles.
     * @param pan the horizontal view angle in degrees
     *        (0 degrees is the x-axis towards the opponent side)
     * @param tilt is the vertical view angle in degrees
     *        (0 degrees is the angle parallel to the floor)
     */
    void ChangePanTilt(float pan, float tilt);
    /** Get the horizontal view direction.
     * @return the pan angle in degrees
     */
    float GetPan() const;
    /** Get the horizontal view direction.
     * @return the tilt angle in degrees
     */
    float GetTilt() const;

protected:
    /** constructs the internal ray collider */
    virtual bool ConstructInternal();

    /** prepares a list of visible nodes */
    void SetupVisibleNodes(TNodeObjectsMap& visibleNodes);

    /** Percept implementation for a static relative axis */
    bool StaticAxisPercept(boost::shared_ptr<oxygen::PredicateList> predList);

    /** Percept implementation relative to the current orientation of
        the RestrictedVisionPerceptor node
    */
    bool DynamicAxisPercept(boost::shared_ptr<oxygen::PredicateList> predList);

    /** Percept lines in the world */
    void SenseLine(oxygen::Predicate& predicate);

    void SetupLines(TLineList& visibleLines);

    bool CheckVisuable(ObjectData& od) const;

    /** Checks if the given object is occluded, seen from from my_pos */
    bool CheckOcclusion(const salt::Vector3f& my_pos, const ObjectData& od) const;

    /** constructs a sense entry for the given node with objects
        in the given predicate
    */
    void AddSense(oxygen::Predicate& predicate,
                  boost::shared_ptr<BaseNode> node,
                  TObjectList& objectList) const;

    void AddSense(oxygen::Predicate& predicate,
                  const TLineList& lineList) const;

    /** applies noise to the setup ObjectData */
    void ApplyNoise(ObjectData& od) const;

    virtual void OnLink();
    virtual void OnUnlink();

protected:
    //! vision calibration error
    salt::Vector3f mError;

    //! true, if the absolute position of the agent is sensed.
    bool mSenseMyPos;

    //! true, if the absolute orientation of the agent's camera in the x-y plane is sensed.
    bool mSenseMyOrien;
    
    //! true, if the absolute position of the ball is sensed.
    bool mSenseBallPos;

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

    /** flag if the senses are always relative to the X-axis of the
        team, default true
    */
    bool mStaticSenseAxis;

    /** flag if the lines can be sensed */
    bool mSenseLine;
    bool mSenseLineName;

    //! horizontal opening of the vision cone
    unsigned int mHViewCone;
    //! vertical opening of the vision cone
    unsigned int mVViewCone;
    //! horizontal view direction in degrees
    float mPan;
    //! the lower bound for the horizontal view direction in degrees
    int mPanLower;
    //! the upper bound for the horizontal view direction in degrees
    int mPanUpper;
    //! vertical view direction in degrees
    float mTilt;
    //! the lower bound for the vertical view direction in degrees
    int mTiltLower;
    //! the upper bound for the vertical view direction in degrees
    int mTiltUpper;

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

DECLARE_CLASS(RestrictedVisionPerceptor);

#endif //VISIONPERCEPTOR_H
