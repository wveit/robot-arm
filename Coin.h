/*
*  Name: Wilbert Veit
*  CIN: 305175278
*  Project: CS450 HW 3 - Robot Arm Game
*/

#pragma once

#include "Vec3.h"

class Coin
{
public:
	Coin(const Vec3 &position) : mPosition(position), mAngle(0) {}
	Vec3 position() const { return mPosition; }
	float angle() const { return mAngle; }
	void update(float deltaTime) { mAngle += sAngleSpeed * deltaTime; }

private:
	Vec3 mPosition;
	float mAngle;
	static float sAngleSpeed;
};

