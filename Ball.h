/*
*  Name: Wilbert Veit
*  CIN: 305175278
*  Project: CS450 HW 3 - Robot Arm Game
*/

#pragma once

#include "Vec3.h"
class World;

class Ball
{
public:
	Ball();
	Vec3 position, direction, velocity;
	float radius, speed;
	bool visible, active;
	void setVelocity();

	void update(float deltaTime, World &world);

private:
	float gravity = 10;
};

