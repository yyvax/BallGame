#include <stdlib.h>  
#include <windows.h>  
#include <stdio.h>  
#include <GL\glut.h>  

#define BITMAP_ID 0x4D42  
#define Height 16  
#define Width 16 

extern int status;							//�����л���ʾģʽ�������±�
extern GLuint texture[13];

//������ͼƬ  
unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr;									//�ļ�ָ��  
	BITMAPFILEHEADER bitmapFileHeader;				//bitmap�ļ�ͷ  
	unsigned char *bitmapImage;						//bitmapͼ������  
	int imageIdx = 0;								//ͼ��λ������  
	unsigned char tempRGB;							//��������  

	//filePtr = fopen(filename, "rb");				//�ԡ�������+����ģʽ���ļ�filename
													//��֤�Ƿ��ܴ��ļ�
	fopen_s(&filePtr, filename, "rb");
	if (filePtr == NULL) {
		printf("file not open\n");
		return NULL;
	}

	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);						//����bitmap�ļ�ͼ

																						//��֤�Ƿ�Ϊbitmap�ļ�
	if (bitmapFileHeader.bfType != BITMAP_ID) {
		fprintf(stderr, "Error in LoadBitmapFile: the file is not a bitmap file\n");
		return NULL;
	}

	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);						//����bitmap��Ϣͷ
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);								//���ļ�ָ������bitmap����
	bitmapImage = new unsigned char[bitmapInfoHeader->biSizeImage];						//Ϊװ��ͼ�����ݴ����㹻���ڴ�  

																						//��֤�ڴ��Ƿ񴴽��ɹ�  
	if (!bitmapImage) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);						//����bitmapͼ������

																						//ȷ�϶���ɹ�  
	if (bitmapImage == NULL) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	//����bitmap�б���ĸ�ʽ��BGR�����潻��R��B��ֵ���õ�RGB��ʽ  
	for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3) {
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}

	// �ر�bitmapͼ���ļ�  
	fclose(filePtr);
	return bitmapImage;
}



//��������ĺ���  
void texload(int i, char *filename)
{
	BITMAPINFOHEADER bitmapInfoHeader;										//bitmap��Ϣͷ  
	unsigned char*   bitmapData;											//��������  

	bitmapData = LoadBitmapFile(filename, &bitmapInfoHeader);
	glBindTexture(GL_TEXTURE_2D, texture[i]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);		//ָ����ǰ����ķŴ�/��С���˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D,
		0,								//mipmap���(ͨ��Ϊ����ʾ���ϲ�)   
		GL_RGB,							//����ϣ���������к졢�̡�������  
		bitmapInfoHeader.biWidth,		//��������������n�����б߿�+2   
		bitmapInfoHeader.biHeight,		//����߶ȣ�������n�����б߿�+2   
		0,								//�߿�(0=�ޱ߿�, 1=�б߿�)   
		GL_RGB,							//bitmap���ݵĸ�ʽ  
		GL_UNSIGNED_BYTE,				//ÿ����ɫ���ݵ�����  
		bitmapData);					//bitmap����ָ��    
}

void drawCube()
{
	int i, j;
	const GLfloat x1 = -0.5, x2 = 0.5;
	const GLfloat y1 = -0.5, y2 = 0.5;
	const GLfloat z1 = -0.5, z2 = 0.5;

	//ָ����������ĸ����㣬ÿ��������3������ֵ��ʾ    
	GLfloat point[6][4][3] = {
		{ { x1,y1,z1 },{ x2,y1,z1 },{ x2,y2,z1 },{ x1,y2,z1 } },
		{ { x1,y1,z1 },{ x2,y1,z1 },{ x2,y1,z2 },{ x1,y1,z2 } },
		{ { x2,y1,z1 },{ x2,y2,z1 },{ x2,y2,z2 },{ x2,y1,z2 } },
		{ { x1,y1,z1 },{ x1,y2,z1 },{ x1,y2,z2 },{ x1,y1,z2 } },
		{ { x1,y2,z1 },{ x2,y2,z1 },{ x2,y2,z2 },{ x1,y2,z2 } },
		{ { x1,y1,z2 },{ x2,y1,z2 },{ x2,y2,z2 },{ x1,y2,z2 } }
	};
	int dir[4][2] = { { 1,1 },{ 1,0 },{ 0,0 },{ 0,1 } };
	//���������λ���ģʽ    

	glBegin(GL_QUADS);
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 4; j++) {
			glTexCoord2iv(dir[j]);
			glVertex3fv(point[i][j]);
		}
	}
	glEnd();
}
