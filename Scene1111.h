#pragma once
#include <cstring>
#include <iostream>
#inlcude <gl/glut.h> 
#include "button.h"


extern bool gameStart;
extern bool gameOver;
extern int wWidth, wHeight;
extern void redraw();

float g_fWidth = wWidth;
float g_fHeight = wHeight;
float g_fDepth = 100;

extern GLuint texture[13];


//��ʼ����
Button *btnStart = new Button(texture[10], wHeight / 2 + 50);///////////////////////
Button *btnExit = new Button(texture[11], wHeight / 2 - 50);
//��������
Button *btnRestart = new Button(texture[12], wHeight / 2 + 50);
//Button *btnExit = new Button(texture[1], wHeight / 2 + 50);

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
	if (!btnStart || !btnExit || !btnRestart)
		return;
	if (button == GLUT_LEFT_BUTTON)
		switch (state)
		{
		case GLUT_DOWN:
			//������£�
			if ((!gameStart & !gameOver) && btnStart->OnMouseDown(x, y))
			{
				//��ʼ��Ϸ
				gameStart = true;
				gameOver = false;
				glutDisplayFunc(redraw);
			}
			else if (btnExit->OnMouseDown(x, y))//��ʼ����or�������棬�����˳���
			{
				//�˳���Ϸ
				gameStart = false;
				gameOver = false;
				exit(0);
			}
			else if (gameStart && gameOver)//С����أ���Ϸ������������������
			{
				//��Ϸ����
				gameStart = false;
				glutDisplayFunc(endScene);
			}
			else if ((!gameStart & gameOver) && btnRestart->OnMouseDown(x, y))
			{
				//���¿�ʼ
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


