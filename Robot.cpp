/*
*  Name: Wilbert Veit
*  CIN: 305175278
*  Project: CS450 HW 3 - Robot Arm Game
*/

#include "Robot.h"
#include "World.h"
#include <iostream>
using namespace std;

const float Pi = 3.14f;

Robot::Robot()
	: mShoulderRotation(0), mShoulderAngle(120), mElbowAngle(90), mWristRotation(0),
	mWristAngle(90), mHandOpenAngle(0), mBaseHeight(1), mBaseRadius(3),
	mUpperArmLength(5), mLowerArmLength(5), mHandLength(1),
	mAngleSpeed(200), mThrowSpeed(800), mLinearSpeed(10),
	mHurtTimer(20), mHurtEndTime(3),
	mHitPoints(10),
	mIsThrowing(false), mIsAttacking(false), mAnimationIsInProgress(false),
	mFreezeTimer(20)
{
	canThrowBall = hasFastMovement = canFreezeEnemies = false;
}

void Robot::moveRobot(const Vec3 &displacement)
{
	mPosition = mPosition + displacement;
}

void Robot::rotateShoulder(float deltaDegrees)
{
	mShoulderRotation += deltaDegrees;
}

void Robot::leanShoulder(float deltaDegrees)
{
	mShoulderAngle += deltaDegrees;
}

void Robot::bendElbow(float deltaDegrees)
{
	mElbowAngle += deltaDegrees;
}

void Robot::rotateWrist(float deltaDegrees)
{
	mWristRotation += deltaDegrees;
}

void Robot::bendWrist(float deltaDegrees)
{
	mWristAngle += deltaDegrees;
}

void Robot::openHand(float deltaDegrees)
{
	mHandOpenAngle += deltaDegrees;
}

float Robot::baseHeight() const
{
	return mBaseHeight;
}

float Robot::upperArmLength() const
{
	return mUpperArmLength;
}

float Robot::lowerArmLength() const
{
	return mLowerArmLength;
}

float Robot::handLength() const
{
	return mHandLength;
}

float Robot::baseRadius() const
{
	return mBaseRadius;
}

float Robot::shoulderRotation() const
{
	return mShoulderRotation;
}

float Robot::shoulderAngle() const
{
	return mShoulderAngle;
}

float Robot::elbowAngle() const
{
	return mElbowAngle;
}

float Robot::wristRotation() const
{
	return mWristRotation;
}

float Robot::wristAngle() const
{
	return mWristAngle;
}

float Robot::handOpenAngle() const
{
	return mHandOpenAngle;
}

Vec3 Robot::robotPosition() const
{
	return mPosition;
}

Vec3 Robot::shoulderPos() const
{
	return mPosition + Vec3(0, mBaseHeight, 0);
}

Vec3 Robot::elbowPos() const
{
	return upperArmVector() + shoulderPos();
}

Vec3 Robot::wristPos() const
{
	return elbowPos() + lowerArmVector();
}

Vec3 Robot::palmPos() const
{
	return elbowPos() + lowerArmVector() + handVector();
}

Vec3 Robot::upperArmVector() const
{
	Vec3 upperArm;
	float radShoulderAngle = mShoulderAngle * Pi / 180; // angle upper arm makes with xz plane
	float radShoulderRotation = mShoulderRotation * Pi / 180; // angle upper arm makes with x axis on xz plane

	upperArm.x = mUpperArmLength * cos(radShoulderAngle);
	upperArm.y = mUpperArmLength * sin(radShoulderAngle);
	upperArm.z = upperArm.x * sin(radShoulderRotation);
	upperArm.x = upperArm.x * cos(radShoulderRotation);

	return upperArm;
}

Vec3 Robot::lowerArmVector() const
{
	Vec3 lowerArm;
	float combinedAngle = -180 + mShoulderAngle + mElbowAngle; // angle lower arm makes with xz plane
	float radCombinedAngle = combinedAngle * Pi / 180;
	float radShoulderRotation = mShoulderRotation * Pi / 180;

	lowerArm.x = mLowerArmLength * cos(radCombinedAngle);
	lowerArm.y = mLowerArmLength * sin(radCombinedAngle);
	lowerArm.z = lowerArm.x * sin(radShoulderRotation);
	lowerArm.x = lowerArm.x * cos(radShoulderRotation);

	return lowerArm;
}

Vec3 Robot::handVector() const
{
	Vec3 hand;
	float combinedAngle = mShoulderAngle + mElbowAngle + mWristAngle; // angle hand makes with xz plane
	float radCombinedAngle = combinedAngle * Pi / 180;
	float radShoulderRotation = mShoulderRotation * Pi / 180;

	hand.x = mHandLength * cos(radCombinedAngle);
	hand.y = mHandLength * sin(radCombinedAngle);
	hand.z = hand.x * sin(radShoulderRotation);
	hand.x = hand.x * cos(radShoulderRotation);

	return hand;
}

void Robot::update(float deltaTime, World &world)
{
	float linearSpeed = mLinearSpeed;
	if (hasFastMovement)
		linearSpeed *= 2;

	// turn as needed
	rotateShoulder( (control.turnRight - control.turnLeft) * mAngleSpeed * deltaTime);

	// find preliminary new position
	Vec3 displacement((control.moveForward - control.moveBack) * linearSpeed * deltaTime, 0, (control.strafeRight - control.strafeLeft) * linearSpeed * deltaTime);
	displacement.rotatey(-mShoulderRotation);

	Vec3 newPosition = mPosition + displacement;

	mFreezeTimer += deltaTime;
	if (canFreezeEnemies && control.freezeEnemies && mFreezeTimer > 10)
	{
		for (Enemy &enemy : world.enemyList)
		{
			enemy.freeze();
		}
		mFreezeTimer = 0;
	}


	// if new position runs into an enemy, don't move
	for (Enemy &enemy : world.enemyList)
	{
		if ((newPosition - enemy.position()).magnitude() < mBaseRadius + enemy.radius())
		{
			newPosition = mPosition;
			break;
		}
	}
	

	// if new position goes past a wall, don't move in that direction
	if (newPosition.x < world.roomBox.left || newPosition.x > world.roomBox.right)
		newPosition.x = mPosition.x;
	if (newPosition.z < world.roomBox.back || newPosition.z > world.roomBox.front)
		newPosition.z = mPosition.z;
	
	mPosition = newPosition;

	// update throw or attack animations as needed
	if (control.throwBall && canThrowBall && !mAnimationIsInProgress)
	{
		mIsThrowing = true;
	}
	else if (control.attack && !mAnimationIsInProgress)
	{
		mIsAttacking = true;
	}

	if (mIsThrowing)
		updateThrowAnimation(deltaTime, world);
	else if (mIsAttacking)
		updateAttackAnimation(deltaTime, world);
	else
		mAnimationTimer += deltaTime;




	if (control.closeElbow || control.openElbow)
	{
		mIsThrowing = false;
		bendElbow((control.closeElbow - control.openElbow) * mAngleSpeed * deltaTime);
	}

	// Detect collision with enemy blades and deduct hitpoints if collision occurs.
	// But if robot has already been hurt in the last 3 seconds, do not hurt the robot.
	for (Enemy &enemy : world.enemyList)
	{
		Vec3 robotEnemyVector = enemy.position() - mPosition;
		if (robotEnemyVector.magnitude() < enemy.attackRadius() + mBaseRadius)
		{
			hurtRobot();
		}
	}


	updateHurtTimer(deltaTime);

}

void Robot::updateHurtTimer(float deltaTime)
{
	mHurtTimer += deltaTime;
}

bool Robot::isHurt() const
{
	return mHurtTimer <= 3;
}

void Robot::hurtRobot()
{
	if (mHurtTimer > 3)
	{
		mHitPoints--;
		mHurtTimer = 0;
	}
}

int Robot::hitPoints() const
{
	return mHitPoints;
}

void Robot::updateThrowAnimation(float deltaTime, World &world)
{
	if (!mAnimationIsInProgress)
	{
		mAnimationTimer = 0;
		world.ball.visible = true;
		world.ball.active = false;
		mAnimationIsInProgress = true;
		goToReadyPosition();
	}

	if (mAnimationTimer < 1)
	{
		bendElbow(mAngleSpeed * deltaTime);
	}
	else if (mAnimationTimer < 1.25)
	{
		bendElbow(-mThrowSpeed * deltaTime);
	}
	else
	{
		world.ball.direction = handVector();
		world.ball.direction.normalize();
		world.ball.setVelocity();
		mIsThrowing = false;
		world.ball.active = true;
		mAnimationIsInProgress = false;
		return;
	}

	world.ball.position = palmPos();
	mAnimationTimer += deltaTime;
}

void Robot::updateAttackAnimation(float deltaTime, World &world)
{
	static float punchSpeed = 400;
	static float retractSpeed = 100;

	if (!mAnimationIsInProgress)
	{
		mAnimationTimer = 0;
		mAnimationIsInProgress = true;
		goToReadyPosition();
	}

	if (mAnimationTimer < 0.25)
	{
		leanShoulder(-punchSpeed * deltaTime);
	}
	else if (mAnimationTimer < 1.25)
	{
		leanShoulder(retractSpeed * deltaTime);
	}
	else
	{
		mIsAttacking = false;
		mAnimationIsInProgress = false;
		return;
	}

	mAnimationTimer += deltaTime;
}

void Robot::goToReadyPosition()
{
	mShoulderAngle = 120;
	mElbowAngle = 90;
}

bool Robot::isAttacking() const
{
	return mIsAttacking;
}

void Robot::nextLevel()
{
	mPosition = Vec3();
	mShoulderRotation = 0;
	mShoulderAngle = 120;
	mElbowAngle = 90;
}