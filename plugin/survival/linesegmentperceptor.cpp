#include "linesegmentperceptor.h"
#include <oxygen/sceneserver/scene.h>
#include <kerosin/sceneserver/staticmesh.h>

using namespace boost;
using namespace kerosin;
using namespace salt;

LineSegmentPerceptor::LineSegmentPerceptor()
{
        mStart.Set(0,0,0);
        mEnd.Set(0,0,0);
}

void LineSegmentPerceptor::SetLineSegment(const salt::Vector3f& start, const salt::Vector3f& end)
{
        mStart  = start;
        mEnd    = end;
}

bool LineSegmentPerceptor::Percept(TDictionary &dictionary)
{
        // update control aspects
        TLeafList meshes;
        TLeafList::iterator i;

        GetScene()->GetChildrenSupportingClass("StaticMesh", meshes, true);
        shared_ptr<StaticMesh> self = shared_static_cast<StaticMesh>(make_shared(make_shared(GetParent())->GetParent())->GetChildOfClass("StaticMesh"));

        Vector3f start, end, ret;

        dictionary["hit"] = false;
        for (i = meshes.begin(); i != meshes.end(); ++i)
        {
                if (self.get() != (*i).get())
                {
                        if (shared_static_cast<StaticMesh>(*i)->Intersect(mStart, mEnd, ret))
                        {
                                dictionary["hit"] = true;
                                dictionary["p"] = ret;
                        }
                }
        }
        return true;
}
