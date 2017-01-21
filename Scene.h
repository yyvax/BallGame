#pragma once
#include <gl\glut.h>
#include <cstring>
#include <iostream>
#include "button.h"

#define GLUT_WHEEL_UP 100
#define GLUT_WHEEL_DOWN 101

extern bool gameStart;
extern bool gameOver;
extern int wWidth, wHeight;
extern void redraw();

float g_fWidth = wWidth;
float g_fHeight = wHeight;
float g_fDepth = 100;

extern GLuint texture[13];

Button *btnStart, *btnExit, *btnRestart;


void startScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	glMatrixMode(GL_PROJECTION);
	{
		glLoadIdentity();
		glOrtho(0, g_fWidth, 0, g_fHeight, 0, g_fDepth);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		btnStart->Render();
		btnExit->Render();
	}
	glutSwapBuffers();
}

void endScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	glMatrixMode(GL_PROJECTION);
	{
		glLoadIdentity();
		glOrtho(0, g_fWidth, 0, g_fHeight, 0, g_fDepth);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		btnRestart->Render();
		btnExit->Render();
	}

	glutSwapBuffers();

}

void mouse(int button, int state, int x, int y)
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
	if (!btnStart || !btnExit || !btnRestart)
		return;
	if (button == GLUT_LEFT_BUTTON)
		switch (state)
		{
		case GLUT_DOWN:
			//左键按下：
			if ((!gameStart & !gameOver) && btnStart->OnMouseDown(x, y))
			{
				//开始游戏
				gameStart = true;
				gameOver = false;
				glutDisplayFunc(redraw);
			}
			else if (btnExit->OnMouseDown(x, y))//开始界面or结束界面，按下退出键
			{
				//退出游戏
				gameStart = false;
				gameOver = false;
				exit(0);
			}
			else if ((!gameStart & gameOver) && btnRestart->OnMouseDown(x, y))
			{
				//重新开始
				gameStart = false;
				gameOver = false;
				glutDisplayFunc(startScene);
			}
			break;

		case GLUT_UP:
			btnStart->OnMouseUp();
			btnExit->OnMouseUp();
			btnRestart->OnMouseUp();
			break;
		}
	glutPostRedisplay();
}

