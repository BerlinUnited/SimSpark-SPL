#ifndef LINESEGMENTPERCEPTOR_H__
#define LINESEGMENTPERCEPTOR_H__

#include <oxygen/agentaspect/perceptor.h>

class LineSegmentPerceptor : public oxygen::Perceptor
{
public:
        LineSegmentPerceptor();

        void SetLineSegment(const salt::Vector3f& start, const salt::Vector3f& end);

        /*!
                Performs a line segment intersection test with all static meshes of the
                scene.
        */
        virtual bool Percept(TDictionary &dictionary);
private:
        //! starting point of line segment
        salt::Vector3f  mStart;
        //! end point of line segment
        salt::Vector3f  mEnd;
};

DECLARE_CLASS(LineSegmentPerceptor);

#endif // LINESEGMENTPERCEPTOR_H__
