//
// Created by ���С��� on 2016/12/23.
//

#ifndef OBJ_BALL_H
#define OBJ_BALL_H

#define LEFT 1
#define RIGHT 2
#define DOWN 3
#define UP 4

class ball {
private:
	float radius;                //�뾶
	int cuboid_number;         //���Ǹ������Ϸ� ���ڿյ�����Ϊ-1
	int cylinder_number;
public:
	float accelerated_speed[3];  //xyz��������ļ��ٶ�
	float position[3];           //��ǰλ��
	float speed[3];              //xyz����������ٶ�
	ball();
	void update_Position();                     //����λ��
	void bounce(float kx, float ky, float kz);  //��һ���ٶȷ���
	void draw();                                //��С��
	void jump_z();                              //��Ծ
	void move(int direction);                   //�ƶ�
	void judge();                               //�ж�С���Ƿ���fang���Ϸ�
	void judge2();                              //�ж�С���Ƿ��������Ϸ�
	bool energy;                                //û����ʱ�޷����� ÿ����ػָ�����
};

class cuboid {
public:
	float position[2];
	float chang;
	float kuan;
	float height; //�����
	float height0;//�����
	cuboid(float x, float y, float chang, float kuan, float height, float height0) {
		position[0] = x;
		position[1] = y;
		this->chang = chang;
		this->kuan = kuan;
		this->height = height;
		this->height0 = height0;
	}
	cuboid() {}

	void cuboid_init(float x, float y, float chang, float kuan, float height, float height0) {
		position[0] = x;
		position[1] = y;
		this->chang = chang;
		this->kuan = kuan;
		this->height = height;
		this->height0 = height0;
	}
};

class cylinder {
public:
	float position[2];
	float r;
	float height0;
	float height;
	void init(float x, float y, float r, float height, float height0) {
		position[0] = x;
		position[1] = y;
		this->r = r;
		this->height = height;
		this->height0 = height0;
	}

};

#endif //OBJ_BALL_H
