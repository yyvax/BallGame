//
// Created by ���С��� on 2017/1/9.
//
#include <GL/glut.h>    /// linux ��ͷ�ļ����ڵ�
#include <cmath>
using namespace std;

const GLfloat PI = 3.14;

static GLfloat yrotate = PI / 4; /// angle between y-axis and look direction
static GLfloat xrotate = PI / 4; /// angle between x-axis and look direction
static GLfloat celength = 20.0f;/// lenght between center and eye

static GLfloat mSpeed = 0.4f; /// center move speed
static GLfloat rSpeed = 0.02f; /// rotate speed
static GLfloat lSpeed = 0.4f; /// reserved

GLboolean mouserdown = GL_FALSE;
GLboolean mouseldown = GL_FALSE;
GLboolean mousemdown = GL_FALSE;
static GLint mousex = 0, mousey = 0;

extern float eye[3];
extern float Center[3];

void CalEyePosition()
{
	if (yrotate > PI / 2.2) yrotate = PI / 2.2;   /// ���ƿ��÷���
	if (yrotate < 0.01)  yrotate = 0.01;
	if (xrotate > 2 * PI)   xrotate = 0.01;
	if (xrotate < 0)   xrotate = 2 * PI;
	if (celength > 50)  celength = 50;     ///  ���ž�������
	if (celength < 5)   celength = 5;
	/// ��������������ϵ���� eye ��λ�ã�
	eye[0] = Center[0] + celength * sin(yrotate) * cos(xrotate);
	eye[2] = Center[2] + celength * sin(yrotate) * sin(xrotate);
	eye[1] = Center[1] + celength * cos(yrotate);
}

/// center moves
void MoveBackward()              /// center �������߷���ˮƽ����ƶ�
{
	Center[0] += mSpeed * cos(xrotate);
	Center[2] += mSpeed * sin(xrotate);
	CalEyePosition();
}

void MoveForward()
{
	Center[0] -= mSpeed * cos(xrotate);
	Center[2] -= mSpeed * sin(xrotate);
	CalEyePosition();
}

/// visual angle rotates
void RotateLeft()
{
	xrotate -= rSpeed;
	CalEyePosition();
}

void RotateRight()
{
	xrotate += rSpeed;
	CalEyePosition();
}

void RotateUp()
{
	yrotate += rSpeed;
	CalEyePosition();
}

void RotateDown()
{
	yrotate -= rSpeed;
	CalEyePosition();
}


/// CALLBACK func for mouse kicks
void MouseFunc(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		if (button == GLUT_RIGHT_BUTTON) mouserdown = GL_TRUE;
		if (button == GLUT_LEFT_BUTTON) mouseldown = GL_TRUE;
		if (button == GLUT_MIDDLE_BUTTON)mousemdown = GL_TRUE;
	}
	else
	{
		if (button == GLUT_RIGHT_BUTTON) mouserdown = GL_FALSE;
		if (button == GLUT_LEFT_BUTTON) mouseldown = GL_FALSE;
		if (button == GLUT_MIDDLE_BUTTON)mousemdown = GL_FALSE;
	}
	mousex = x, mousey = y;
}

/// CALLBACK func for mouse motions
void MouseMotion(int x, int y)
{
	if (mouserdown == GL_TRUE)
	{       /// �����Ե������ǵ�����ת�ٶȵģ�������ã��ﵽ�Լ���Ҫ�ٶȼ���
		xrotate += (x - mousex) / 80.0f;
		yrotate -= (y - mousey) / 120.0f;
	}

	if (mouseldown == GL_TRUE)
	{
		celength += (y - mousey) / 25.0f;
	}
	mousex = x, mousey = y;
	CalEyePosition();
	glutPostRedisplay();
}

void LookAt()            /// ���� gluLookAt(), ��Ҫ��ֱ�ӵ���Ҫÿ�ζ�д�ü�����������
{
	CalEyePosition();
	gluLookAt(eye[0], eye[1], eye[2],
		Center[0], Center[1], Center[2],
		0, 1, 0);
}