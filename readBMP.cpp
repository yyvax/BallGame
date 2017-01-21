#include <stdlib.h>  
#include <windows.h>  
#include <stdio.h>  
#include <GL\glut.h>  

#define BITMAP_ID 0x4D42  
#define Height 16  
#define Width 16 

extern int status;							//用于切换显示模式的两个下标
extern GLuint texture[13];

//读纹理图片  
unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr;									//文件指针  
	BITMAPFILEHEADER bitmapFileHeader;				//bitmap文件头  
	unsigned char *bitmapImage;						//bitmap图像数据  
	int imageIdx = 0;								//图像位置索引  
	unsigned char tempRGB;							//交换变量  

	//filePtr = fopen(filename, "rb");				//以“二进制+读”模式打开文件filename
													//验证是否能打开文件
	fopen_s(&filePtr, filename, "rb");
	if (filePtr == NULL) {
		printf("file not open\n");
		return NULL;
	}

	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);						//读入bitmap文件图

																						//验证是否为bitmap文件
	if (bitmapFileHeader.bfType != BITMAP_ID) {
		fprintf(stderr, "Error in LoadBitmapFile: the file is not a bitmap file\n");
		return NULL;
	}

	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);						//读入bitmap信息头
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);								//将文件指针移至bitmap数据
	bitmapImage = new unsigned char[bitmapInfoHeader->biSizeImage];						//为装载图像数据创建足够的内存  

																						//验证内存是否创建成功  
	if (!bitmapImage) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);						//读入bitmap图像数据

																						//确认读入成功  
	if (bitmapImage == NULL) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	//由于bitmap中保存的格式是BGR，下面交换R和B的值，得到RGB格式  
	for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3) {
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}

	// 关闭bitmap图像文件  
	fclose(filePtr);
	return bitmapImage;
}



//加载纹理的函数  
void texload(int i, char *filename)
{
	BITMAPINFOHEADER bitmapInfoHeader;										//bitmap信息头  
	unsigned char*   bitmapData;											//纹理数据  

	bitmapData = LoadBitmapFile(filename, &bitmapInfoHeader);
	glBindTexture(GL_TEXTURE_2D, texture[i]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);		//指定当前纹理的放大/缩小过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D,
		0,								//mipmap层次(通常为，表示最上层)   
		GL_RGB,							//我们希望该纹理有红、绿、蓝数据  
		bitmapInfoHeader.biWidth,		//纹理宽带，必须是n，若有边框+2   
		bitmapInfoHeader.biHeight,		//纹理高度，必须是n，若有边框+2   
		0,								//边框(0=无边框, 1=有边框)   
		GL_RGB,							//bitmap数据的格式  
		GL_UNSIGNED_BYTE,				//每个颜色数据的类型  
		bitmapData);					//bitmap数据指针    
}

void drawCube()
{
	int i, j;
	const GLfloat x1 = -0.5, x2 = 0.5;
	const GLfloat y1 = -0.5, y2 = 0.5;
	const GLfloat z1 = -0.5, z2 = 0.5;

	//指定六个面的四个顶点，每个顶点用3个坐标值表示    
	GLfloat point[6][4][3] = {
		{ { x1,y1,z1 },{ x2,y1,z1 },{ x2,y2,z1 },{ x1,y2,z1 } },
		{ { x1,y1,z1 },{ x2,y1,z1 },{ x2,y1,z2 },{ x1,y1,z2 } },
		{ { x2,y1,z1 },{ x2,y2,z1 },{ x2,y2,z2 },{ x2,y1,z2 } },
		{ { x1,y1,z1 },{ x1,y2,z1 },{ x1,y2,z2 },{ x1,y1,z2 } },
		{ { x1,y2,z1 },{ x2,y2,z1 },{ x2,y2,z2 },{ x1,y2,z2 } },
		{ { x1,y1,z2 },{ x2,y1,z2 },{ x2,y2,z2 },{ x1,y2,z2 } }
	};
	int dir[4][2] = { { 1,1 },{ 1,0 },{ 0,0 },{ 0,1 } };
	//设置正方形绘制模式    

	glBegin(GL_QUADS);
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 4; j++) {
			glTexCoord2iv(dir[j]);
			glVertex3fv(point[i][j]);
		}
	}
	glEnd();
}
