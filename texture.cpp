#include <cstdio>
#include <GL/glut.h>
#include <cstdlib>


GLuint texture[10];


//#pragma pack(1)
class BMP
{
public:
	//14字节
	unsigned short    bfType;            //文件标识 2字节 必须为BM
	unsigned int      bfSize;                //文件大小 4字节
	unsigned short    bfReserved1;        //保留，每字节以"00"填写 2字节
	unsigned short    bfReserved2;        //同上 2字节
	unsigned int      bfOffBits;            //记录图像数据区的起始位置(图象数据相对于文件头字节的偏移量)。 4字节

											//40字节
	unsigned int  biSize;                //表示本结构的大小 4字节
	int    biWidth;                //位图的宽度  4字节
	int    biHeight;               //位图的高度  4字节
	unsigned short   biPlanes;            //永远为1 ,   2字节
	unsigned short   biBitCount;        //位图的位数  分为1 4 8 16 24 32   2字节
	unsigned int  biCompression;       //压缩说明   4字节
	unsigned int  biSizeImage;            //表示位图数据区域的大小以字节为单位  4字节
	int   biXPelsPerMeter;                //用象素/米表示的水平分辨率   4字节
	int   biYPelsPerMeter;                //用象素/米表示的垂直分辨率   4字节
	unsigned int  biClrUsed;            //位图使用的颜色索引数   4字节
	unsigned int  biClrImportant;        //对图象显示有重要影响的颜色索引的数目  4字节
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
	FILE* file = fopen(filename, "r");   //打开一个文件进行读写操作。
	if (file == NULL)
	{
		printf("can't read file %s! ", filename);
		return NULL;
	}

	printf("bmp size : %lu", sizeof(BMP));

	BMPInfo* bmpInfo = new BMPInfo();
	BMP bmp;

	//读取图像信息
	fseek(file, 0L, 0); //读取图像文件类型
	fread(&bmp, sizeof(BMP), 1, file);
	bmpInfo->imagedata = (unsigned char*)malloc(bmp.biSizeImage);
	fseek(file, (long)bmp.bfOffBits, 0);
	fread(bmpInfo->imagedata, sizeof(unsigned char), bmp.biSizeImage, file);
	fclose(file);

	bmpInfo->bmp = bmp;

	return bmpInfo;
}

void LoadGLTextures(int i, char *a)                               // 载入位图(调用上面的代码)并转换成纹理
{
	BMPInfo *TextureImage[1];                   // 创建纹理的存储空间
	TextureImage[0] = image_info(a);

	// 使用来自位图数据生成 的典型纹理
	glBindTexture(GL_TEXTURE_2D, texture[i]);
	//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TextureImage[i]->bmp.biWidth,
	// TextureImage[i]->bmp.biHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, TextureImage[i]->imagedata);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 	    //mipmap层次(通常为，表示最上层)
		GL_RGB,	//我们希望该纹理有红、绿、蓝数据
		TextureImage[0]->bmp.biWidth, //纹理宽带，必须是n，若有边框+2
		TextureImage[0]->bmp.biHeight, //纹理高度，必须是n，若有边框+2
		0, //边框(0=无边框, 1=有边框)
		GL_RGB,	//bitmap数据的格式
		GL_UNSIGNED_BYTE, //每个颜色数据的类型
		TextureImage[0]->imagedata);
	if (TextureImage[0])                            // 纹理是否存在
	{
		delete TextureImage[0];
	}
}