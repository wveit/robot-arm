/*
*  Name: Wilbert Veit
*  CIN: 305175278
*  Project: CS450 HW 3 - Robot Arm Game
*/

#pragma once

#include <list>

#include "Robot.h"
#include "Ball.h"
#include "Box.h"
#include "Enemy.h"
#include "Coin.h"

class World
{
public:
	Robot robot;
	Ball ball;
	Box roomBox;
	Box goalBox;
	std::list<Enemy> enemyList;
	std::list<Coin> coinList;

	World();

	void update(float deltaTime);
	void reset();
	void queueForRemoval(const Enemy &enemy);

	bool robotIsInGoal() const;
	int coinsCollected() const;
	int currentCoins() const;
	void spendCoins(int coinsSpent);
	void nextLevel();

private:
	void updateNumEnemies(float deltaTime);
	const Enemy *mpEnemyToRemove;
	int mNumCoinsCollected, mCurrentCoins;


};

