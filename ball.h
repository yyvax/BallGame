//
// Created by 天才小程峥 on 2016/12/23.
//

#ifndef OBJ_BALL_H
#define OBJ_BALL_H

#define LEFT 1
#define RIGHT 2
#define DOWN 3
#define UP 4

class ball {
private:
	float radius;                //半径
	int cuboid_number;         //在那个球柱上方 若在空地上则为-1
	int cylinder_number;
public:
	float accelerated_speed[3];  //xyz三个方向的加速度
	float position[3];           //当前位置
	float speed[3];              //xyz三个方向的速度
	ball();
	void update_Position();                     //更新位置
	void bounce(float kx, float ky, float kz);  //以一定速度反弹
	void draw();                                //画小球
	void jump_z();                              //跳跃
	void move(int direction);                   //移动
	void judge();                               //判断小球是否在fang柱上方
	void judge2();                              //判断小球是否在球柱上方
	bool energy;                                //没能量时无法起跳 每次落地恢复能量
};

class cuboid {
public:
	float position[2];
	float chang;
	float kuan;
	float height; //顶面高
	float height0;//底面高
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
