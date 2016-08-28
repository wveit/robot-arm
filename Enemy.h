/*
*  Name: Wilbert Veit
*  CIN: 305175278
*  Project: CS450 HW 3 - Robot Arm Game
*/

#pragma once

class World;
#include "Vec3.h"

class Enemy
{
public:
	Enemy();
	Enemy(const Vec3 &position);
	void update(float deltaTime, World &world);

	Vec3 position() const;
	float radius() const;
	float attackRadius() const;
	bool isAttacking() const;
	bool isHurt() const;
	bool isFrozen() const;
	void freeze();


private:
	Vec3 mPosition;
	float mRadius, mAttackRadius, mSpeed, mAttackTimer, mAttackEndTime, mAttackDelay;
	float mHurtTimer, mFreezeTimer;
	int mHitPoints;

	void updateAttackAnimation(float deltaTime);
};

