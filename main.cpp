/*
*  Name: Wilbert Veit
*  CIN: 305175278
*  Project: CS450 HW 3 - Robot Arm Game
*/


#include <cstdlib>
#include <ctime>
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;
#include "World.h"
#include "Renderer.h"
#include "Camera.h"
#include <stack>

#include "Materials.h"

GLfloat screenWidth = 800, screenHeight = 600;

enum Menu{None, Pause, Instructions, Welcome, Buy, InGoal, Death} currentMenu;
stack<Menu> menuStack;
World world;
Renderer renderer;
Camera cam;
float mouseDownPositionX, oldCameraRotation, cameraRotation;
string buyMessage;


void init();
void reset();
void nextLevel();
void lightingInit();
void myDisplay();
void myMouse(int button, int status, int x, int y);
void myMotion(int x, int y);
void myKeyboard(unsigned char key, int x, int y);
void myKeyboardUp(unsigned char key, int x, int y);
void mySpecial(int key, int x, int y);
void mySpecialUp(int key, int x, int y);
void myTimer(int value);
void updateCamera();
void renderHUD();
void renderMenu(const string &str);
void renderString(float x, float y, float z, string str);


int main(int argc, char **argv)
{
	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("CS450 HW3 - world.robot Game");

	glutDisplayFunc(myDisplay);
	glutMouseFunc(myMouse);
	glutMotionFunc(myMotion);
	glutKeyboardFunc(myKeyboard);
	glutKeyboardUpFunc(myKeyboardUp);
	glutSpecialFunc(mySpecial);
	glutSpecialUpFunc(mySpecialUp);
	glutTimerFunc(16, myTimer, 0);

	init();
	glutMainLoop();
}

void renderMenu(const string &str)
{
	glDisable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1, 1, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1, 0, 0);
	glRectf(-1, -1, 1, 1);
	glColor3f(0, 0, 1);
	
	renderString(-0.8, 0.92, 1, str);

}

void myTimer(int value)
{
	static float lastTime = glutGet(GLUT_ELAPSED_TIME);
	float currentTime = glutGet(GLUT_ELAPSED_TIME);
	float deltaTime = (currentTime - lastTime) / 1000.f;
	lastTime = currentTime;

	if (currentMenu == Menu::None)
	{
		world.update(deltaTime);
		updateCamera();

		if (world.robot.hitPoints() < 1)
		{
			currentMenu = Menu::Death;
		}

		if (world.robotIsInGoal())
		{
			currentMenu = Menu::InGoal;
		}
	}

	glutPostRedisplay();
	glutTimerFunc(16, myTimer, 0);
}


void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (currentMenu == Menu::Pause)
	{
		renderMenu(string("---PAUSE MENU---\nPress a number key to select the option:\n")
			+"(1) resume game\n(2) see instructions\n(3) restart game");
	}
	else if (currentMenu == Menu::Instructions)
	{
		renderMenu(string("---INSTRUCTIONS---\nPress the (1) key to return to previous menu\n\n")
			+ "In this game you are being attacked by vicious enemies... but you control a robot arm!\n"
			+ "Defeat enemies and collect their gold coins to purchase upgrades.\n"
			+ "See how long you can survive.\n\n"
			+ "* The red square is a safe spot. You can buy upgrades there too.\n"
			+ "* W, A, S, D keys move the robot arm.\n"
			+ "* 7, 8 and 9 keys use the robot's attack abilities\n"
			+ "    (You'll start off with only the punch ability but can purchase others later).\n"
			+ "* Hit P at any time to pause the game.\n"
			+ "* Look around by dragging the mouse. Left Mouse Button is normal look around.\n"
			+ "* Right Mouse Button is look around with a snap-back when you let go.\n"		
		);
	}
	else if (currentMenu == Menu::InGoal)
	{
		renderMenu(string("---CHECKPOINT---\nYou made it to safety!\n")
			+"Press a number key to select the option:\n(1) resume game\n(2) buy something");
	}
	else if (currentMenu == Menu::Buy)
	{
		renderMenu(string("---STORE---\nBuy upgrades to help fight enemies!\n")
			+"You have " + to_string(world.currentCoins()) + " Gold\n"
			+ "Press a number key to select the option:\n"
			+ "(1) Buy Faster Speed for 5 Gold\n"
			+ "(2) Buy Throw Ball ability for 10 Gold\n"
			+ "(3) Buy Freeze ability for 15 Gold\n"
			+ "(4) Go back\n\n\n\n\n\n" + buyMessage);
	}
	else if (currentMenu == Menu::Death)
	{
		renderMenu(string("---DEATH---\nSorry... you died.\nPress (1) when you're ready to restart the game\n\n\n")
			+ "Coins Collected: " + to_string(world.coinsCollected())
			+ "\nCurrent Coins: " + to_string(world.currentCoins())
			+ "\nAbilities Purchased: " + (world.robot.hasFastMovement ? "Fast Movement, " : "") 
			+ (world.robot.canThrowBall ? "Throw Ball, " : "") + (world.robot.canFreezeEnemies ? "Freeze Enemies" : ""));
	}
	else if (currentMenu == Menu::Welcome)
	{
		renderMenu(string("---WELCOME---\nEnjoy the Robot-Arm Game.\nPress a number key to select the option:\n")
			+ "(1) start the game\n"
			+ "(2) see instructions");
	}
	else
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);

		// 3d rendering
		cam.setShape(60, 1, 0.1, 1000);
		cam.setModelViewMatrix();

		GLfloat light_position[] = { 1.0, 10.0, 0.0, 0.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);

		renderer.render(world.ball);
		renderer.render(world.robot, false, true);
		renderer.renderRoom(world.roomBox);
		renderer.renderGoal(world.goalBox);
		for (Enemy &enemy : world.enemyList)
			renderer.render(enemy);
		for (Coin &coin : world.coinList)
			renderer.renderCoin(coin);

		renderHUD();
	}
	


	glutSwapBuffers();
}

void renderString(float x, float y, float z, string str)
{
	glRasterPos3f(x, y, z);
	float currentY = y;
	for (int i = 0; i < str.length(); i++)
	{
		char currentChar = str.at(i);
		if (currentChar == '\n')
		{
			currentY -= 0.05;
			glRasterPos3f(x, currentY, z);
		}
		else
		{
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str.at(i));
		}
	}
}

void renderHUD()
{
	glDisable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1, 1, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1, 0, 0);
	glRectf(-1, 0.9, 1, 1);
	glColor3f(0, 0, 1);
	stringstream ss;
	ss << "hp: " << (world.robot.hitPoints() > 9 ? "" : " ") << world.robot.hitPoints()<< "  numEnemies: " << world.enemyList.size()
		<< "    current coins: " << world.currentCoins() << "    total coins collected: " << world.coinsCollected();
	
	renderString(-0.8, 0.92, 1, ss.str());

}

void myMouse(int button, int status, int x, int y)
{
	if (status == GLUT_DOWN)
	{
		mouseDownPositionX = x;
	}
	else if (button == GLUT_LEFT_BUTTON)
	{
		oldCameraRotation = cameraRotation;
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		cameraRotation = oldCameraRotation = 0;
	}
}

void myMotion(int x, int y)
{
	if (mouseDownPositionX != 0)
		cameraRotation = (x - mouseDownPositionX) / 5 + oldCameraRotation;
}


void myKeyboard(unsigned char key, int x, int y)
{
	if (currentMenu == Menu::None)
	{
		switch (key)
		{
		case 'w':
			world.robot.control.moveForward = true;
			break;
		case 's':
			world.robot.control.moveBack = true;
			break;
		case 'a':
			world.robot.control.turnLeft = true;
			break;
		case 'd':
			world.robot.control.turnRight = true;
			break;
		case 'q':
			world.robot.control.strafeLeft = true;
			break;
		case 'e':
			world.robot.control.strafeRight = true;
			break;
		case '7':
			world.robot.control.attack = true;
			break;
		case '8':
			world.robot.control.throwBall = true;
			break;
		case '9':
			world.robot.control.freezeEnemies = true;
			break;
		case 'p':
			menuStack.push(currentMenu);
			currentMenu = Menu::Pause;
			break;
		}
	}
	else if (currentMenu == Menu::Pause)
	{
		switch (key)
		{
		case '1':
			currentMenu = menuStack.top();
			menuStack.pop();
			break;
		case '2':
			menuStack.push(currentMenu);
			currentMenu = Menu::Instructions;
			break;
		case '3':
			reset();
			break;
		}
	}
	else if (currentMenu == Menu::Instructions)
	{
		if (key == '1')
		{
			currentMenu = menuStack.top();
			menuStack.pop();
		}

	}
	else if (currentMenu == Menu::InGoal)
	{
		if (key == '1')
		{
			nextLevel();
		}
		else if (key == '2')
		{
			menuStack.push(currentMenu);
			currentMenu = Menu::Buy;
			buyMessage = "";
		}
	}
	else if (currentMenu == Menu::Buy)
	{
		if (key == '1')
		{
			if (world.currentCoins() >= 5)
			{
				if (world.robot.hasFastMovement)
				{
					buyMessage = "Purchase Unsuccessful: Already have fast movement";
				}
				else
				{
					buyMessage = "Purchase Successful: You now have fast movement";
					world.spendCoins(5);
					world.robot.hasFastMovement = true;
				}
			}
			else
			{
				buyMessage = "Purchase Unsuccessful: You don't have enough coins";
			}
		}
		else if (key == '2')
		{
			if (world.currentCoins() >= 10)
			{
				if (world.robot.canThrowBall)
				{
					buyMessage = "Purchase Unsuccessful: Already can throw ball";
				}
				else
				{
					buyMessage = "Purchase Successful: You now can throw ball";
					world.spendCoins(10);
					world.robot.canThrowBall = true;
				}
			}
			else
			{
				buyMessage = "Purchase Unsuccessful: You don't have enough coins";
			}
		}
		else if (key == '3')
		{
			if (world.currentCoins() >= 15)
			{
				if (world.robot.canFreezeEnemies)
				{
					buyMessage = "Purchase Unsuccessful: Already can freeze enemies";
				}
				else
				{
					buyMessage = "Purchase Successful: You now can freeze enemies";
					world.spendCoins(15);
					world.robot.canFreezeEnemies = true;
				}
			}
			else
			{
				buyMessage = "Purchase Unsuccessful: You don't have enough coins";
			}
		}
		else if (key == '4')
		{
			currentMenu = menuStack.top();
			menuStack.pop();
		}
	}
	else if (currentMenu == Menu::Death)
	{
		if (key == '1')
		{
			reset();
		}
	}
	else if (currentMenu == Menu::Welcome)
	{
		if (key == '1')
		{
			currentMenu = None;
		}
		else if (key == '2')
		{
			menuStack.push(currentMenu);
			currentMenu = Menu::Instructions;
		}
	}
}

void myKeyboardUp(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'w':
			world.robot.control.moveForward = false;
			break;
		case 's':
			world.robot.control.moveBack = false;
			break;
		case 'a':
			world.robot.control.turnLeft = false;
			break;
		case 'd':
			world.robot.control.turnRight = false;
			break;
		case 'q':
			world.robot.control.strafeLeft = false;
			break;
		case 'e':
			world.robot.control.strafeRight = false;
			break;		
		case '7':
			world.robot.control.attack = false;
			break;
		case '8':
			world.robot.control.throwBall = false;
			break;
		case '9':
			world.robot.control.freezeEnemies = false;
			break;
	}
}

void mySpecial(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_UP:
			world.robot.control.closeElbow = true;
			break;
		case GLUT_KEY_DOWN:
			world.robot.control.openElbow = true;
			break;
		case GLUT_KEY_LEFT:
			break;
		case GLUT_KEY_RIGHT:
			break;
	}
}

void mySpecialUp(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_UP:
			world.robot.control.closeElbow = false;
			break;
		case GLUT_KEY_DOWN:
			world.robot.control.openElbow = false;
			break;
		case GLUT_KEY_LEFT:
			break;
		case GLUT_KEY_RIGHT:
			break;
	}
}

void init()
{
	reset();
	
	lightingInit();

	cam.setShape(60, 1, 0.01, 1000);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glClearColor(0, 0, 0, 0);
	glColor3f(0, 0, 0);

	glLineWidth(3);
	glPointSize(3);
}

void lightingInit()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);

	GLfloat ambient[] = { 1.f, 1.f, 1.f, 1.f };
	GLfloat light_position[] = { 0.0, 2.0, 0.0, 0.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

void updateCamera()
{
	Vec3 camVec(20, 0, 0);
	camVec.rotatey(-world.robot.shoulderRotation() - cameraRotation);
	Vec3 robotPos = world.robot.robotPosition();
	Vec3 eye(robotPos.x - camVec.x, 8, robotPos.z - camVec.z);
	cam.set(eye, robotPos, Vec3(0, 1, 0));
}


void reset()
{
	world.reset();
	while (!menuStack.empty())
		menuStack.pop();
	currentMenu = Menu::Welcome;
	cameraRotation = oldCameraRotation = 0;
	cam.set(Vec3(0, 2, 20), Vec3(0, 0, 0), Vec3(0, 1, 0));
}

void nextLevel()
{
	while (!menuStack.empty())
		menuStack.pop();
	currentMenu = Menu::None;
	world.nextLevel();
	updateCamera();
	glutPostRedisplay();
}