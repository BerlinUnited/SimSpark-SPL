#ifndef _CAMERA_H
#define _CAMERA_H

#include <math.h>
#include <GL/glut.h>
#include <zeitgeist/zeitgeist.h>
#include <oxygen/oxygen.h>

using namespace boost;
using namespace std;
using namespace zeitgeist;
using namespace salt;

#define PI 3.141592

class Camera
{
 public:
  Camera(){};
  Camera(const Vector3f& camPos,
	 const Vector3f& lookAtPos,
	 const Vector3f& upVector);
  
  ~Camera() {};

  void Look();
  void SetCameraPos(const Vector3f& newCamPos);
  void SetLookAtPos(const Vector3f& lookAtPos);
  void SetUpVector(const Vector3f& upVector);
  void SetViewByMouse(const Vector2f& mousePos,
		      const Vector2f& refPos);

  Vector3f GetCameraPos();

 protected:
  double mMouseSensitivity;
  double mPhi, mTheta, mFixDist;
  Vector3f mPosition;
  Vector3f mLookAtPos;
  Vector3f mUpVector;
};
#endif
