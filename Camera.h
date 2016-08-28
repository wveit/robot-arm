/*
*  Name: Wilbert Veit
*  CIN: 305175278
*  Project: CS450 HW 3 - Robot Arm Game
*/

#ifndef CAMERA_H
#define CAMERA_H

#include "Vec3.h"

class Camera {
  private:
	Vec3 eye;
	Vec3 u,v,n;
	double viewAngle, aspect, nearDist, farDist;

  public:
	Camera();
	
	void set(Vec3 eye, Vec3 look, Vec3 up);	
    void slide(float delU, float delV, float delN);
	void roll(float angle);
	void pitch(float angle);
	void yaw(float angle);
	
	void setModelViewMatrix();
	void setShape(float viewAngle, float aspectRatio, float nearD, float farD);

	Vec3 getEye() const;
};

#endif
