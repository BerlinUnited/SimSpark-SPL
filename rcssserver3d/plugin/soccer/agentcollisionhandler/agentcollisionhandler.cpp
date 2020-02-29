#include "agentcollisionhandler.h"
#include <oxygen/agentaspect/agentaspect.h>

using namespace oxygen;

void AgentCollisionHandler::OnLink()
{   
}

#include <ode/ode.h>

void AgentCollisionHandler::HandleCollision(boost::shared_ptr<Collider> collidee, GenericContact &contact)
{
    if (!mAgentState.get())
    {
        mAgentState = FindAgentState(this);
        if (!mAgentState.get())
        {
            GetLog()->Error() << "(AgentCollisionHandler) Could not find own AgentState\n";
            return;
        }
    }

    boost::shared_ptr<AgentState> other = FindAgentState(collidee.get());
    if (other.get())
    {
        dContact &ODEContact = (dContact &)contact;
        //GetLog()->Error() << "(AgentCollisionHandler) contact pos " << ODEContact.geom.pos[0]<<","<< ODEContact.geom.pos[1]  << ","<< ODEContact.geom.pos[2] << "\n";
        if (mAgentState->GetTeamIndex() != other->GetTeamIndex()) {
            boost::shared_ptr<AgentState> recordCollisionAgent;
            boost::shared_ptr<AgentState> otherCollisionAgent;
            if (mAgentState->GetTeamIndex() == TI_LEFT) {
                recordCollisionAgent = mAgentState; 
                otherCollisionAgent = other;
            }
            else {
                recordCollisionAgent = other;
                otherCollisionAgent = mAgentState;
            }
            bool fAlreadyHaveCollision = false;
            salt::Vector3f newCollPos = salt::Vector3f(ODEContact.geom.pos[0], ODEContact.geom.pos[1], ODEContact.geom.pos[2]);
            OpponentCollisionInfoVec& oppCollisionPosInfoVec = recordCollisionAgent->GetOppCollisionPosInfoVec();
            for (unsigned int c = 0; c < oppCollisionPosInfoVec.size(); c++) {
                if (oppCollisionPosInfoVec[c].first == otherCollisionAgent->GetUniformNumber()) {
                    salt::Vector3f collPos = oppCollisionPosInfoVec[c].second.first;
                    int prevCollNum = oppCollisionPosInfoVec[c].second.second;
                    // Get updated average collision position
                    collPos = (collPos*prevCollNum + newCollPos)/(prevCollNum+1); 
                    oppCollisionPosInfoVec[c].second.first = collPos;
                    oppCollisionPosInfoVec[c].second.second = prevCollNum+1;  
                    fAlreadyHaveCollision = true;
                    break;
                }
            }
            if (!fAlreadyHaveCollision) {
                oppCollisionPosInfoVec.push_back(OpponentCollisionInfo(
                    otherCollisionAgent->GetUniformNumber(),
                    OpponentCollisionPositionInfo (newCollPos, 1)));
            }
        }

        boost::shared_ptr<TouchGroup> myGroup = mAgentState->GetTouchGroup();
        boost::shared_ptr<TouchGroup> otherGroup = other->GetTouchGroup();

        // Check if not already in same group
        if (myGroup == otherGroup)
            return;

        // Insert agents into group with lowest index
        // This ensures assignment uniqueness
        boost::shared_ptr<TouchGroup> updatedGroup; 
        if (myGroup < otherGroup)
        {
            myGroup->insert(otherGroup->begin(), otherGroup->end());
            otherGroup->clear();
            updatedGroup = myGroup;
        }
        else
        {
            otherGroup->insert(myGroup->begin(), myGroup->end());
            myGroup->clear();
            updatedGroup = otherGroup;
        }

        for (TouchGroup::iterator agentIt = updatedGroup->begin();
            agentIt != updatedGroup->end(); ++agentIt)
        {
            (*agentIt)->SetTouchGroup(updatedGroup);
        }
    }
}

boost::shared_ptr<AgentState> AgentCollisionHandler::FindAgentState(BaseNode* node)
{
    boost::shared_ptr<AgentAspect> agentAspect
      = node->FindParentSupportingClass<AgentAspect>().lock();
    if (!agentAspect.get())
    {
        return boost::shared_ptr<AgentState>();
    }
    
    return boost::static_pointer_cast<AgentState>(agentAspect->FindChildSupportingClass<AgentState>(true));
}
