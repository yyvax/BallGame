//
// Created by ���С��� on 2016/12/24.
//
#include "ball.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <GL/glut.h>

using namespace std;

float position_default[3] = { 0,0,10.0f };  //��ʼλ��
float accelerated_speed_z = -0.015f;             //z�����ʼ���ٶ�,���������ٶ�g
float accelerated_speed_air = 0.005f;             //����������ɵļ��ٶȣ�xy������ͬ
float radius_default = 0.3f;                      //�뾶
float speed_jump = 0.3f;                         //�����ٶ�

extern vector<cuboid> cuboids;
extern vector<cylinder> cylinders;

ball::ball() {
	//��ʼ��С��λ��
	for (int i = 0; i < 3; ++i) {
		position[i] = position_default[i];
	}
	//��ʼ��С����ٶ�
	accelerated_speed[0] = 0.0f;
	accelerated_speed[1] = 0.0f;
	accelerated_speed[2] = accelerated_speed_z;
	//��ʼ��С��뾶
	radius = radius_default;
	//��ʼ���ٶ�
	speed[0] = 0.0f;
	speed[1] = 0.0f;
	speed[2] = 0.0f;

	energy = false;
}

void ball::update_Position() {
	//λ��

	for (int i = 0; i < 3; ++i) {
		position[i] += speed[i];
	}
	//�ٶȸı�
	for (int i = 0; i < 3; ++i) {
		speed[i] += accelerated_speed[i];
	}
	//����
	if (accelerated_speed[0] > 0)
		accelerated_speed[0] = -accelerated_speed_air;
	else if (accelerated_speed[0] < 0)
		accelerated_speed[0] = accelerated_speed_air;

	if (accelerated_speed[1] > 0)
		accelerated_speed[1] = -accelerated_speed_air;
	else if (accelerated_speed[1] < 0)
		accelerated_speed[1] = accelerated_speed_air;



	//��ײ�ж�
	//����������Ϸ� z���򷴵�
	if (cuboid_number != -1) {
		//�����淴��
		if (position[2] < (radius + cuboids[cuboid_number].height)
			&& position[2] > (-radius + cuboids[cuboid_number].height0)) {
			bounce(0.4f, 0.4f, -0.6f); //���淴��ϵ��
			if (abs(position[2] - (radius + cuboids[cuboid_number].height)) <
				abs(position[2] - (-radius + cuboids[cuboid_number].height0))) {
				position[2] = (radius + cuboids[cuboid_number].height); //������
			}
			else if (abs(position[2] - (radius + cuboids[cuboid_number].height)) >
				abs(position[2] - (-radius + cuboids[cuboid_number].height0))) {
				position[2] = (-radius + cuboids[cuboid_number].height0); //������
				energy = false;
			}
		}
		//���淴��
		if (position[2] < (radius + 0)) {
			bounce(0.81f, 0.81f, -0.5f);  //���淴��ϵ��
			position[2] = (radius + 0); //������
		}
		extern bool c1p;
		if (cuboid_number == cuboids.size() - 1 && position[2] < (radius + cuboids[cuboid_number].height + 0.5f)
			&& position[2] > (-radius + cuboids[cuboid_number].height0)) {
			if (c1p) position[1] -= 0.02f;
			else position[1] += 0.02f;
		}

	}
	//������������Ϸ�

	else {
		//���淴��
		if (position[2] < (radius + 0)) {
			bounce(0.81f, 0.81f, -0.5f);  //���淴��ϵ��
			position[2] = (radius + 0); //������
		}
		//�����淴��
		for (auto it = cuboids.begin(); it != cuboids.end(); ++it) {
			//�������������ײ

			if (this->position[0] < ((*it).position[0] + (*it).chang / 2.0f + this->radius)
				&& this->position[0] > ((*it).position[0] - (*it).chang / 2.0f - this->radius)
				&& this->position[1] < ((*it).position[1] + (*it).kuan / 2.0f + this->radius)
				&& this->position[1] > ((*it).position[1] - (*it).kuan / 2.0f - this->radius)
				&& this->position[2] < ((*it).height + this->radius)
				&& this->position[2] > ((*it).height0 - this->radius))
			{
				float d1, d2, d3, d4;
				d1 = abs(this->position[0] - ((*it).position[0] + (*it).chang / 2.0f) - this->radius);
				d2 = abs(this->position[0] - ((*it).position[0] - (*it).chang / 2.0f) + this->radius);
				d3 = abs(this->position[1] - ((*it).position[1] + (*it).kuan / 2.0f) - this->radius);
				d4 = abs(this->position[1] - ((*it).position[1] - (*it).kuan / 2.0f) + this->radius);
				float min;
				min = d1;
				if (d2 < min) min = d2;
				if (d3 < min) min = d3;
				if (d4 < min) min = d4;
				if (min == d1) {
					bounce(-1.0f, 1.0f, 1.0f);
				}
				if (min == d2) {
					bounce(-1.0f, 1.0f, 1.0f);
				}
				if (min == d3) {
					bounce(1.0f, -1.0f, 1.0f);
				}
				if (min == d4) {
					bounce(1.0f, -1.0f, 1.0f);
				}

			}
		}

		//�����Բ���Ϸ�
		if (cylinder_number != -1) {
			//�����淴��
			if (position[2] < (radius + cylinders[cylinder_number].height)
				&& position[2] > (-radius + cylinders[cylinder_number].height0)) {
				bounce(0.4f, 0.4f, -0.6f); //���淴��ϵ��
				if (abs(position[2] - (radius + cylinders[cylinder_number].height)) <
					abs(position[2] - (-radius + cylinders[cylinder_number].height0))) {
					position[2] = (radius + cylinders[cylinder_number].height); //������
				}
				else if (abs(position[2] - (radius + cylinders[cylinder_number].height)) >
					abs(position[2] - (-radius + cylinders[cylinder_number].height0))) {
					position[2] = (-radius + cylinders[cylinder_number].height0); //������
					energy = false;
				}
			}
		}
		else {
			for (auto it = cylinders.begin(); it != cylinders.end(); ++it) {
				//�������������ײ

				if (sqrt(pow(position[0] - (*it).position[0], 2) + pow(position[1] - (*it).position[1], 2)) < (this->radius + (*it).r)
					&& this->position[2] < ((*it).height + this->radius)
						&& this->position[2] > ((*it).height0 - this->radius)) {
					float a1, a2;
					float v1[2] = { position[0] - (*it).position[0], position[1] - (*it).position[1] };
					a1 = atanf(v1[0] / v1[1]);			
					a2 = sqrtf(speed[0] * speed[0] + speed[1] * speed[1]) * cosf(a1) * 2;
					float m = a2 / sqrtf(v1[0] * v1[0] + v1[1] * v1[1]);					
					v1[0] = m*v1[0];
					v1[1] = m*v1[1];
					float vp[2];
					vp[0] = (v1[0] - speed[0]);
					vp[1] = (v1[1] - speed[1]);
					speed[0] = 0.5*vp[0];
					speed[1] = 0.5*vp[1];
					position[0] += speed[0];
					position[1] += speed[1];
				}
			}
		}
	}

	

}

//����
void ball::bounce(float kx, float ky, float kz) {
	speed[0] *= kx; //x����˥��
	speed[1] *= ky; //y����˥��
	speed[2] *= kz; //z����˥��
	energy = true;
}

//����
void ball::jump_z() {
	if (energy) {
		speed[2] = speed_jump;
		energy = false;
	}

}


//����
void ball::draw() {
	extern GLuint texture[13];
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);					//ѡ������texture[status2]  
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);		//���������ܹ���Ӱ��  
	GLUquadricObj *quadricObj = gluNewQuadric();
	gluQuadricDrawStyle(quadricObj, GLU_LINE);
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	gluQuadricTexture(quadricObj, GL_TRUE);
	glTranslatef(position[0], position[1], position[2]);
	gluSphere(quadricObj, radius, 600, 600);
	glPopMatrix();
}

void ball::move(int direction) {
	extern float eyePX;
	extern float eyePY;
	extern float eyePZ;
	extern float viewDX;
	extern float viewDY;
	extern float viewDZ;
	float r2 = sqrtf(viewDX*viewDX + viewDY*viewDY);
	switch (direction) {
	case 1: {
		accelerated_speed[0] = -0.015f* viewDY / r2;
		accelerated_speed[1] = 0.015f* viewDX / r2;
		break;
	}
	case 2: {
		accelerated_speed[0] = 0.015f* viewDY / r2;
		accelerated_speed[1] = -0.015f* viewDX / r2;
		break;
	}
	case 3: {
		accelerated_speed[0] = -0.015f * viewDX / r2;
		accelerated_speed[1] = -0.015f* viewDY / r2;
		break;
	}
	case 4: {
		accelerated_speed[0] = 0.015f * viewDX / r2;
		accelerated_speed[1] = 0.015f * viewDY / r2;
		break;
	}
	default: break;
	}

}

void ball::judge() {
	extern float eye[3];
	extern float Center[3];

	for (auto it = cuboids.begin(); it != cuboids.end(); ++it) {
		if (this->position[0] < ((*it).position[0] + (*it).chang / 2.0 + radius)
			&& this->position[0] > ((*it).position[0] - (*it).chang / 2.0 - radius)
			&& this->position[1] < ((*it).position[1] + (*it).kuan / 2.0 + radius)
			&& this->position[1] > ((*it).position[1] - (*it).kuan / 2.0 - radius)
			&& (this->position[2] > ((*it).height + this->radius)
				|| this->position[2] < ((*it).height0 - this->radius)))
		{
			this->cuboid_number = (int)(it - cuboids.begin());
			return;
		}
	}
	this->cuboid_number = -1;
}

void ball::judge2() {

	for (auto it = cylinders.begin(); it != cylinders.end(); ++it) {
		if ( sqrt(pow(position[0]-(*it).position[0],2)+pow(position[1] - (*it).position[1], 2)) < (this->radius+(*it).r)
			&& (this->position[2] > ((*it).height + this->radius)
				|| this->position[2] < ((*it).height0 - this->radius)))
		{
			this->cylinder_number = (int)(it - cylinders.begin());
			return;
		}
	}
	this->cylinder_number = -1;
}