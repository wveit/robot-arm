/*
*  Name: Wilbert Veit
*  CIN: 305175278
*  Project: CS450 HW 3 - Robot Arm Game
*/

#pragma once

#include "Vec3.h"
#include "RobotControl.h"
class World;

class Robot
{
public:
	Robot();

	float baseHeight() const;
	float upperArmLength() const;
	float lowerArmLength() const;
	float handLength() const;
	float baseRadius() const;

	float shoulderRotation() const;
	float shoulderAngle() const;
	float elbowAngle() const;
	float wristRotation() const;
	float wristAngle() const;
	float handOpenAngle() const;
	
	Vec3 robotPosition() const;
	Vec3 shoulderPos() const;
	Vec3 elbowPos() const;
	Vec3 wristPos() const;
	Vec3 palmPos() const;

	Vec3 upperArmVector() const;
	Vec3 lowerArmVector() const;
	Vec3 handVector() const;

	bool isAttacking() const;
	bool isHurt() const;

	int hitPoints() const;

	void update(float deltaTime, World &world);
	RobotControl control;
	void nextLevel();
	void setPosition(const Vec3 &position){ mPosition = position; }


	bool canThrowBall, hasFastMovement, canFreezeEnemies;

private:
	float mShoulderRotation, mShoulderAngle, mElbowAngle, mWristRotation, mWristAngle, mHandOpenAngle;
	float mBaseHeight, mBaseRadius, mUpperArmLength, mLowerArmLength, mHandLength;
	float mAngleSpeed, mThrowSpeed, mLinearSpeed;
	float mHurtTimer, mHurtEndTime;
	Vec3 mPosition;
	int mHitPoints;
	bool mIsThrowing, mIsAttacking, mAnimationIsInProgress;

	float mAnimationTimer, mFreezeTimer;
	
	void moveRobot(const Vec3 &displacement);
	void rotateShoulder(float deltaDegrees);
	void leanShoulder(float deltaDegrees);
	void bendElbow(float deltaDegrees);
	void rotateWrist(float deltaDegrees);
	void bendWrist(float deltaDegrees);
	void openHand(float deltaDegrees);

	void updateThrowAnimation(float deltaTime, World &world);
	void updateAttackAnimation(float deltaTime, World &world);
	void hurtRobot();
	void updateHurtTimer(float deltaTime);
	void goToReadyPosition();
};

