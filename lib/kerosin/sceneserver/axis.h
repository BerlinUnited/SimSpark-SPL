#ifndef AXIS_H__
#define AXIS_H__

#include <oxygen/sceneserver/basenode.h>

namespace kerosin
{

class Axis : public oxygen::BaseNode
{
        //
        // Functions
        //
public:
        Axis();

private:
        virtual void RenderInternal();
};

DECLARE_CLASS(Axis);

} //namespace kerosin
#endif //AXIS_H__
