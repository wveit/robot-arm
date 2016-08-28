/*
*  Name: Wilbert Veit
*  CIN: 305175278
*  Project: CS450 HW 3 - Robot Arm Game
*/

#include "Helper.h"
#include <cmath>
#include "Vec3.h"


const GLfloat Pi = 3.14f;



void drawCylinder(GLfloat radius, GLfloat height, bool wireframe, GLuint numPoints)
{
	glPushMatrix();
	glRotatef(180, 1, 0, 0);
	drawCircle(radius, wireframe, numPoints);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, height, 0);
	drawCircle(radius, wireframe, numPoints);
	glPopMatrix();
	drawRing(radius, height, wireframe, numPoints);
}

void drawCircle(GLfloat radius, bool wireframe, GLuint numPoints)
{
	GLfloat angleIncrement = 2 * Pi / numPoints;
	GLfloat maxAngle = 2 * Pi + angleIncrement / 2; // maxAngle is slightly larger than 2*Pi to ensure the circle is completed

	glNormal3f(0, 1, 0);

	for (GLfloat angle = 0; angle < maxAngle; angle += angleIncrement)
	{
		if (wireframe)
			glBegin(GL_LINE_STRIP);
		else
			glBegin(GL_TRIANGLES);
		glVertex3f(0, 0, 0);
		glVertex3f(cos(angle) * radius, 0, sin(angle) * radius);
		glVertex3f(cos(angle - angleIncrement) * radius, 0, sin(angle - angleIncrement) * radius);
		glEnd();
	}
}

void drawRing(GLfloat radius, GLfloat height, bool wireframe, GLuint numPoints)
{
	GLfloat angleIncrement = 2 * Pi / numPoints;
	GLfloat maxAngle = 2 * Pi + angleIncrement / 2; // maxAngle is slightly larger than 2*Pi to ensure the ring is completed

	
	if (wireframe)
		glBegin(GL_LINE_STRIP);
	else
		glBegin(GL_QUADS);
	
	//glBegin(GL_QUADS);
	for (GLfloat angle = 0; angle < maxAngle; angle += angleIncrement) 
	{
		Vec3 tl(cos(angle) * radius, 0, sin(angle) * radius), 
			tr(cos(angle) * radius, height, sin(angle) * radius), 
			br(cos(angle + angleIncrement) * radius, height, sin(angle + angleIncrement) * radius), 
			bl(cos(angle + angleIncrement) * radius, 0, sin(angle + angleIncrement) * radius);
		Vec3 normal = (tl - bl).cross(br - bl);
		normal.normalize();

		glNormal3f(normal.x, normal.y, normal.z);
		glVertex3f(tl.x, tl.y, tl.z);
		glVertex3f(tr.x, tr.y, tr.z);
		glVertex3f(br.x, br.y, br.z);
		glVertex3f(bl.x, bl.y, bl.z);
	}
	glEnd();
}

