/*
*  Name: Wilbert Veit
*  CIN: 305175278
*  Project: CS450 HW 3 - Robot Arm Game
*/

#pragma once

struct RobotControl
{
public:
	bool turnLeft = false;
	bool turnRight = false;
	bool leanBack = false;
	bool leanForward = false;
	bool openElbow = false;
	bool closeElbow = false;
	bool wristForward = false;
	bool wristBack = false;
	bool openHand = false;
	bool closeHand = false;
	bool moveForward = false;
	bool moveBack = false;
	bool activate = false;
	bool throwBall = false;
	bool attack = false;
	bool strafeRight = false;
	bool strafeLeft = false;
	bool freezeEnemies = false;
};

