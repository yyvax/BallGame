//
// Created by 天才小程峥 on 2016/12/24.
//
#include "ball.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <GL/glut.h>

using namespace std;

float position_default[3] = { 0,0,10.0f };  //初始位置
float accelerated_speed_z = -0.015f;             //z方向初始加速度,即重力加速度g
float accelerated_speed_air = 0.005f;             //空气阻力造成的加速度，xy方向相同
float radius_default = 0.3f;                      //半径
float speed_jump = 0.3f;                         //弹跳速度

extern vector<cuboid> cuboids;
extern vector<cylinder> cylinders;

ball::ball() {
	//初始化小球位置
	for (int i = 0; i < 3; ++i) {
		position[i] = position_default[i];
	}
	//初始化小球加速度
	accelerated_speed[0] = 0.0f;
	accelerated_speed[1] = 0.0f;
	accelerated_speed[2] = accelerated_speed_z;
	//初始化小球半径
	radius = radius_default;
	//初始化速度
	speed[0] = 0.0f;
	speed[1] = 0.0f;
	speed[2] = 0.0f;

	energy = false;
}

void ball::update_Position() {
	//位移

	for (int i = 0; i < 3; ++i) {
		position[i] += speed[i];
	}
	//速度改变
	for (int i = 0; i < 3; ++i) {
		speed[i] += accelerated_speed[i];
	}
	//减速
	if (accelerated_speed[0] > 0)
		accelerated_speed[0] = -accelerated_speed_air;
	else if (accelerated_speed[0] < 0)
		accelerated_speed[0] = accelerated_speed_air;

	if (accelerated_speed[1] > 0)
		accelerated_speed[1] = -accelerated_speed_air;
	else if (accelerated_speed[1] < 0)
		accelerated_speed[1] = accelerated_speed_air;



	//碰撞判断
	//如果在柱子上方 z方向反弹
	if (cuboid_number != -1) {
		//上下面反弹
		if (position[2] < (radius + cuboids[cuboid_number].height)
			&& position[2] > (-radius + cuboids[cuboid_number].height0)) {
			bounce(0.4f, 0.4f, -0.6f); //柱面反弹系数
			if (abs(position[2] - (radius + cuboids[cuboid_number].height)) <
				abs(position[2] - (-radius + cuboids[cuboid_number].height0))) {
				position[2] = (radius + cuboids[cuboid_number].height); //防卡死
			}
			else if (abs(position[2] - (radius + cuboids[cuboid_number].height)) >
				abs(position[2] - (-radius + cuboids[cuboid_number].height0))) {
				position[2] = (-radius + cuboids[cuboid_number].height0); //防卡死
				energy = false;
			}
		}
		//地面反弹
		if (position[2] < (radius + 0)) {
			bounce(0.81f, 0.81f, -0.5f);  //地面反弹系数
			position[2] = (radius + 0); //防卡死
		}
		extern bool c1p;
		if (cuboid_number == cuboids.size() - 1 && position[2] < (radius + cuboids[cuboid_number].height + 0.5f)
			&& position[2] > (-radius + cuboids[cuboid_number].height0)) {
			if (c1p) position[1] -= 0.02f;
			else position[1] += 0.02f;
		}

	}
	//如果不在柱子上方

	else {
		//地面反弹
		if (position[2] < (radius + 0)) {
			bounce(0.81f, 0.81f, -0.5f);  //地面反弹系数
			position[2] = (radius + 0); //防卡死
		}
		//柱侧面反弹
		for (auto it = cuboids.begin(); it != cuboids.end(); ++it) {
			//如果发生侧面碰撞

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

		//如果在圆柱上方
		if (cylinder_number != -1) {
			//上下面反弹
			if (position[2] < (radius + cylinders[cylinder_number].height)
				&& position[2] > (-radius + cylinders[cylinder_number].height0)) {
				bounce(0.4f, 0.4f, -0.6f); //柱面反弹系数
				if (abs(position[2] - (radius + cylinders[cylinder_number].height)) <
					abs(position[2] - (-radius + cylinders[cylinder_number].height0))) {
					position[2] = (radius + cylinders[cylinder_number].height); //防卡死
				}
				else if (abs(position[2] - (radius + cylinders[cylinder_number].height)) >
					abs(position[2] - (-radius + cylinders[cylinder_number].height0))) {
					position[2] = (-radius + cylinders[cylinder_number].height0); //防卡死
					energy = false;
				}
			}
		}
		else {
			for (auto it = cylinders.begin(); it != cylinders.end(); ++it) {
				//如果发生侧面碰撞

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

//反弹
void ball::bounce(float kx, float ky, float kz) {
	speed[0] *= kx; //x方向衰减
	speed[1] *= ky; //y方向衰减
	speed[2] *= kz; //z方向衰减
	energy = true;
}

//弹跳
void ball::jump_z() {
	if (energy) {
		speed[2] = speed_jump;
		energy = false;
	}

}


//画球
void ball::draw() {
	extern GLuint texture[13];
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);					//选择纹理texture[status2]  
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);		//设置纹理不受光照影响  
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