/*
*  Name: Wilbert Veit
*  CIN: 305175278
*  Project: CS450 HW 3 - Robot Arm Game
*/

#include "Renderer.h"
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>
#include <iostream>
using namespace std;
#include "Helper.h"
#include "Materials.h"

#include "Robot.h"
#include "Ball.h"
#include "Box.h"
#include "Enemy.h"
#include "Coin.h"

const GLfloat Pi = 3.14f;

Renderer::Renderer()
{
}

void Renderer::render(const Ball &ball)
{
	if (!ball.visible)
		return;

	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslatef(ball.position.x, ball.position.y, ball.position.z);
	glutWireSphere(0.3, 10, 10);
	glPopMatrix();
}

void Renderer::renderGoal(const Box &goalBox)
{
	setRedMaterial();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glVertex3f(goalBox.left, goalBox.bottom, goalBox.back);
	glVertex3f(goalBox.left, goalBox.bottom, goalBox.front);
	glVertex3f(goalBox.right, goalBox.bottom, goalBox.front);
	glVertex3f(goalBox.right, goalBox.bottom, goalBox.back);
	glEnd();
}

void Renderer::renderCoin(const Coin &coin)
{
	setGoldMaterial();
	glPushMatrix();

	glTranslatef(coin.position().x, 1, coin.position().z);
	glRotatef(coin.angle(), 0, 1, 0);
	glRotatef(90, 1, 0, 0);
	glTranslatef(0, -0.125, 0);
	drawCylinder(1, 0.25);
	glPopMatrix();
}

void Renderer::renderRoom(const Box &roomBox)
{
	setGreenRubberMaterial();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glVertex3f(roomBox.left, roomBox.bottom, roomBox.back);
	glVertex3f(roomBox.left, roomBox.bottom, roomBox.front);
	glVertex3f(roomBox.right, roomBox.bottom, roomBox.front);
	glVertex3f(roomBox.right, roomBox.bottom, roomBox.back);
	glEnd();

	setBlueMaterial();
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1);
	glVertex3f(roomBox.left, roomBox.bottom, roomBox.back);
	glVertex3f(roomBox.right, roomBox.bottom, roomBox.back);
	glVertex3f(roomBox.right, roomBox.top, roomBox.back);
	glVertex3f(roomBox.left, roomBox.top, roomBox.back);

	glNormal3f(0, 0, 1);
	glVertex3f(roomBox.left, roomBox.bottom, roomBox.front);
	glVertex3f(roomBox.left, roomBox.top, roomBox.front);
	glVertex3f(roomBox.right, roomBox.top, roomBox.front);
	glVertex3f(roomBox.right, roomBox.bottom, roomBox.front);

	glNormal3f(-1, 0, 0);
	glVertex3f(roomBox.left, roomBox.bottom, roomBox.back);
	glVertex3f(roomBox.left, roomBox.top, roomBox.back);
	glVertex3f(roomBox.left, roomBox.top, roomBox.front);
	glVertex3f(roomBox.left, roomBox.bottom, roomBox.front);

	glNormal3f(1, 0, 0);
	glVertex3f(roomBox.right, roomBox.bottom, roomBox.back);
	glVertex3f(roomBox.right, roomBox.bottom, roomBox.front);
	glVertex3f(roomBox.right, roomBox.top, roomBox.front);
	glVertex3f(roomBox.right, roomBox.top, roomBox.back);
	glEnd();

}

void Renderer::render(const Robot &robot, bool wireframe, bool controlsAreOn)
{
	glLineWidth(1);

	setSilverMaterial();
	// draw activation light
	glPushMatrix();
	glTranslatef(robot.robotPosition().x, robot.robotPosition().y, robot.robotPosition().z);
	glRotatef(-robot.shoulderRotation(), 0, 1, 0);
	glTranslatef(1.5, robot.baseHeight(), 0);

	if (controlsAreOn)
		glColor3f(1, 0, 0);
	else
		glColor3f(0, 0, 0);
	glutWireSphere(0.2, 10, 10);
	glPopMatrix();


	glColor3f(0, 0, 1);
	glPushMatrix(); // Transforms that apply to whole arm
	glTranslatef(robot.robotPosition().x, robot.robotPosition().y, robot.robotPosition().z);
	if (robot.isHurt())
		setRedMaterial();
	drawCylinder(3, 1, wireframe);
	setSilverMaterial();

	glTranslatef(0, robot.baseHeight(), 0);
	glRotatef(-robot.shoulderRotation() - 90, 0, 1, 0);
	glRotatef(robot.shoulderAngle() - 90, 1, 0, 0);

	// draw the shoulder
	setGoldMaterial();
	if (wireframe)
		glutWireSphere(1, 10, 10);
	else
		glutSolidSphere(1, 10, 10);
	setSilverMaterial();

	// draw the first part of upper arm
	glPushMatrix(); // Transforms that apply to one side of upper arm
	glTranslatef(-0.3, robot.upperArmLength() / 2, 0);
	glScalef(0.3, robot.upperArmLength(), 0.75);
	if (wireframe)
		glutWireCube(1);
	else
		glutSolidCube(1);
	glPopMatrix(); // back to whole arm transforms

	// draw the second part of upper arm
	glPushMatrix(); // Transforms that apply to other side of upper arm
	glTranslatef(0.3, robot.upperArmLength() / 2, 0);
	glScalef(0.3, robot.upperArmLength(), 0.75);
	if (wireframe)
		glutWireCube(1);
	else
		glutSolidCube(1);
	glPopMatrix(); // back to whole arm transforms


	glPushMatrix(); // Transforms that apply to lower arm and hand
	glTranslatef(0, robot.upperArmLength(), 0);
	glRotatef(robot.elbowAngle() + 180, 1, 0, 0);


	// draw the elbow
	setGoldMaterial();
	if (wireframe)
		glutWireSphere(0.5, 10, 10);
	else
		glutSolidSphere(0.5, 10, 10);
	setSilverMaterial();

	// draw the lower arm
	glPushMatrix(); // Transforms for just the lower arm
	glTranslatef(0, robot.lowerArmLength() / 2, 0);
	glScalef(0.3, robot.lowerArmLength(), 0.75);
	if (wireframe)
		glutWireCube(1);
	else
		glutSolidCube(1);
	glPopMatrix(); // back to lower arm and hand

	glPushMatrix(); // Transforms that apply to hand
	glTranslatef(0, robot.lowerArmLength(), 0);
	glRotatef(robot.wristAngle() + 180, 1, 0, 0);

	// draw wrist
	setGoldMaterial();
	if (wireframe)
		glutWireSphere(0.5, 10, 10);
	else
		glutSolidSphere(0.5, 10, 10);
	setSilverMaterial();

	// draw hand
	glTranslatef(0, robot.handLength() / 3, 0);
	if (wireframe)
		glutWireSphere(0.5, 10, 10);
	else
		glutSolidSphere(0.5, 10, 10);
	glScalef(0.15, 0.15, 0.15);
	glTranslatef(0, 3 * robot.handLength() / 3, 0);
	drawHand(robot, wireframe);

	glPopMatrix(); // Back to lower arm and hand
	glPopMatrix(); // back to whole arm
	glPopMatrix(); // back to beginning

	glColor3f(1, 0, 0);
	glBegin(GL_POINTS);
	Vec3 pos = robot.palmPos();
	glVertex3f(pos.x, pos.y, pos.z);
	glEnd();
}

void Renderer::lineRender(const Robot &robot)
{
	Vec3 shoulder = robot.shoulderPos();
	Vec3 elbow = shoulder + robot.upperArmVector();
	Vec3 wrist = elbow + robot.lowerArmVector();
	Vec3 palm = wrist + robot.handVector();

	glColor3f(0, 0, 0.5);
	glLineWidth(2);

	glBegin(GL_LINE_STRIP);
	glVertex3f(shoulder.x, shoulder.y, shoulder.z);
	glVertex3f(elbow.x, elbow.y, elbow.z);
	glVertex3f(wrist.x, wrist.y, wrist.z);
	glVertex3f(palm.x, palm.y, palm.z);
	glEnd();


	glPointSize(5);
	glBegin(GL_POINTS);
	glVertex3f(shoulder.x, shoulder.y, shoulder.z);
	glVertex3f(elbow.x, elbow.y, elbow.z);
	glVertex3f(wrist.x, wrist.y, wrist.z);
	glVertex3f(palm.x, palm.y, palm.z);
	glEnd();

}

void Renderer::renderAxes()
{
	glLineWidth(2);
	glColor3f(1, 0, 1);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(3, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 3, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 3);
	glEnd();
}

void Renderer::drawFinger(bool wireframe)
{
	glPushMatrix();
	glTranslatef(1, 0.5, 0);
	glRotatef(-10, 0, 0, 1);
	glScalef(1, 2, 1);
	if (wireframe)
		glutWireCube(1);
	else
		glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.2, 2, 0);
	if (wireframe)
		glutWireCube(1);
	else
		glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1, 3.5, 0);
	glRotatef(10, 0, 0, 1);
	glScalef(1, 2, 1);
	if (wireframe)
		glutWireCube(1);
	else
		glutSolidCube(1);
	glPopMatrix();
}

void Renderer::drawHand(const Robot &robot, bool wireframe)
{
	int numFingers = 5;
	GLfloat angleInterval = 360.f / numFingers;
	for (GLfloat angle = 0; angle < 360; angle += angleInterval)
	{
		glPushMatrix();
		glRotatef(angle, 0, 1, 0);
		glRotatef(robot.handOpenAngle(), 0, 0, 1);
		glTranslatef(2, 0, 0);
		drawFinger(wireframe);
		glPopMatrix();
	}
}




void Renderer::render(const Enemy &enemy)
{
	if (enemy.isFrozen())
		setBlueMaterial();
	else if (enemy.isHurt())
		setRedMaterial();
	else
		setSilverMaterial();

	glPushMatrix();
	glTranslatef(enemy.position().x, 0, enemy.position().z);
	drawCylinder(enemy.radius(), 1);

	setSilverMaterial();
	if (enemy.isAttacking())
	{
		glTranslatef(0, 0.5, 0);
		drawCircle(enemy.attackRadius());
	}

	glPopMatrix();
}