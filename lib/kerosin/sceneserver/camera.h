#ifndef CAMERA_H__
#define CAMERA_H__

#include <oxygen/sceneserver/basenode.h>
#include <salt/frustum.h>

namespace kerosin
{

class Camera : public oxygen::BaseNode
{
        //
        // Functions
        //
public:
        Camera();
        virtual ~Camera();

        // set properties
        f_inline void   SetViewport(int x, int y, int width, int height)        {       mX = x; mY = y; mWidth = width; mHeight = height;       }
        f_inline void   SetFOV(const float fov)         {       mFOV = fov;             }
        f_inline void   SetZNear(const float zNear)     {       mZNear = zNear; }
        f_inline void   SetZFar(const float zFar)       {       mZFar = zFar;   }

        // adjust properties (delta increments)
        f_inline void   AdjustFOV(const float fov)              {       mFOV+=fov;              }
        f_inline void   AdjustZNear(const float zNear)  {       mZNear+=zNear;  }
        f_inline void   AdjustZFar(const float zFar)    {       mZFar+=zFar;    }

        f_inline float                                  GetFOV()const                           {       return mFOV;    }
        f_inline float                                  GetZNear()const                         {       return mZNear;  }
        f_inline float                                  GetZFar()const                          {       return mZFar;   }
        f_inline const salt::Matrix&    GetViewTransform() const        {       return mViewTransform;  }

        //! this fills in a frustum object with the correct parameters for this camera
        void DescribeFrustum(salt::Frustum& frustum) const;

        //! sets the OpenGL view parameters
        void Bind();
protected:
        //! gets the right viewport resolution
        virtual void OnLink();
private:
        //! calculates the view matrix (world->view space transformation)
        virtual void UpdateHierarchyInternal();


        //
        // Members
        //
protected:
        //! horizontal field of view
        float           mFOV;           // default = 60 degrees
        //! near clipping plane
        float           mZNear;         // default = 1
        //! far clipping plane
        float           mZFar;          // default = 2000
        //! x-position of upper left viewport corner
        int                     mX;                     // default = 0
        //! y-position of upper left viewport corner
        int                     mY;                     // default = 0
        //! width of viewport
        int                     mWidth;         // default = engine window width
        //! height of viewport
        int                     mHeight;        // default = engine window height

        float           mHalfWorldWidth;
        float           mHalfWorldHeight;

        salt::Matrix    mViewTransform;
        salt::Matrix    mProjectionTransform;
};

DECLARE_CLASS(Camera);

} //namespace kerosin

#endif //CAMERA_H__
