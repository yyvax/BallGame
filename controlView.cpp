#include <gl\glut.h>
#include<math.h>
#include<iostream>
#include<Windows.h>

#define GLUT_WHEEL_UP 3
#define GLUT_WHEEL_DOWN 4

float eyePX = 0;
float eyePY = -20;
float eyePZ = 20;
float viewDX = 0;
float viewDY = 0;
float viewDZ = 0;

GLfloat dx = 0, dy = 0, dz = -1.0f;
GLfloat ax = 0, ay = 0, az = 0;
GLint mx = 640, my = 360;
POINT mousePos;
float DEG_TO_RAD = 0.017453f;

extern bool gameStart;
extern bool gameOver;

void SetCamera(GLfloat x, GLfloat y)
{
	ax += x*0.05;
	ay -= y*0.08;
	float r = 1000;
	if (ay >= 90) ay = 90;
	if (ay <= -90) ay = -90;
	GLfloat dx, dy, dz;
	dy = -r*cos(ax*DEG_TO_RAD);
	dx = -r*sin(ax*DEG_TO_RAD);
	dz = -r*sin(ay);

	dz = r*sin(ay*DEG_TO_RAD);


	dx = dx * cos(ay*DEG_TO_RAD);
	dy = dy * cos(ay*DEG_TO_RAD);
	
	viewDX = eyePX + dx;
	viewDY = eyePY + dy;
	viewDZ = eyePZ + dz;
	//myDisplay();
}
void MouseMove(int x, int y)		//无点击鼠标移动
{
	
		GetCursorPos(&mousePos);
		if ((gameStart ) && ((mousePos.x != 680 / 2) || (mousePos.y != 680 / 2)))
		{
			SetCamera(float(mousePos.x) - float(680 / 2), float(mousePos.y) - float(680 / 2));
			SetCursorPos(680 / 2, 680 / 2);
			ShowCursor(false);
		}
		if (gameOver) {
			ShowCursor(true);
		}
	
}

void myMouse(int button, int state, int x, int y)
{
	extern float zoom;
	if (button == GLUT_WHEEL_UP)
	{
		zoom += 20.0f;
	}
	if (button == GLUT_WHEEL_DOWN)
	{
		zoom -= 20.0f;
	}
}
