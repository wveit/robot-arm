/*
*  Name: Wilbert Veit
*  CIN: 305175278
*  Project: CS450 HW 3 - Robot Arm Game
*/

#include "Ball.h"
#include <iostream>
using namespace std;
#include "World.h"

Ball::Ball()
{
	radius = 1;
	visible = false;
	active = false;
	speed = 100;
}

void Ball::setVelocity()
{
	velocity = direction * speed;
}

void Ball::update(float deltaTime, World &world)
{
	if (!active)
		return;

	velocity.y -= gravity * deltaTime;
	Vec3 newPosition = position + velocity * deltaTime;
	Box &room = world.roomBox;

	if (newPosition.x < room.left || newPosition.x > room.right)
	{
		velocity.x = -velocity.x;
	}
	else if (newPosition.z < room.back || newPosition.z > room.front)
	{
		velocity.z = -velocity.z;
	}
	else if (newPosition.y < room.bottom || newPosition.y > room.top)
	{
		velocity.y = -velocity.y;
		velocity = velocity * 0.95;
	}
	else
	{
		position = newPosition;
	}


}
