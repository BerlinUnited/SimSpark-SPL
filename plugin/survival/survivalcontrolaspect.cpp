#include "survivalcontrolaspect.h"
#include <zeitgeist/logserver/logserver.h>
#include <kerosin/fontserver/fontserver.h>
#include <kerosin/openglserver/openglserver.h>
#include <oxygen/physicsserver/body.h>

using namespace boost;
using namespace kerosin;
using namespace oxygen;
using namespace salt;
using namespace std;
using namespace zeitgeist;

shared_ptr<oxygen::Effector>
SurvivalControlAspect::RequestEffectorInternal(shared_ptr<oxygen::AgentAspect>& agent,
                                               const string& effectorName)
{
        if (
                        (RegisterAgent(agent)) &&
                        (effectorName.compare("oxygen/ForceEffector") == 0)
                )
        {
                return CreateEffector(effectorName);
        }

        // no valid agentaspect/effector combination
        return shared_ptr<oxygen::Effector>();
}

boost::shared_ptr<Perceptor> SurvivalControlAspect::RequestPerceptorInternal(boost::shared_ptr<oxygen::AgentAspect>& agent, const std::string& perceptorName)
{
        if (
                        (RegisterAgent(agent)) &&
                        (agent->GetClass()->Supports("SurvivalAgentAspect"))
                )
        {
                if (perceptorName.compare("LineSegmentPerceptor") == 0)
                {
                        return CreatePerceptor(perceptorName);
                }

                if (perceptorName.compare("oxygen/VisionPerceptor") == 0)
                {
                        return CreatePerceptor(perceptorName);
                }
        }

        // no valid agentaspect/effector combination
        return shared_ptr<Perceptor>();
}

bool
SurvivalControlAspect::RegisterAgent(shared_ptr<oxygen::AgentAspect>& agent)
{
        for (ControlledAgentList::iterator i = mControlledAgents.begin();i != mControlledAgents.end(); ++i)
        {
                if ((*i).agent == agent)
                {
                        // we already know this agent
                        return true;
                }
        }

        // it's a new agent, make sure, we know how to treat an agent class
        if (! agent->GetClass()->Supports("SurvivalAgentAspect"))
        {
                return false;
        }

        // it's new and valid, remember it
        AgentControlState state;

        state.agent                     = shared_ptr<SurvivalAgentAspect>(dynamic_cast<SurvivalAgentAspect*>(agent.get()));
        state.healthInit        = 100;
        state.health            = state.healthInit;
        state.death                     = 0;
        state.hasFood           = false;

        mControlledAgents.push_back(state);

        return true;
}

/*bool SurvivalControlAspect::ConstructInternal()
{
        GetLog()->Normal() << "Survival Control Aspect started..." << endl;

        return true;
}*/

void SurvivalControlAspect::PrePhysicsUpdateInternal(float deltaTime)
{
        shared_ptr<BaseNode> food       = shared_static_cast<BaseNode>(GetCore()->Get("/usr/scene/food"));
        Vector3f foodPos;

        if (food.get() != NULL)
        {
                foodPos = food->GetWorldTransform().Pos();
        }

        int hasFoodCount = 0;

        // update all registered agents
        for (ControlledAgentList::iterator i = mControlledAgents.begin();i != mControlledAgents.end(); ++i)
        {
                AgentControlState& state = (*i);
                state.hasFood = false;

                // decrease the health of the agent
                state.health -= (mSickRate * deltaTime);
                if (state.health < 0.0f)
                {
                        shared_ptr<BaseNode> parent = shared_static_cast<BaseNode>(make_shared(state.agent->GetParent()));
                        shared_ptr<Body> body           = shared_static_cast<Body>(parent->GetChildSupportingClass("Body"));

                        if (body.get() != NULL)
                        {
                                Matrix mat = parent->GetWorldTransform();

                                // the shock of it's rebirth catapults the agents :)
                                float randX = ((rand() - RAND_MAX/2) / (float)(RAND_MAX/2)) * 200;
                                float randZ = ((rand() - RAND_MAX/2) / (float)(RAND_MAX/2)) * 200;
                                dBodyAddForce(body->GetODEBody(), randX, 500, randZ);
                        }

                        state.health = state.healthInit;
                        state.death++;
                }

                // check if it is close enough to the food
                if (food.get() != NULL)
                {
                        Vector3f dist = state.agent->GetWorldTransform().Pos() - foodPos;
                        if (dist.Length() < mFoodMinDist)
                        {
                                // the agent touched the food
                                state.hasFood = true;
                                hasFoodCount++;
                        }
                }
        }

        if (hasFoodCount > 0)
        {
                // if the food was eaten, we reposition it
                Vector3f newPos;
                newPos.x()      = 20.0f*(1.0f-2.0f*rand()/(float)RAND_MAX);
                newPos.y()      = 1.0f;
                newPos.z()      = 20.0f*(1.0f-2.0f*rand()/(float)RAND_MAX);

                Matrix mat      = food->GetWorldTransform();
                mat.Pos()       = newPos;
                food->SetLocalTransform(mat);

                // distribute the food among the agents
                const float divFoodValue = mFoodValue / hasFoodCount;

                for (ControlledAgentList::iterator i = mControlledAgents.begin();i != mControlledAgents.end(); ++i)
                {
                        AgentControlState& state = (*i);

                        if (state.hasFood)
                        {
                                state.hasFood = false;
                                state.health += divFoodValue;

                                // set the agent to not seeking as
                                // the food was repositioned
                                state.agent->SetIsSeeking(false);
                        }
                }
        }
}

void SurvivalControlAspect::OnLink()
{
        // init variables
        mSickRate               = 6.0f;
        mFoodMinDist    = 2.0f;
        mFoodValue              = 100.0f;

        shared_ptr<FontServer> fontServer = shared_static_cast<FontServer>(GetCore()->Get("/sys/server/font"));
        mFont = fontServer->GetFont("font/andalemo.ttf", 14);
}

void SurvivalControlAspect::RenderInternal()
{
        shared_ptr<FontServer> fontServer = shared_static_cast<FontServer>(GetCore()->Get("/sys/server/font"));

        if (fontServer.get() == NULL)
        {
                return;
        }

        fontServer->Begin();
        mFont->Bind();
        glColor3f(1,0,0);

        // print all registered agents
        const float lineHeight = 20;
        float y = 0;

        for (ControlledAgentList::iterator i = mControlledAgents.begin();i != mControlledAgents.end(); ++i)
        {
                const AgentControlState& state = (*i);
                shared_ptr<Leaf> parent = make_shared(state.agent->GetParent());

                mFont->Printf(0, y, "%8s: %.0f (%d)", parent->GetName().c_str(), state.health, state.death);
                y+= lineHeight;
        }

        glColor3f(1,1,1);
        fontServer->End();
}
