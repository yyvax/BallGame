#pragma once
#include <iostream>
#include <gl\glut.h>
#include <string>
#include <Windows.h>

using namespace std;

extern int wHeight;
extern int wWidth;
GLfloat btnWidth = 140;
GLfloat btnHeight = 40;

void drawCube();
class Button
{
	float m_fPosX;      //表示在正交投影坐标系(左下角为坐标原点)的坐标，  
	float m_fPosY;
	float m_fWidth = btnWidth;     //屏幕像素单位  
	float m_fHeight = btnHeight;
	bool m_bPressed = false;
	GLuint m_texture;

public:
	Button(GLuint tex, float posY)
	{
		m_fPosX = wWidth / 2 - btnWidth / 2;
		m_fPosY = posY;
		m_texture = tex;
	}

	void Render()
	{
		glPushMatrix();
		{
			//将中心位于原点的cube移动到使cube左下角坐标为m_fPosX,m_fPosY的位置  
			//必须考虑cube的自身长宽  
			glTranslatef(m_fPosX + m_fWidth / 2, m_fPosY + m_fHeight / 2, -2.0);    //-2.0只是为了按钮可见  
			if (m_bPressed)
			{
				//double scaleAmt = 10.0 * sin( (double)rand() );  
				//double scaleAmt = sin( (double)rand() );  
				glScalef(0.9, 0.9, 1.0);
			}
			//cube中心位于原点  
			glScalef(m_fWidth, m_fHeight, 5.0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, m_texture);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
			//textureLoad::drawCube(1, 1);
			drawCube();
			glDisable(GL_TEXTURE_2D);
		}
		glPopMatrix();
	}


	bool OnMouseDown(int mousex, int mousey)
	{
		//鼠标的位置：mousex，mousey坐标系是原点位于左上角  
		//必须将mousey变换到原点位于左下角的坐标系中  
		mousey = 500 - mousey;
		if (mousex > m_fPosX && mousex < m_fPosX + m_fWidth &&
			mousey > m_fPosY && mousey < m_fPosY + m_fHeight)
		{
			printf("button x:%d  y:%d.... \n", mousex, mousey);
			m_bPressed = true;

			return true;
		}
		return false;
	}
	void OnMouseUp()
	{
		m_bPressed = false;
	}
};