#include "survivalcontrolaspect.h"
#include "survivalagentaspect.h"
#include <zeitgeist/logserver/logserver.h>
#include <kerosin/fontserver/fontserver.h>
#include <kerosin/openglserver/openglserver.h>

using namespace boost;
using namespace kerosin;
using namespace salt;
using namespace std;
using namespace zeitgeist;

shared_ptr<Effector> SurvivalControlAspect::RequestEffectorInternal(shared_ptr<AgentAspect>& agent, const string& effectorName)
{
	// always make sure, we know how to treat an agent class
	if (agent->GetClass()->Supports("SurvivalAgentAspect"))
	{
		if (effectorName.compare("kerosin/ForceEffector") == 0)
			return CreateEffector(effectorName);
	}

	// no valid agentaspect/effector combination
	return shared_ptr<Effector>();
}

boost::shared_ptr<Perceptor> SurvivalControlAspect::RequestPerceptorInternal(boost::shared_ptr<AgentAspect>& agent, const std::string& perceptorName)
{
	// always make sure, we know how to treat an agent class
	if (agent->GetClass()->Supports("SurvivalAgentAspect"))
	{
		if (perceptorName.compare("LineSegmentPerceptor") == 0)
			return CreatePerceptor(perceptorName);
		if (perceptorName.compare("kerosin/VisionPerceptor") == 0)
			return CreatePerceptor(perceptorName);
	}

	// no valid agentaspect/effector combination
	return shared_ptr<Perceptor>();
}

/*bool SurvivalControlAspect::ConstructInternal()
{
	GetLog()->Normal() << "Survival Control Aspect started..." << endl;
	
	return true;
}*/

void SurvivalControlAspect::PrePhysicsUpdateInternal(float deltaTime)
{
	mHealthSlow -= mSickRate*deltaTime;
	mHealthFast -= mSickRate*deltaTime;

	if (mHealthSlow < 0.0f)
	{
		mHealthSlow = 100.0f;
		mDeathSlow++;
	}

	if (mHealthFast < 0.0f)
	{
		mHealthFast = 100.0f;
		mDeathFast++;
	}

	// check if an agent is close enough to eat the food
	shared_ptr<BaseNode> slow = shared_static_cast<BaseNode>(GetCore()->Get("/usr/scene/slow"));
	shared_ptr<BaseNode> fast = shared_static_cast<BaseNode>(GetCore()->Get("/usr/scene/fast"));
	shared_ptr<BaseNode> food = shared_static_cast<BaseNode>(GetCore()->Get("/usr/scene/food"));

	Vector3f	dist;
	bool		slowHasIt = false;
	bool		fastHasIt = false;

	dist = slow->GetWorldTransform().Pos() - food->GetWorldTransform().Pos();
	if (dist.Length() < 2.0f)
	{
		slowHasIt = true;
	}

	dist = fast->GetWorldTransform().Pos() - food->GetWorldTransform().Pos();
	if (dist.Length() < 2.0f)
	{
		fastHasIt = true;
	}

	if (slowHasIt && fastHasIt)
	{
		// only give both half the health
		mHealthSlow += 50.0f;
		mHealthFast += 50.0f;
	}
	else
	{
		if (slowHasIt) mHealthSlow = 100.0f;
		if (fastHasIt) mHealthFast = 100.0f;
	}

	if (mHealthSlow > 100.0f) mHealthSlow = 100.0f;
	if (mHealthFast > 100.0f) mHealthFast = 100.0f;

	// if the food was eaten, we reposition it
	if (slowHasIt || fastHasIt)
	{
		Vector3f newPos;
		newPos.x() = 20.0f*(1.0f-2.0f*rand()/(float)RAND_MAX);
		newPos.y() = 1.0f;
		newPos.z() = 20.0f*(1.0f-2.0f*rand()/(float)RAND_MAX);
		Matrix mat = food->GetWorldTransform();
		mat.Pos() = newPos;
		food->SetLocalTransform(mat);

		// also set both agents to not seeking
		shared_static_cast<SurvivalAgentAspect>(GetCore()->Get("/usr/scene/slow/_agent"))->SetIsSeeking(false);
		shared_static_cast<SurvivalAgentAspect>(GetCore()->Get("/usr/scene/fast/_agent"))->SetIsSeeking(false);
	}
}

void SurvivalControlAspect::OnLink()
{
	// init variables
	mDeathSlow = 0;
	mDeathFast = 0;

	mHealthSlow = 100;
	mHealthFast = 100;

	mSickRate = 6;

	shared_ptr<FontServer> fontServer = shared_static_cast<FontServer>(GetCore()->Get("/sys/server/font"));
	mFont = fontServer->GetFont("font/andalemo.ttf", 14);
}

void SurvivalControlAspect::RenderInternal()
{
	shared_ptr<FontServer> fontServer = shared_static_cast<FontServer>(GetCore()->Get("/sys/server/font"));

	if (fontServer.get() != NULL)
	{
		fontServer->Begin();
		mFont->Bind();
		glColor3f(1,0,0);
		mFont->Printf(0, 0, "Slow: %03.0f (%d)", mHealthSlow, mDeathSlow);
		mFont->Printf(0, 20, "Fast: %03.0f (%d)", mHealthFast, mDeathFast);
		glColor3f(1,1,1);
		fontServer->End();
	}
}