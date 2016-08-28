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

#include "Materials.h"

GLfloat screenWidth = 800, screenHeight = 600;

World world;
Renderer renderer;
Camera cam;
bool wireframe, controlsAreOn, animationOn, showAxes, paused, isBuying, isVictorious, isDead;
float mouseDownPositionX, oldCameraRotation, cameraRotation;


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
void gameOverMessage();
void victoryMessage();
void printStats(int value);
void renderHUD();


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

void myTimer(int value)
{
	static float lastTime = glutGet(GLUT_ELAPSED_TIME);
	float currentTime = glutGet(GLUT_ELAPSED_TIME);
	float deltaTime = (currentTime - lastTime) / 1000.f;
	lastTime = currentTime;

	if (!paused)
	{
		world.update(deltaTime);
		updateCamera();

		if (world.robot.hitPoints() < 1)
		{
			paused = true;
			isDead = true;
			gameOverMessage();
		}

		if (world.robotIsInGoal())
		{
			isVictorious = true;
			paused = true;
			victoryMessage();
		}
	}

	glutPostRedisplay();
	glutTimerFunc(16, myTimer, 0);
}


void myDisplay()
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 3d rendering
	cam.setShape(60, 1, 0.1, 1000);
	cam.setModelViewMatrix();

	GLfloat light_position[] = { 1.0, 10.0, 0.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	renderer.render(world.ball);
	renderer.render(world.robot, wireframe, controlsAreOn);
	renderer.renderRoom(world.roomBox);
	renderer.renderGoal(world.goalBox);
	for (Enemy &enemy : world.enemyList)
		renderer.render(enemy);
	for (Coin &coin : world.coinList)
		renderer.renderCoin(coin);

	renderHUD();



	glutSwapBuffers();
}

void renderString(float x, float y, float z, string str)
{
	glRasterPos3f(x, y, z);
	for (int i = 0; i < str.length(); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str.at(i));
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

	glEnable(GL_LIGHTING);
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
			paused = !paused;
			break;
		case 'm':
			world.reset();
			break;
		case 'j':
			if (isVictorious)
				nextLevel();
			else if (isDead)
				reset();
			break;
		case 'k':
			// allows buying something
			isBuying = true;
			cout << "What would you like to buy?" << endl;
			cout << "Press 1 to buy 'Throw Ball Ability' for ten coins" << endl;
			cout << "Press 2 to buy 'Faster Speed' for five coins" << endl;
			cout << "Press 3 to buy 'Freeze Enemies' for 15 coins" << endl;
			cout << "Press 4 to not buy anything." << endl;
			break;
		case '1':
			if (isBuying)
			{
				if (world.robot.canThrowBall)
				{
					cout << "You already have 'Throw Ball Ability'!" << endl;
				}
				else if (world.currentCoins() >= 10)
				{
					world.spendCoins(10);
					world.robot.canThrowBall = true;
				}
				else
				{
					cout << "Sorry, you don't have enough coins for 'Throw Ball Ability'" << endl;
				}
			}
			break;
		case '2':
			if (isBuying)
			{
				if (world.robot.hasFastMovement)
				{
					cout << "You already have 'Fast Movement'!" << endl;
				}
				else if (world.currentCoins() >= 5)
				{
					world.spendCoins(5);
					world.robot.hasFastMovement = true;
				}
				else
				{
					cout << "Sorry, you don't have enough coins for 'Fast Movement'" << endl;
				}
			}
			break;
		case '3':
			if (isBuying)
			{
				if (world.robot.canFreezeEnemies)
				{
					cout << "You already have 'Freeze Enemies Ability'!" << endl;
				}
				else if (world.currentCoins() >= 15)
				{
					world.spendCoins(15);
					world.robot.canFreezeEnemies = true;
				}
				else
				{
					cout << "Sorry, you don't have enough coins for 'Freeze Enemies Ability'" << endl;
				}
			}
			break;
		case '4':
			isBuying = false;
			break;
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
	isBuying = isVictorious = isDead = false;
	wireframe = false;
	controlsAreOn = true;
	world.reset();
	showAxes = false;
	paused = false;
	cameraRotation = oldCameraRotation = 0;
	
	lightingInit();

	cam.set(Vec3(0, 2, 20), Vec3(0, 0, 0), Vec3(0, 1, 0));
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

void gameOverMessage()
{
	cout << "\n\n\nSorry, you lose, press the 'm' key to start over.\n\n\n" << endl;
}

void victoryMessage()
{
	cout << "\n\n\n";
	cout << "Congratulations!You've completed this level.\n\n";
	cout << "You can spend your hard earned coins by pressing 'k'.\n\n";
	cout << "To play the next level press 'j'\n\n" << endl;
	cout << "To start over press 'm'\n\n" << endl;
}

void printStats(int value)
{
	if (!paused)
	{
		if (system("clear")) system("cls");
		cout << "======================================================================" << endl;
		cout << "  hp   ||  num enemies  ||  current coins  ||   total coins collected" << endl;
		cout << "----------------------------------------------------------------------" << endl;
		cout << "  " << (world.robot.hitPoints() > 9 ? "" : " ") << world.robot.hitPoints()
			<< "   ||       " << world.enemyList.size()
			<< "       ||           " << world.currentCoins()
		<< "     ||   " << world.coinsCollected() << endl;
	}
}

void reset()
{
	world.reset();
}

void nextLevel()
{
	world.nextLevel();
	updateCamera();
	glutPostRedisplay();
	paused = false;
}