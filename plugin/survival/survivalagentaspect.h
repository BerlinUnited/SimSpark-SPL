#ifndef SURVIVALAGENTASPECT_H__
#define SURVIVALAGENTASPECT_H__

#include <oxygen/agentaspect/agentaspect.h>
#include <kerosin/agentaspect/visionperceptor.h>
#include <oxygen/agentaspect/forceeffector.h>
#include "linesegmentperceptor.h"

class SurvivalAgentAspect : public oxygen::AgentAspect
{
        //
        // functions
        //
public:
        SurvivalAgentAspect();

        void Think(float deltaTime);

        // set parameters
        void SetWanderingStrength(float v)      {       mWanderingStrength = v; }
        void SetWanderingRate(float v)          {       mWanderingRate = v; }

        void SetIsSeeking(bool v)                       {       mIsSeeking = v; }


private:
        virtual void OnLink();

        salt::Vector3f  CalcSeekForce(const salt::Vector3f& seek);
        salt::Vector3f  CalcAvoidanceForce(const salt::Vector3f& avoid);
        salt::Vector3f  CalcWanderingForce();

        //
        // members
        //
private:
        // perceptors
        boost::shared_ptr<kerosin::VisionPerceptor>     mVisionPerceptor;
        boost::shared_ptr<LineSegmentPerceptor>         mLineSegmentPerceptor;

        // effectors
        boost::shared_ptr<oxygen::ForceEffector>       mForceEffector;

        // state of the agent
        bool                    mIsSeeking;
        salt::Vector3f  mSeekPoint;

        // wandering members

        //! radius of circle defining legal steering forces
        float           mWanderingStrength;
        //! offset of wandering strength
        float           mWanderingOffset;
        //! possible offset
        float           mWanderingRate;
        //! previous wandering force
        salt::Vector3f  mWanderingForce;
};

DECLARE_CLASS(SurvivalAgentAspect);

#endif //SURVIVALCONTROLASPECT_H__
