/*
*  Name: Wilbert Veit
*  CIN: 305175278
*  Project: CS450 HW 3 - Robot Arm Game
*/

#pragma once

class Robot;
class Ball;
class Box;
class Enemy;
class Coin;

class Renderer
{
public:
	Renderer();
	void render(const Robot &robot, bool wireframe, bool controlsAreOn);
	void renderRoom(const Box &roomBox);
	void renderGoal(const Box &goalBox);
	void renderCoin(const Coin &coin);
	void renderAxes();
	void render(const Ball &ball);
	void drawHand(const Robot &robot, bool wireframe = false);
	void drawFinger(bool wireframe = false);
	void render(const Enemy &enemy);


	void lineRender(const Robot &robot);
};

