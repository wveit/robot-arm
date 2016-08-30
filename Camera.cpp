/*
*  Name: Wilbert Veit
*  CIN: 305175278
*  Project: CS450 HW 3 - Robot Arm Game
*/



#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>
#include "camera.h"
#include <iostream>
using namespace std;

const float Pi = 3.14159265f;

Camera::Camera()
{
	eye = Vec3(0, 0, 0);
	u = Vec3(1, 0, 0);
	v = Vec3(0, 1, 0);
	n = Vec3(0, 0, 1);
}

void Camera :: setModelViewMatrix()
{
	GLfloat m[16];
	m[0] =  u.x; m[4] =  u.y; m[8]  =  u.z;  m[12] = -eye.dot(u);
	m[1] =  v.x; m[5] =  v.y; m[9]  =  v.z;  m[13] = -eye.dot(v);
	m[2] =  n.x; m[6] =  n.y; m[10] =  n.z;  m[14] = -eye.dot(n);
	m[3] =  0;   m[7] =  0;   m[11] =  0;    m[15] = 1.0;

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(m);
}
void Camera::set(Vec3 Eye, Vec3 look, Vec3 up)
{
	eye = Eye;
    n = eye - look;
	u = up.cross(n);
	n.normalize(); 
	u.normalize();
	v = n.cross(u);
}

void Camera::slide(float delU, float delV, float delN )
{
	eye = eye + u * delU + v * delV + n * delN;
}

void Camera::roll(float angle)
{
	float cs = cos(Pi/180 * angle);
	float sn = sin(Pi/180 * angle);
	Vec3 t = u;
	u = Vec3(cs*t.x + sn*v.x, cs*t.y + sn*v.y, cs*t.z + sn*v.z);
	v = Vec3(-sn*t.x + cs*v.x, -sn*t.y + cs*v.y, -sn*t.z + cs*v.z);
}

void Camera::yaw(float angle)
{
	float cs = cos(Pi / 180 * angle);
	float sn = sin(Pi / 180 * angle);
	Vec3 t = u;
	u = Vec3(cs*t.x + sn*n.x, cs*t.y + sn*n.y, cs*t.z + sn*n.z);
	n = Vec3(-sn*t.x + cs*n.x, -sn*t.y + cs*n.y, -sn*t.z + cs*n.z);
}

void Camera::pitch(float angle)
{
	float cs = cos(Pi / 180 * angle);
	float sn = sin(Pi / 180 * angle);
	Vec3 t = v;
	v = Vec3(cs*t.x + sn*n.x, cs*t.y + sn*n.y, cs*t.z + sn*n.z);
	n = Vec3(-sn*t.x + cs*n.x, -sn*t.y + cs*n.y, -sn*t.z + cs*n.z);
}


void Camera::setShape(float viewAngle, float aspectRatio, float nearD, float farD)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(viewAngle, aspectRatio, nearD, farD);
}

Vec3 Camera::getEye() const
{
	return eye;
}
