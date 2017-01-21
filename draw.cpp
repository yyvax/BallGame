#include <cmath>
#include <GL/glut.h>

#define PI 3.1415926
extern GLuint texture[13];
GLfloat mat_diffuse[] = { 0.85f, 0.65f, 0.6f, 1.0f };
GLfloat mat_ambient[] = { 1.0f, 0.8f, 0.8f, 1.0f };
GLfloat mat_specular[] = { 0.4f,0.6f,0.8f,1.0f };

void drawCube();
void Draw_Leg(float x, float y, float chang, float kuan, float height, float height0, int s)
{
	glPushMatrix();
	glTranslatef(x, y, height0);
	glScalef(chang + 0.05f, kuan + 0.05f, height - height0 + 0.05f);
	glTranslatef(0, 0, 0.5f);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[s]);					//选择纹理texture[status2]  
																//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,  GL_MODULATE);		//设置纹理受光照影响  
	drawCube();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void Draw_Cylinder(float x, float y, float r, float height, float height0) {
	GLUquadricObj *quadricObj = gluNewQuadric();
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTranslatef(x, y, height0);
	gluQuadricTexture(quadricObj, GL_TRUE);
	gluCylinder(quadricObj, r, r, height - height0, 30, 30);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	double n = 50;//分段数
	int i;
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(x,y,height);
	for (i = 0; i <= n; i++)
		glVertex3f(x+r*cos(2 * PI / n*i), y+r*sin(2 * PI / n*i), height);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}