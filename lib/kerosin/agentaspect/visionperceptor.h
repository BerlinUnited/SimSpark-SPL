#ifndef VISIONPERCEPTOR_H__
#define VISIONPERCEPTOR_H__

/*!
        The VisionPerceptor class is used to filter visible visual aspects out
        of the scene graph.
*/

#include <oxygen/agentaspect/perceptor.h>

namespace kerosin
{

class VisionPerceptor : public oxygen::Perceptor
{
        //
        // functions
        //
public:
        /*
                Returns:
                        "visibleObjects" as a TLeafList
        */
        virtual bool Percept(TDictionary &dictionary);
        /*
                Setup view direction
        */
        void SetViewDirection(const salt::Vector3f& dir)
        {
                mDir = dir;
        }
private:
        salt::Vector3f  mDir;
};

DECLARE_CLASS(VisionPerceptor);

}
#endif //VISIONPERCEPTOR_H__
