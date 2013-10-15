#include "agentcollisionhandler.h"
#include <oxygen/agentaspect/agentaspect.h>

using namespace oxygen;

void AgentCollisionHandler::OnLink()
{   
}

void AgentCollisionHandler::HandleCollision(boost::shared_ptr<Collider> collidee, GenericContact& contact)
{
  if (!mAgentState.get())
  {
    mAgentState = FindAgentState(this);
    if (!mAgentState.get())
    {
        GetLog()->Error() <<
            "(AgentCollisionHandler) Could not find own AgentState\n";
        return;
    }
  }

  boost::shared_ptr<AgentState> other = FindAgentState(collidee.get());
  if (other.get())
  {
      boost::shared_ptr<TouchGroup> myGroup = mAgentState->GetTouchGroup();
      boost::shared_ptr<TouchGroup> otherGroup = other->GetTouchGroup();
      
      // Check if not already in same group
      if (myGroup == otherGroup)
          return;
        
      // Insert agents into group with lowest index
      // This ensures assignment uniqueness
      if (myGroup < otherGroup)
      {
          myGroup->insert(otherGroup->begin(), otherGroup->end());
          otherGroup->clear();
          other->SetTouchGroup(myGroup);
      }
      else
      {
          otherGroup->insert(myGroup->begin(), myGroup->end());
          myGroup->clear();
          mAgentState->SetTouchGroup(otherGroup);
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
    
    return boost::shared_static_cast<AgentState>(agentAspect->FindChildSupportingClass<AgentState>(true));
}
