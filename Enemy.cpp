/*
*  Name: Wilbert Veit
*  CIN: 305175278
*  Project: CS450 HW 3 - Robot Arm Game
*/

#include "Enemy.h"
#include "World.h"
#include <iostream>
using namespace std;

Enemy::Enemy()
	: Enemy(Vec3(0,0,0))
{
}

Enemy::Enemy(const Vec3 &position)
	: mPosition(position), mRadius(3), mAttackRadius(2), mSpeed(7), mAttackTimer(0), mAttackEndTime(2), mAttackDelay(5), mHurtTimer(10), mFreezeTimer(10), mHitPoints(2)
{
}

void Enemy::update(float deltaTime, World &world)
{
	// only allow enemy to move if it's not frozen (mFreezeTimer < 3)
	mFreezeTimer += deltaTime;
	if (mFreezeTimer > 3)
	{
		Vec3 direction = world.robot.robotPosition() - mPosition;
		direction.normalize();
		Vec3 newPosition = mPosition + direction * mSpeed * deltaTime;

		// don't let enemy overlap with robot or other enemies
		if ((newPosition - world.robot.robotPosition()).magnitude() < mRadius + world.robot.baseRadius())
			newPosition = mPosition;
		for (Enemy &enemy : world.enemyList)
		{
			if (&enemy != this && (newPosition - enemy.position()).magnitude() < mRadius + enemy.radius())
			{
				newPosition = mPosition;
				break;
			}
		}

		mPosition = newPosition;

		updateAttackAnimation(deltaTime);
	}



	// add behavior if ball hits enemy or robot punches enemy
	mHurtTimer += deltaTime;
	Vec3 ballPos(world.ball.position.x, 0, world.ball.position.z);
	Vec3 ballEnemyVector = ballPos - mPosition;
	if (world.ball.active && ballEnemyVector.magnitude() < mRadius && mHurtTimer > 3)
	{
		mHitPoints--;
		mHurtTimer = 0;
		if (mHitPoints < 1)
			world.queueForRemoval(*this);
	}
	if (world.robot.isAttacking() && (world.robot.palmPos() - mPosition).magnitude() < mRadius && mHurtTimer > 3)
	{
		mHitPoints--;
		mHurtTimer = 0;
		if (mHitPoints < 1)
			world.queueForRemoval(*this);
	}
}

bool Enemy::isFrozen() const
{
	return mFreezeTimer <= 3;
}

void Enemy::freeze()
{
	mFreezeTimer = 0;
}

bool Enemy::isHurt() const
{
	return mHurtTimer <= 3;
}

void Enemy::updateAttackAnimation(float deltaTime)
{
	mAttackTimer += deltaTime;

	if (mAttackTimer < mAttackEndTime)
		mAttackRadius = 5;
	else if (mAttackTimer < mAttackEndTime + mAttackDelay)
		mAttackRadius = 2;
	else
		mAttackTimer = 0;

}

Vec3 Enemy::position() const
{
	return mPosition;
}

float Enemy::radius() const
{
	return mRadius;
}

float Enemy::attackRadius() const
{
	return mAttackRadius;
}

bool Enemy::isAttacking() const
{
	return mAttackTimer < mAttackEndTime;
}
