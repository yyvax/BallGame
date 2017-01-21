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
	float m_fPosX;      //��ʾ������ͶӰ����ϵ(���½�Ϊ����ԭ��)�����꣬  
	float m_fPosY;
	float m_fWidth = btnWidth;     //��Ļ���ص�λ  
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
			//������λ��ԭ���cube�ƶ���ʹcube���½�����Ϊm_fPosX,m_fPosY��λ��  
			//���뿼��cube��������  
			glTranslatef(m_fPosX + m_fWidth / 2, m_fPosY + m_fHeight / 2, -2.0);    //-2.0ֻ��Ϊ�˰�ť�ɼ�  
			if (m_bPressed)
			{
				//double scaleAmt = 10.0 * sin( (double)rand() );  
				//double scaleAmt = sin( (double)rand() );  
				glScalef(0.9, 0.9, 1.0);
			}
			//cube����λ��ԭ��  
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
		//����λ�ã�mousex��mousey����ϵ��ԭ��λ�����Ͻ�  
		//���뽫mousey�任��ԭ��λ�����½ǵ�����ϵ��  
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