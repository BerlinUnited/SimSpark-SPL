//#ifndef _GLSERVER_H
//#define _GLSERVER_H

#include <GL/glut.h>
#include "camera.h"

class GLserver
{
 public:
  GLserver(){};
  GLserver(int width, int height,
	   salt::Vector3f camPos, 
	   salt::Vector3f lookAtPos, 
	   salt::Vector3f up, bool wire);
  ~GLserver() {};

  void InitGL();     
  void DrawGround();
  void ApplyCamera();
  void DrawSphere(salt::Vector3f spherePos, float radius);
  
  //inline functions
  inline void SetViewByMouse(const Vector2f& mousePos, const Vector2f& refPos) 
    {mCamera.SetViewByMouse(mousePos, refPos);} 
  
protected:
  salt::Vector3f mCamPos;
  salt::Vector3f mLookAt;
  salt::Vector3f mUp;
  Camera mCamera;
  bool mWireframe;
  int mWidth, mHeight;
};
  
//#endif
  
  
