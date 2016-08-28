/*
*  Name: Wilbert Veit
*  CIN: 305175278
*  Project: CS450 HW 3 - Robot Arm Game
*/

#include "Materials.h"

#include <cstdlib>
#include <gl/glut.h>

GLfloat silverAmbient[] = { 0.19225f, 0.19225f, 0.19225f, 1.f };
GLfloat silverDiffuse[] = { 0.5074f, 0.5074f, 0.5074f, 1.f };
GLfloat silverSpecular[] = { 0.508273f, 0.508273f, 0.508273f, 1.f };
GLfloat silverShininess[] = { 0.4f * 128 };

GLfloat goldAmbient[] = { 0.24725f, 0.1995f, 0.0745f, 1.f };
GLfloat goldDiffuse[] = { 0.75164f, 0.60648f, 0.22648f, 1.f };
GLfloat goldSpecular[] = { 0.628281f, 0.555802f, 0.366065f, 1.f };
GLfloat goldShininess[] = { 0.4f * 128 };

GLfloat greenRubberAmbient[] = { 0.f, 0.05f, 0.f, 1.f };
GLfloat greenRubberDiffuse[] = { 0.4f, 0.5f, 0.4f, 1.f };
GLfloat greenRubberSpecular[] = { 0.04f, 0.3f, 0.04f, 1.f };
GLfloat greenRubberShininess[] = { 0.4f * 128 };

GLfloat redAmbient[] = { 0.3f, 0.f, 0.f, 1.f };
GLfloat redDiffuse[] = { 0.5f, 0.1f, 0.1f, 1.f };
GLfloat redSpecular[] = { 0.3f, 0.02f, 0.04f, 1.f };
GLfloat redShininess[] = { 0.4f * 128 };

GLfloat blueAmbient[] = { 0.f, 0.f, 0.1f, 1.f };
GLfloat blueDiffuse[] = { 0.02f, 0.2f, 0.8f, 1.f };
GLfloat blueSpecular[] = { 0.04f, 0.04f, 0.5f, 1.f };
GLfloat blueShininess[] = { 0.4f * 128 };

GLfloat whiteAmbient[] = { 1.f, 1.f, 1.f, 1.f };
GLfloat whiteDiffuse[] = { 1.f, 1.f, 1.f, 1.f };
GLfloat whiteSpecular[] = { 1.f, 1.f, 1.f, 1.f };
GLfloat whiteShininess[] = { 0.4f * 128 };

GLfloat blackAmbient[] = { 0.f, 0.f, 0.f, 1.f };
GLfloat blackDiffuse[] = { 0.f, 0.f, 0.f, 1.f };
GLfloat blackSpecular[] = { 0.1f, 0.1f, 0.1f, 1.f };
GLfloat blackShininess[] = { 0.4f * 128 };

void setGoldMaterial()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, goldAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, goldDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, goldSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, goldShininess);
}

void setSilverMaterial()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, silverAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, silverDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, silverSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, silverShininess);
}


void setGreenRubberMaterial()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, greenRubberAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, greenRubberDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, greenRubberSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, greenRubberShininess);
}

void setRedMaterial()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, redAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, redDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, redSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, redShininess);
}

void setBlueMaterial()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, blueAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blueDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, blueSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, blueShininess);
}

void setWhiteMaterial()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, whiteAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, whiteShininess);
}

void setBlackMaterial()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, blackAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blackDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, blackSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, blackShininess);
}