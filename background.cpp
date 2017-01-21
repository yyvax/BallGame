//
// Created by 天才小程峥 on 2016/12/25.
//
#include <iostream>
#include <vector>
#include <GL/glut.h>
#include <Windows.h>
#include "ball.h"

using namespace std;

// x, y, length, height
cuboid left1(-30, 0, 0.5, 60, 40, 0);
cuboid right1(30, 0, 0.5, 60, 40, 0);
cuboid forward1(0, 30, 60, 0.5, 40, 0);
cuboid backward(0, -30, 60, 0.5, 40, 0);
cuboid victory(8, -11, 0.99f, 0.99f, 9.99f, 9.51f);
cuboid c1(-10.0f, -7.0f, 1.0f, 1.0f, 4.0f, 3.5f);
bool c1p;

//Draw_Leg(8, -11, 1, 1, 10, 9.5, 2);

cuboid cubes[100];
cylinder cys[100];
extern GLuint texture[13];
vector<cuboid> cuboids;
vector<cylinder> cylinders;

void texload(int i, char *filename);
void Init() {

	cuboids.push_back(left1);
	cuboids.push_back(right1);
	cuboids.push_back(forward1);
	cuboids.push_back(backward);
	cuboids.push_back(victory);
	cuboids.push_back(c1);
	BITMAPINFOHEADER bitmapInfoHeader;									//bitmap信息头  
	unsigned char*   bitmapData1, *bitmapData2, *bitmapData3;			//纹理数据   

	glGenTextures(13, texture); // 第一参数是需要生成标示符的个数, 第二参数是返回标示符的数组  
	texload(0, "wood.bmp");
	//texload(1, "Brick.bmp");
	texload(1, "Brick.bmp");
	texload(2, "snow.bmp");
	texload(3, "red.bmp");
	texload(4, "sky.bmp");

	//button texture
	texload(10, "Start.bmp");
	texload(11, "Exit.bmp");
	texload(12, "Restart.bmp");
}

void cuboid_Position_Update() {
	if (c1.position[1] > -6.0f) {
		c1p = true;
	}
	if (c1.position[1] < -7.0f) {
		c1p = false;
	}
	if (c1p) c1.position[1] -= 0.02f;
	else c1.position[1] += 0.02f;
	cuboids.pop_back();
	cuboids.push_back(c1);
	//    cout << c1.position[0] << " " << c1.position[1] << endl;
}



