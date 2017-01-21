#include <cstdio>
#include <GL/glut.h>
#include <cstdlib>


GLuint texture[10];


//#pragma pack(1)
class BMP
{
public:
	//14�ֽ�
	unsigned short    bfType;            //�ļ���ʶ 2�ֽ� ����ΪBM
	unsigned int      bfSize;                //�ļ���С 4�ֽ�
	unsigned short    bfReserved1;        //������ÿ�ֽ���"00"��д 2�ֽ�
	unsigned short    bfReserved2;        //ͬ�� 2�ֽ�
	unsigned int      bfOffBits;            //��¼ͼ������������ʼλ��(ͼ������������ļ�ͷ�ֽڵ�ƫ����)�� 4�ֽ�

											//40�ֽ�
	unsigned int  biSize;                //��ʾ���ṹ�Ĵ�С 4�ֽ�
	int    biWidth;                //λͼ�Ŀ��  4�ֽ�
	int    biHeight;               //λͼ�ĸ߶�  4�ֽ�
	unsigned short   biPlanes;            //��ԶΪ1 ,   2�ֽ�
	unsigned short   biBitCount;        //λͼ��λ��  ��Ϊ1 4 8 16 24 32   2�ֽ�
	unsigned int  biCompression;       //ѹ��˵��   4�ֽ�
	unsigned int  biSizeImage;            //��ʾλͼ��������Ĵ�С���ֽ�Ϊ��λ  4�ֽ�
	int   biXPelsPerMeter;                //������/�ױ�ʾ��ˮƽ�ֱ���   4�ֽ�
	int   biYPelsPerMeter;                //������/�ױ�ʾ�Ĵ�ֱ�ֱ���   4�ֽ�
	unsigned int  biClrUsed;            //λͼʹ�õ���ɫ������   4�ֽ�
	unsigned int  biClrImportant;        //��ͼ����ʾ����ҪӰ�����ɫ��������Ŀ  4�ֽ�
};
//#pragma pack(0)

class BMPInfo
{
public:
	BMP bmp;
	unsigned char* imagedata;
	~BMPInfo()
	{
		delete imagedata;
	}
};

BMPInfo* image_info(char *filename)
{
	FILE* file = fopen(filename, "r");   //��һ���ļ����ж�д������
	if (file == NULL)
	{
		printf("can't read file %s! ", filename);
		return NULL;
	}

	printf("bmp size : %lu", sizeof(BMP));

	BMPInfo* bmpInfo = new BMPInfo();
	BMP bmp;

	//��ȡͼ����Ϣ
	fseek(file, 0L, 0); //��ȡͼ���ļ�����
	fread(&bmp, sizeof(BMP), 1, file);
	bmpInfo->imagedata = (unsigned char*)malloc(bmp.biSizeImage);
	fseek(file, (long)bmp.bfOffBits, 0);
	fread(bmpInfo->imagedata, sizeof(unsigned char), bmp.biSizeImage, file);
	fclose(file);

	bmpInfo->bmp = bmp;

	return bmpInfo;
}

void LoadGLTextures(int i, char *a)                               // ����λͼ(��������Ĵ���)��ת��������
{
	BMPInfo *TextureImage[1];                   // ��������Ĵ洢�ռ�
	TextureImage[0] = image_info(a);

	// ʹ������λͼ�������� �ĵ�������
	glBindTexture(GL_TEXTURE_2D, texture[i]);
	//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TextureImage[i]->bmp.biWidth,
	// TextureImage[i]->bmp.biHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, TextureImage[i]->imagedata);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 	    //mipmap���(ͨ��Ϊ����ʾ���ϲ�)
		GL_RGB,	//����ϣ���������к졢�̡�������
		TextureImage[0]->bmp.biWidth, //��������������n�����б߿�+2
		TextureImage[0]->bmp.biHeight, //����߶ȣ�������n�����б߿�+2
		0, //�߿�(0=�ޱ߿�, 1=�б߿�)
		GL_RGB,	//bitmap���ݵĸ�ʽ
		GL_UNSIGNED_BYTE, //ÿ����ɫ���ݵ�����
		TextureImage[0]->imagedata);
	if (TextureImage[0])                            // �����Ƿ����
	{
		delete TextureImage[0];
	}
}