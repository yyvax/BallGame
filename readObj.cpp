#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
//undefine if u want to use the default bending constraint of pbd
#include<sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <stack>
#include "ball.h"
using namespace std;
GLfloat rtx = 0.0f, rty = 0.0f, rtz = 0.0;
GLfloat step = 0.1;
//float exp1 = 1e-3;
const int GRID_SIZE = 10;
GLint viewport[4];
GLdouble PP[16];
int v_num; //记录点的数量
int vn_num;//记录法线的数量
int vt_num;//
int f_num; //记录面的数量

GLfloat **vArr; //存放点的二维数组
GLfloat **vnArr;//存放法线的二维数组
int **fvArr; //存放面顶点的二维数组
int **fnArr; //存放面法线的二维数组
stack<int> face_cube[4];
vector<vector<int>> faces;
vector<vector<int>> faces_cylinder;

int readfile(string addrstr) //将文件内容读到数组中去
{
	int i;
	string s1;
	float f2, f3, f4;
	vArr = new GLfloat*[v_num];
	for (i = 0; i < v_num; i++)
	{
		vArr[i] = new GLfloat[3];
	}
	vnArr = new GLfloat*[vn_num];
	for (i = 0; i<vn_num; i++)
	{
		vnArr[i] = new GLfloat[3];
	}
	fvArr = new int*[f_num];
	fnArr = new int*[f_num];
	for (i = 0; i<f_num; i++)
	{
		fvArr[i] = new int[4];
		fnArr[i] = new int[3];
	}
	ifstream infile(addrstr.c_str());
	string sline;//每一行
	int ii = 0, jj = 0, kk = 0;

	bool ifCube = false;
	bool ifCylinder = false;
	extern vector<cuboid> cuboids;
	extern cuboid cubes[100];
	bool v1 = false;
	bool v2 = false;

	while (getline(infile, sline))
	{
		//g pCube 判断是否是长方体 圆柱
		//g group pasted__pasted__pCube1
		//g pCylinder
		if (sline[0] == 'g') {
			int j;
			for (j = 2; j < sline.length() - 5; ++j) {
				if (sline[j] == 'p' && sline[j + 1] == 'C'
					&& sline[j + 2] == 'u' && sline[j + 3] == 'b'
					&& sline[j + 4] == 'e') {
					ifCube = true;
					cout << sline << endl;
					break;
				}
			}
			for (j = 2; j < sline.length() - 5; ++j) {
				if (sline[j] == 'p' && sline[j + 1] == 'C'
					&& sline[j + 2] == 'y' && sline[j + 3] == 'l'
					&& sline[j + 4] == 'i') {
					ifCylinder = true;
					cout << sline << endl;
					break;
				}
			}			       
			if (v1 && ifCube) {
				face_cube[0].push(0);
				face_cube[1].push(0);
				face_cube[2].push(0);
				face_cube[3].push(0);
				cout << sline << endl;
				ifCube = false;
				v1 = false;
			}
			if (v2 && ifCylinder) {
				vector<int> temp;
				for (int i = 0; i < 4; i++)
					temp.push_back(0);
				faces_cylinder.push_back(temp);
				cout << sline << endl;
				ifCylinder = false;
				cout << ifCylinder << endl;
				v2 = false;
			}
		}
		if (ifCube) v1 = true;
		if (ifCylinder) v2 = true;
		
		if (sline[0] == 'v')
		{
			if (sline[1] == 'n')//vn
			{
				istringstream sin(sline);
				sin >> s1 >> f2 >> f3 >> f4;
				vnArr[ii][0] = f2;
				vnArr[ii][1] = f3;
				vnArr[ii][2] = f4;
				ii++;
			}
			else if (sline[1] == 't') {}
			else//v
			{
				istringstream sin(sline);
				sin >> s1 >> f2 >> f3 >> f4;
				vArr[jj][0] = f2;
				vArr[jj][1] = f3;
				vArr[jj][2] = f4;

				jj++;
			}
		}
		if (sline[0] == 'f') //读取面
		{
			istringstream in(sline);
			GLfloat a;
			in >> s1;//去掉前缀f
			int i, k;
			//点数改这里
			vector<int> temp;
			for (i = 0; i<4; i++)
			{
				in >> s1;
				//				cout<<s1<<endl;
				//                printf("%s\n",s1.c_str());
				//取得顶点索引和法线索引
				a = 0;
				for (k = 0; s1[k] != '/'; k++)
				{
					a = a * 10 + (s1[k] - 48);
				}
				fvArr[kk][i] = a;
				if (ifCube) {
					face_cube[i].push(fvArr[kk][i]);
				}
				if (ifCylinder) {
					temp.push_back(fvArr[kk][i]);
				}
				a = 0;
				for (k = k + 2; s1[k]; k++) //????
				{
					a = a * 10 + (s1[k] - 48);;
				}
				fnArr[kk][i] = a;
			}

			if (ifCylinder) faces_cylinder.push_back(temp);
			kk++;
		}
	}
	return 0;
}

void judgeCylinder() {
	float xMax = 0, yMax = 0, zMax = 0;
	float xMin = 0, yMin = 0, zMin = 0;
	bool first = true;
	int number = 0;
	extern cylinder cys[100];
	extern vector<cylinder> cylinders;
	for (int i = 0; i < faces_cylinder.size(); ++i) {
		if (!faces_cylinder[i][0] && !faces_cylinder[i][1] && !faces_cylinder[i][2] && !faces_cylinder[i][3]) {
			first = true;
			//x y 长 宽 高
			cout << "cylinder:--------"<<(xMax + xMin) / 2.0 << " " << (yMax + yMin) / 2.0 << " " << xMax - xMin << " " <<
				yMax - yMin << " " << zMax - zMin << endl;
			cys[number].init((xMax + xMin) / 2.0f, (yMax + yMin) / 2.0f, (xMax - xMin)/2.0f, zMax - zMin, 0);
			cylinders.push_back(cys[number]);
			cout << cylinders.size() << endl;
			number++;
			i++;
		}
		if (i == faces_cylinder.size()) break;
		int a0, a1, a2, a3;
		a0 = faces_cylinder[i][0];
		a1 = faces_cylinder[i][1];
		a2 = faces_cylinder[i][2];
		a3 = faces_cylinder[i][3];

		float f2, f3, f4;//x z y
		f2 = vArr[a0][0], f4 = vArr[a0][2], f3 = vArr[a0][1];
		if (first) {
			xMax = f2;
			xMin = f2;
			yMax = f4;
			yMin = f4;
			zMax = f3;
			zMin = f3;
			first = false;
		}
		if (f2 > xMax) xMax = f2;
		if (f4 > yMax) yMax = f4;
		if (f3 > zMax) zMax = f3;
		if (f2 < xMin) xMin = f2;
		if (f4 < yMin) yMin = f4;
		if (f3 < zMin) zMin = f3;

	}
}

void judgeCube() {


	float xMax = 0, yMax = 0, zMax = 0;
	float xMin = 0, yMin = 0, zMin = 0;
	// glColor3f(1,1,1);
	//glBegin(GL_LINES);
	extern cuboid cubes[100];
	int number = 0;
	bool first = true;
	extern vector<cuboid> cuboids;

	while (!face_cube->empty()) {
		int a = face_cube[0].top();
		int b = face_cube[1].top();
		int c = face_cube[2].top();
		int d = face_cube[3].top();
		vector<int> temp;
		temp.push_back(a);
		temp.push_back(b);
		temp.push_back(c);
		temp.push_back(d);
		faces.push_back(temp);
		face_cube[0].pop();
		face_cube[1].pop();
		face_cube[2].pop();
		face_cube[3].pop();
	}
	for (int i = 0; i < faces.size(); ++i) {
		if (!faces[i][0] && !faces[i][1] && !faces[i][2] && !faces[i][3]) {
			//            cout << faces[i][0] << " " << faces[i][1] << " " << faces[i][2] << endl;
			//glEnd();
			//glBegin(GL_LINES);
			first = true;
			//x y 长 宽 高
			cout << (xMax + xMin) / 2.0 << " " << (yMax + yMin) / 2.0 << " " << xMax - xMin << " " <<
				yMax - yMin << " " << zMax - zMin << endl;
			cubes[number].cuboid_init((xMax + xMin) / 2.0f, (yMax + yMin) / 2.0f, xMax - xMin, yMax - yMin, zMax - zMin,
				0);
			cuboids.push_back(cubes[number]);
			if (number == 3) {
				cout << cuboids[number].chang << " " << cuboids[number].kuan << endl;
				cuboids.pop_back();
			}
			cout << cuboids.size() << endl;
			number++;
			i++;
		}
		if (i == faces.size()) break;
		int a0, a1, a2, a3;
		//        a0 = fvArr[faces[i][0]][0] - 1;
		//        a1 = fvArr[faces[i][1]][1] - 1;
		//        a2 = fvArr[faces[i][2]][2] - 1;
		//        //点数改这里
		//        a3 = fvArr[faces[i][3]][3] - 1;
		a0 = faces[i][0];
		a1 = faces[i][1];
		a2 = faces[i][2];
		a3 = faces[i][3];

		float f2, f3, f4;//x z y
		f2 = vArr[a0][0], f4 = vArr[a0][2], f3 = vArr[a0][1];
		if (first) {
			xMax = f2;
			xMin = f2;
			yMax = f4;
			yMin = f4;
			zMax = f3;
			zMin = f3;
			first = false;
		}
		if (f2 > xMax) xMax = f2;
		if (f4 > yMax) yMax = f4;
		if (f3 > zMax) zMax = f3;
		if (f2 < xMin) xMin = f2;
		if (f4 < yMin) yMin = f4;
		if (f3 < zMin) zMin = f3;

		//        f2 = vArr[a1][0], f4 = vArr[a1][2], f3 = vArr[a1][1];
		if (f2 > xMax) xMax = f2;
		if (f4 > yMax) yMax = f4;
		if (f3 > zMax) zMax = f3;
		if (f2 < xMin) xMin = f2;
		if (f4 < yMin) yMin = f4;
		if (f3 < zMin) zMin = f3;
		//        f2 = vArr[a2][0], f4 = vArr[a2][2], f3 = vArr[a2][1];
		if (f2 > xMax) xMax = f2;
		if (f4 > yMax) yMax = f4;
		if (f3 > zMax) zMax = f3;
		if (f2 < xMin) xMin = f2;
		if (f4 < yMin) yMin = f4;
		if (f3 < zMin) zMin = f3;
		f2 = vArr[a3][0], f4 = vArr[a3][2], f3 = vArr[a3][1];
		if (f2 > xMax) xMax = f2;
		if (f4 > yMax) yMax = f4;
		if (f3 > zMax) zMax = f3;
		if (f2 < xMin) xMin = f2;
		if (f4 < yMin) yMin = f4;
		if (f3 < zMin) zMin = f3;
	}
}

void getLineNum(string addrstr) {   //获取点数以及面数
									//	cout<<addrstr.c_str()<<endl;
	v_num = vn_num = f_num = vt_num = 0;
	ifstream infile(addrstr.c_str());
	string sline;//每一行
	while (getline(infile, sline)) {


		if (sline[0] == 'v') {
			if (sline[1] == 'n') {
				vn_num++;
			}
			else if (sline[1] == 't') { //vt
				vt_num++;
			}
			else {
				v_num++;
			}
		}
		else {
			if (sline[0] == 'f') {
				f_num++;
			}
		}
	}
}

string s1;
GLfloat f2, f3, f4;

void draw(void)
{
	//    cout<<f_num;
	//    glTranslatef(0,0,-100);
	glPushMatrix();
	//    glRotatef(90,1,0,0);
	glBegin(GL_LINES);
	glColor3f(1, 1, 1);
	glLineWidth(0.0001f);
	for (int i = 0; i < f_num; i++) {
		int a0, a1, a2, a3;
		a0 = fvArr[i][0] - 1;
		a1 = fvArr[i][1] - 1;
		a2 = fvArr[i][2] - 1;
		//点数改这里
		a3 = fvArr[i][3] - 1;
		//        glVertex3f(vArr[a0][0],vArr[a0][1],vArr[a0][2]);
		//        glVertex3f(vArr[a1][0],vArr[a1][1],vArr[a1][2]);
		//        glVertex3f(vArr[a1][0],vArr[a1][1],vArr[a1][2]);
		//        glVertex3f(vArr[a2][0],vArr[a2][1],vArr[a2][2]);
		//        glVertex3f(vArr[a2][0],vArr[a2][1],vArr[a2][2]);
		//        //点数改这里
		//        glVertex3f(vArr[a3][0],vArr[a3][1],vArr[a3][2]);
		//        glVertex3f(vArr[a3][0],vArr[a3][1],vArr[a3][2]);
		//        glVertex3f(vArr[a0][0],vArr[a0][1],vArr[a0][2]);

		//将 yz反一下
		glVertex3f(vArr[a0][0], vArr[a0][2], vArr[a0][1]);
		glVertex3f(vArr[a1][0], vArr[a1][2], vArr[a1][1]);
		glVertex3f(vArr[a1][0], vArr[a1][2], vArr[a1][1]);
		glVertex3f(vArr[a2][0], vArr[a2][2], vArr[a2][1]);
		glVertex3f(vArr[a2][0], vArr[a2][2], vArr[a2][1]);
		//点数改这里
		glVertex3f(vArr[a3][0], vArr[a3][2], vArr[a3][1]);
		glVertex3f(vArr[a3][0], vArr[a3][2], vArr[a3][1]);
		glVertex3f(vArr[a0][0], vArr[a0][2], vArr[a0][1]);
	}
	glPopMatrix();
	glEnd();
	//judgeCube();
}
