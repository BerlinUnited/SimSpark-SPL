#include "sonarperceptor.h"
#include <soccerbase/soccerbase.h>
#include <objectstate/objectstate.h>

using namespace zeitgeist;
using namespace oxygen;
using namespace boost;
using namespace salt;

SonarPerceptor::SonarPerceptor() : Perceptor(),
    mMaxEchoes(10),
    mMinDistance(0.25),
    mMaxDistance(5.0),
    mSenseCone(60),
    mAddNoise(false),
    mActiveSceneLastModifiedNum(0)
{
    SetPredicateName("US"); // ultra sonic

    boost::shared_ptr<salt::NormalRNG<>> rng(new salt::NormalRNG<>(0.0, 0.005f));
    mDistRng = rng;
}

SonarPerceptor::~SonarPerceptor()
{
    mDistRng.reset();
}

void SonarPerceptor::OnLink()
{
    SoccerBase::GetActiveScene(*this, mActiveScene);
    SoccerBase::GetTransformParent(*this, mTransformParent);
}

void SonarPerceptor::OnUnlink()
{
    mTransformParent.reset();
    mActiveScene.reset();
    mDistRng.reset();
}

void SonarPerceptor::SetMaxEchoes(unsigned int max)
{
    mMaxEchoes = max;
}

void SonarPerceptor::SetRange(float min, float max)
{
    mMinDistance = min;
    mMaxDistance = max;
}

void SonarPerceptor::SetSenseCone(unsigned int angle)
{
    mSenseCone = angle;
}

void SonarPerceptor::AddNoise(bool add_noise)
{
    mAddNoise = add_noise;
}

bool SonarPerceptor::Percept(boost::shared_ptr<oxygen::PredicateList> predList)
{
    Predicate& predicate = predList->AddPredicate();
    predicate.name = mPredicateName;
    predicate.parameter.Clear();

    // add the name of the perceptor to distinguish multiple instances
    predicate.parameter.AddValue(GetName());

    TObjectDistList distances;
    RetrieveEchoes(distances);

    // order the detected distances in order to select the closest ones
    std::sort(distances.begin(), distances.end());

    ParameterList& element = predicate.parameter.AddList();
    for (unsigned int i = 0; i < mMaxEchoes; ++i) {
        if (distances.size() > i) {
            element.AddValue(distances[i]);
        } else {
            element.AddValue(mMaxDistance);
        }
    }

    return true;
}

const zeitgeist::Leaf::TLeafList& SonarPerceptor::GetObjects()
{
    if (mActiveSceneLastModifiedNum != mActiveScene->GetModifiedNum())
    {
        // remove old objects
        mObjectList.clear();
        // retrieve all new objects; this is slow!
        mActiveScene->ListChildrenSupportingClass<ObjectState>(mObjectList, true);
        // filter for Agents and Goalposts
        for (TLeafList::iterator i = mObjectList.begin(); i != mObjectList.end();)
        {
            // only use agents and goalposts
            if (!i->get()->GetClass()->SupportsClass("AgentState") &&
                !i->get()->GetClass()->SupportsClass("GoalPost")) {
                i = mObjectList.erase(i);
                continue;
            }

            ++i;
        }

        mActiveSceneLastModifiedNum = mActiveScene->GetModifiedNum();
    }

    return mObjectList;
}

void SonarPerceptor::RetrieveEchoes(TObjectDistList& distances)
{
    const TLeafList &objectList = GetObjects();
    // get the transformation matrix describing the current orientation
    const Matrix& mat = mTransformParent->GetWorldTransform();
    // angle is half the cone (left/right, top/bottom)
    const int angle = mSenseCone >> 1;

    for (TLeafList::const_iterator i = objectList.begin(); i != objectList.end(); ++i)
    {
        // skip invalid nodes
        if (i->get() == 0) { continue; }
        // get the parent transform node in order to retrieve positions
        boost::shared_ptr<ObjectState> obj = static_pointer_cast<ObjectState>(*i);
        boost::shared_ptr<Transform> j = obj->GetTransformParent();
        if (j.get() == 0) { continue; } // this should never happen

        salt::Vector3f relPos = j->GetWorldTransform().Pos() - mat.Pos();
        float dist = relPos.Length();

        if (mAddNoise)
        {
            dist += (*(mDistRng.get()))();
        }


        if (dist > mMaxDistance)
        {
            continue;
        }

        // determine position relative to the local reference frame
        Vector3f localRelPos = mat.InverseRotate(relPos);

        // theta is the angle in horizontal plane, with fwAngle as 0 degree
        float theta = gNormalizeDeg (
                        gRadToDeg(
                            gNormalizeRad(
                                gArcTan2(localRelPos[1],localRelPos[0])
                            )
                        ) - 90
                    );

        // skip objects outside the "view" cone
        if (gAbs(theta) > angle)
        {
            continue;
        }

        // the sonar sensor has a min. distance, everything below can't be quantified
        if (dist < mMinDistance) {
            distances.push_back(mMinDistance);
        } else {
            distances.push_back(dist);
        }
    }
}
