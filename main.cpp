// glutEx1.cpp : 定义控制台应用程序的入口点。
//


#include <stdlib.h>
#include <vector>
#include "ball.h"
#include <iostream>
#include <GL/glut.h>
#include "Scene.h"
#include "screenShot.h"
using namespace std;

float fTranslate;
float fRotate;
float fScale = 1.0f;	// set inital scale value to 1.0f

bool gameStart = false;
bool gameOver = false; 
int wHeight = 680;
int wWidth = 680;
extern float accelerated_speed_z;
float eye[3] = { 0, -20, 20 };
float Center[3] = { 0, 0, 0 };
float light_pos[] = { 0, -8, 16 };

float zoom = 500.0f;

ball BALL;

int status = 1;
GLuint texture[13];

void Draw_Leg(float x, float y, float chang, float kuan, float height, float height0, int status);

void Draw_Cylinder(float x, float y, float r, float height, float height0);

void draw();

void Init();

void getLineNum(string addrstr);

int readfile(string addrstr);

//void CalEyePosition();

extern vector<cuboid> cuboids;
extern vector<cylinder> cylinders;
void cuboid_Position_Update();
float v = 0.5;
GLfloat cos_alpha;
GLfloat sin_theta;
GLfloat cos_theta;
GLfloat sin_phi;

void Draw_Scene() // This function draws a triangle with RGB colors
{
	glPolygonMode(GL_FRONT, GL_FILL);
	glColor3f(0.5, 0.5, 0.5);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(1, 1, 1);
	
	for (auto it = cuboids.begin(); it != cuboids.end(); ++it) {
		Draw_Leg((*it).position[0], (*it).position[1], (*it).chang, (*it).kuan, (*it).height, (*it).height0, 1);
	}
	for (auto it = cylinders.begin(); it != cylinders.end(); ++it) {
		Draw_Cylinder((*it).position[0], (*it).position[1], (*it).r, (*it).height, (*it).height0);
	}
	for (int i = -30; i < 30; i += 1)
		for (int j = -30; j < 30; j+= 1)
			Draw_Leg(i, j, 0.95, 0.95, 0, -10, 2);
	
	for (int i = -30; i < 30; i += 1)
		for (int j = -30; j < 30; j += 1)
			Draw_Leg(i, j, 0.95, 0.95, 50, 40, 4);
	//
	for (int i = -30; i < 30; i += 1)
		for (int j = 0; j < 50; j += 2)
			Draw_Leg(-30, i, 0.95, 0.95, j+2, j, 4);

	for (int i = -30; i < 30; i += 1)
		for (int j = 0; j < 50; j += 2)
			Draw_Leg(30, i, 0.95, 0.95, j + 2, j, 4);

	for (int i = -30; i < 30; i += 1)
		for (int j = 0; j < 50; j += 2)
			Draw_Leg(i, -30, 0.95, 0.95, j + 2, j, 4);

	for (int i = -30; i < 30; i += 1)
		for (int j = 0; j < 50; j += 2)
			Draw_Leg(i, 30, 0.95, 0.95, j + 2, j, 4);
	if (BALL.position[0] > 6.0 && BALL.position[0] < 9.0
		&& BALL.position[1] > -11.6 && BALL.position[1] < -10.4
		&& BALL.position[2] > 8.9 && BALL.position[2] < 10.1) {
	
		gameOver = true;
		BALL.position[0] = 0.0f;
		BALL.position[1] = 0.0f;
		BALL.position[2] = 10.0f;
		BALL.speed[0] = 0.0f;
		BALL.speed[1] = 0.0f;
		BALL.speed[2] = 0.0f;
		BALL.accelerated_speed[0] = 0.0f;
		BALL.accelerated_speed[1] = 0.0f;
		BALL.accelerated_speed[2] = accelerated_speed_z;
	}

	if (!gameOver)
		Draw_Leg(8, -11, 1, 1, 10, 9.5, 2);
	else {
		Draw_Leg(8, -11, 1.01, 1.01, 10.01, 9.501, 3);
	}
	
	BALL.judge();
	BALL.judge2();
	cuboid_Position_Update();
	BALL.update_Position();
	BALL.draw();
}


void updateView(int width, int height)
{
	glViewport(0, 0, width, height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	float whRatio = (GLfloat)width / (GLfloat)height;

	gluPerspective(75.0f, whRatio, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
} 

void reshape(int width, int height)
{
	if (height == 0)										// Prevent A Divide By Zero By
	{
		height = 1;										// Making Height Equal One
	}

	wHeight = height;
	wWidth = width;

	updateView(wHeight, wWidth);
}

void idle()
{
	glutPostRedisplay();
}



void key(unsigned char k, int x, int y)
{
	switch (k) {
	default:
		break;

	case 27:
		exit(0);

	case 'A': //a
		BALL.move(LEFT);
		break;

	case 'D': //d
		BALL.move(RIGHT);
		break;

	case 'S': //s
		BALL.move(DOWN);
		break;

	case 'W': //w
		BALL.move(UP);
		break;

	case 'a': //a
		BALL.move(LEFT);
		break;

	case 'd': //d
		BALL.move(RIGHT);
		break;

	case 's': //s
		BALL.move(DOWN);
		break;

	case 'w': //w
		BALL.move(UP);
		break;

	case ' ':
		BALL.jump_z();
		break;
	case 't': {
		if (status == 0) {
			status = 1;
			break;
		}
		if (status == 1)
			status = 0;
		break;
	}
	case 'T': {
		if (status == 0) {
			status = 1;
			break;
		}
		if (status == 1)
			status = 0;
		break;
	}
	case 'l': {
		SaveScreenShot();
		break;
	}
	}
	updateView(wHeight, wWidth);
}


void redraw()
{
	extern float eyePX;
	extern float eyePY;
	extern float eyePZ;
	extern float viewDX;
	extern float viewDY;
	extern float viewDZ;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();									// Reset The Current Modelview Matrix

	eyePX = BALL.position[0] - viewDX / zoom;
	eyePY = BALL.position[1] - viewDY / zoom;
	eyePZ = BALL.position[2];

	//gluLookAt(eye[0], eye[1], eye[2],
	//	Center[0], Center[1], Center[2],
	//	0, 0, 1);				// 场景（0，0，0）的视点中心 (0,5,50)，Y轴向上
	gluLookAt(eyePX,eyePY,eyePZ+1,
		viewDX, viewDY, viewDZ,
		0, 0, 1);				// 场景（0，0，0）的视点中心 (0,5,50)，Y轴向上
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat yellow[] = { 0.8, 0.8, 0,1.0 };


	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	glLightfv(GL_LIGHT0, GL_AMBIENT, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);

	glEnable(GL_LIGHT0);

	glPushMatrix();
	Draw_Scene();
	//draw();
	glPopMatrix();

	if (gameStart && gameOver)//小球落地，游戏结束，跳到结束界面
	{
		//游戏结束
		gameStart = false;
		glutDisplayFunc(endScene);
	}

	glutSwapBuffers();
}

void judgeCube();
void judgeCylinder();
void MouseMove(int x, int y);
void myMouse(int button, int state, int x, int y);


int main(int argc, char *argv[])
{
	getLineNum("4.obj");
	readfile("4.obj");
	judgeCube();
	judgeCylinder();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(680, 680);
	glutCreateWindow("................");
	Init();

	//开始界面
	btnStart = new Button(texture[10], 50);
	btnExit = new Button(texture[11], - 50);
	//结束界面
	btnRestart = new Button(texture[12], 50);
	//btnExit = new Button(texture[1], wHeight / 2 + 50);

	glutDisplayFunc(endScene);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutPassiveMotionFunc(MouseMove);
	glutMouseFunc(&mouse);
	glutIdleFunc(idle);

	glutMainLoop();

	return 0;
}


