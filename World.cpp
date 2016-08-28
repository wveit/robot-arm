/*
*  Name: Wilbert Veit
*  CIN: 305175278
*  Project: CS450 HW 3 - Robot Arm Game
*/

#include "World.h"
#include <iostream>
using namespace std;

World::World()
{
	reset();
}


void World::update(float deltaTime)
{
	robot.update(deltaTime, *this);

	if (ball.visible)
		ball.update(deltaTime, *this);

	for (Enemy &enemy : enemyList)
		enemy.update(deltaTime, *this);

	for (Coin &coin : coinList)
		coin.update(deltaTime);

	updateNumEnemies(deltaTime);

	// if there is an enemy queued for removal, remove it
	if (mpEnemyToRemove)
	{
		auto iter = enemyList.begin();
		for (int i = 0; i < enemyList.size(); i++)
		{
			if (&*iter == mpEnemyToRemove)
			{
				// place coin where the enemy is
				coinList.push_back(Coin(iter->position()));

				// remove enemy
				enemyList.erase(iter);
				mpEnemyToRemove = NULL;
				break;
			}
			iter++;
		}
	}

	// collect coins if robot is on top of one
	
	auto iter = coinList.begin();
	while (iter != coinList.end())
	{
		if ((iter->position() - robot.robotPosition()).magnitude() < robot.baseRadius())
		{
			coinList.erase(iter);
			mNumCoinsCollected++;
			mCurrentCoins++;
			break;
		}
		++iter;
	}
}

void World::updateNumEnemies(float deltaTime)
{
	const float SecondsPerSpawn = 5;
	static float spawnTimer = 0;
	static Enemy enemy;
	spawnTimer += deltaTime;

	if (spawnTimer > SecondsPerSpawn)
	{

		// If there are fewer than 5 enemies, spawn a new enemy in a random location
		// Make sure not to spawn on top of or right next to another enemy or the robot
		if (enemyList.size() < 5)
		{
			Vec3 position;
			position.x = roomBox.left + rand() % (int)(roomBox.right - roomBox.left);
			position.z = roomBox.back + rand() % (int)(roomBox.front - roomBox.back);
			for (Enemy enemy : enemyList)
			{
				if ((position - enemy.position()).magnitude() < 4 * enemy.radius())
					return;
			}
			if ((position - robot.robotPosition()).magnitude() < 2 * (robot.baseRadius() + enemy.radius()))
			{
				return;
			}

			spawnTimer = 0;
			enemyList.push_back(Enemy(position));
		}
	}
}


void World::queueForRemoval(const Enemy &enemy)
{
	mpEnemyToRemove = &enemy;
}

bool World::robotIsInGoal() const
{
	Vec3 pos = robot.robotPosition();
	return pos.x > goalBox.left && pos.x < goalBox.right && pos.z > goalBox.back && pos.z < goalBox.front;
}

void World::reset()
{
	mNumCoinsCollected = mCurrentCoins = 0;
	roomBox = Box(-100, 100, 100, -100, 20, 0);
	robot = Robot();
	robot.setPosition(Vec3(roomBox.left + 10, roomBox.bottom, roomBox.front - 10));
	goalBox = Box(roomBox.right - 10, roomBox.right, roomBox.back + 10, roomBox.back, roomBox.top, roomBox.bottom + 0.1);
	ball = Ball();
	enemyList.clear();
	coinList.clear();
	mpEnemyToRemove = NULL;
}

int World::coinsCollected() const
{
	return mNumCoinsCollected;
}

int World::currentCoins() const
{
	return mCurrentCoins;
}

void World::spendCoins(int coinsSpent)
{
	mCurrentCoins -= coinsSpent;
}

void World::nextLevel()
{
	enemyList.clear();
	coinList.clear();
	robot.nextLevel();
}