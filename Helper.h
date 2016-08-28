/*
*  Name: Wilbert Veit
*  CIN: 305175278
*  Project: CS450 HW 3 - Robot Arm Game
*/

#pragma once

#include <Windows.h>
#include <gl/GL.h>


void drawCylinder(GLfloat radius, GLfloat height, bool wireframe = false, GLuint numPoints = 20);
void drawCircle(GLfloat radius, bool wireframe = false, GLuint numPoints = 20);
void drawRing(GLfloat radius, GLfloat height, bool wireframe = false, GLuint numPoints = 20);


