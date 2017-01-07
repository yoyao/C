
#define _CRT_SECURE_NO_WARNINGS

#pragma disable(warning:4996)

#pragma region 这是网上大神写的  玛德，原来是可以折叠的，还是个预编译指令

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
	size_t width;
	size_t height;
	unsigned char *data;
} Image;

static Image* image_new (size_t width,size_t height)
{   
	Image *image;

	image = (Image*)malloc (sizeof *image);
	image->width = width;
	image->height = height;
	image->data = (unsigned char*)malloc (width * height);

	return image;
}

static void image_free (Image *image)
{   
	free (image->data);
	free (image);
}

static void image_fill (Image *image,unsigned char value)
{   
	memset (image->data, value, image->width * image->height);//将整个数据区域填满传过来的value值
}



/*
image_set_pixel:
Sets a pixel passed in signed (x, y) coordinates, where (0,0) is at
the center of the image.
*/

//设置图片的像素值
/*
有一个公式为：象素=分辨率*尺寸。 分辨率是单位尺寸长度内的象素数目，单位是（象素/英寸）例：72象素/英寸，就是常说的分辨率为72。尺寸的单位可以是毫米，厘米，分米等等。
公式中的尺寸是图片的尺寸大小，而象素是图片占用磁盘空间的大小。一个图片的象素越大占用的磁盘空间就越大。
分辨率越高，图片越清晰，当尺寸不变的情况，根据公式，分辨率越高，象素越高，图象处占用的磁盘空间越多。
*/
static void image_set_pixel (Image *image,size_t x,size_t y,unsigned char value)
{
	size_t tx, ty;

	unsigned char *p;
	     
	/*重点 很巧妙*/tx = (image->width / 2) + x; //宽度除以2是正中间的位置 加上x（x值可能是负数，是负数的时候即可达到左右圆形内都铺满的效果） 即可得出其在圆内的坐标
	/*重点 很巧妙*/ty = (image->height / 2) + y;//同X轴理

	p = image->data + (ty * image->width) + tx;//得到像素的位置
	/*
   ****	其实所谓的图片可以想象成是一个个点组成的矩阵 得到每个位置 然后设置不同的颜色 在显示的时候自然就是那个形状
	*/

	*p = value;//由指针间接赋值像素值 即将想要设置的像素位置设为value值
}





/*
画图函数共有6个循环:
1.-r<x<r,-r<y<r(画个大圆<白色>);
2.0<x<r,-r<y<0(在第一象限填充黑色(0);
3.-1/2r<x<0,-r<y<0(在圆心上面画一个中圆<半圆>,黑色);
4.1/2r<x<r,0<y<r(大圆心下面的黑色部分);
5.-1/6r<x<1/6r,1/3r<y<2/3r(大圆心下面的小圆<黑色>)
6.-1/6r<x<1/6r,-1/3r<y<-2/3r(大圆心上面的小圆<白色>)
*/
static void draw_Taijitu(Image *image,int radius,int value)
{
	int x,y;

	int rlimit ,llimit;

	int radius_2 = radius*radius;//半径的平方 r²

	for(y = -radius;y<radius;y++)//(画个大圆<白色>)  沿着Y轴画一个直径的长度 
		                         //以象限来看 是从y轴的对称负数方向开始画，从数值上看是从-300，-299开始 但是因为平方，所以数值不会有影响
	{
		for(x= -radius;x<radius;x++)//每个循环沿着X轴画了一个直径的长度
		{
			//  x²+ y²      r²
			if(x*x+y*y <= radius_2)//圆的标准方程 圆心的坐标（a,b） 半径r 所以 (x-a)²+ (y-b)²=r²
			{
				image_set_pixel(image,x,y,0xff);//每个像素画一个255（白色）
			}
		}
	}



	for(y = -radius;y<0;y++)//第一象限画了四分之一的黑色 第一象限开始画半径长度 之所以y = -radius 是因为从数值上看这样是从299,298开始的 因为是平方 所以(x*x)+(y*y)不会有影响
	{
		for(x = 0;x<radius;x++)//每个循环沿着X轴画了一个半径的长度
		{       //  x²+ y²      r²
			if((x*x)+(y*y) <= radius_2)//只要x²+ y² <=r² 就是说明这个点在圆内或者搭到圆边
			{
				image_set_pixel(image,x,y,value);
			}
		}
	}



	for(y = -radius;y<0;y++)//把第一象限的y轴变成了圆形 画半径长度
	{                                    //y²
		double d_tmp =(double)(-radius*y-y*y);
		                                    
		for(x = -(int)sqrt(d_tmp);x<0;x++)
		{
			image_set_pixel(image,x,y,value);
		}
	}





	for(y = 0;y<radius;y++)//把第三象限的y轴变成了圆形 至此黑白八卦成型，独缺上下两个小圆
	{
		llimit = (int)sqrt((double)(radius*y - y*y));

		rlimit = (int)sqrt((double)(radius_2 - y*y));

		for(x = llimit;x<rlimit;x++)
		{
			image_set_pixel(image,x,y,value);
		}
	}





	for(y = 2*radius/6;y<4*radius/6;y++)//画出下面的黑色小圆
	{
		rlimit =(int) sqrt((double)(radius*y-y*y-2*radius_2/9));
		llimit = -rlimit;

		for(x = llimit;x<rlimit;x++)
		{
			image_set_pixel(image,x,y,value);
		}
	}




	for(y = -4*radius/6;y<-2*radius/6;y++)//画出上面的白色小圆 至此全部完成
	{
		rlimit = sqrt(double(-radius*y-y*y-2*radius_2/9));
		llimit = -rlimit;

		for(x = llimit;x<rlimit;x++)
		{
			image_set_pixel(image,x,y,0xff);
		}
	}


	return ;

}


/*
pgm格式图片存储分为两个部分 头部分和数据部分

文件头包括的信息依次是:

1.PGM文件的格式类型(是P2还是P5);

2.图像的宽度;（分割符）

3.图像的高度;（分割符）

4.图像灰度值可能的最大值;（分割符）

文件头的这四部分信息都是以ASCII码形式存储的,所以可以直接在将P2或P5格式的PGM文件在记事本中打开看到文件头的信息.

② PGM文件包括的是灰度级数据,0值代表黑色,最大值代表白色.



在P2或P5类型的PGM文件头的4个信息之间用分割符分开,PGM的合法分隔符包括:空格,TAB,回车符,换行符.
PGM文件头的信息应该由合法分割符号分开,
如上面两幅图所展.文件头的第4个信息,图像灰度值可能的最大值标明了文件数据部分可能出现的像素灰度值的最大值.上面两幅图都是指定的255,所以在数据区的像素数据取值范围在0到255.

数据部分

数据部分记录图像每个像素的灰度值,按照图像从上到下,从左到右的顺序依次存储每个像素的灰度值.对于像素灰度值的表示P2格式和P5格式有所不同.

P5格式

P5格式的文件,每个像素用可以用二进制表示.比如有一幅P5格式图像,灰度值可能的最大值为255,它的第一行第一列像素值为100,那么该图像每个像素使用一个字节表示,
第一行第一列为数值为100的二进制一个字节表示.
如果这副图灰度值可能的最大值是65535,那么它的第一行第一列为数值为100的二进制两个字节表示(因为表示到65535需要两个字节).
每个像素数据之间没有间隔的连续存储,图像一行信息结束后从下一行第一列继续,两行图像数据之间也没有间隔的连续存储,直到将图像的所有信息表示完.因为是以二进制表示,
所以数据部分在记事本中打开后看到的将会是乱码.

P2格式

P2格式的文件,每个像素使用字符串来表示,比如一副P2格式图像,灰度值可能的最大值为255,它的第一行第一列像素值为100,那么该图像图像每个像素使用3个ASCII字符表示,
第一行第一列数据为ASII表示的"100".不同于P5格式,每个像素数据之间需要用一个空格符分开存储,在图像的每一行数据结束时需要换行.还有一点需要注意,P2格式文件数据部分当数据超过70个字节的时候,
会自动换行.也就是说图像的每一行数据末尾,或者存储时超过70个字节时,需要进行换行.
如下图所示,这是一个P2格式的PGM文件在Visual Studio中查看的效果.我们可以看到,每超过70个自己时候需要换行,在图像数据一行结束时候也需要换行(在0 1 1 1结束后,图像第一行数据结束换行).
*/
static void image_save (const Image *image,const char *filename)
{
	FILE *out;
	out = fopen (filename, "wb");
	if (!out)
	{
		return;
	}
	fprintf (out, "P5\n");//输出文件格式 P2 或者 P5 结尾使用分割符（回车）分开

	//%zu用来输出size_t 类型
	fprintf (out, "%zu %zu\n", image->width, image->height);

	fprintf (out, "255\n");//灰度的最大值1

	fwrite (image->data, 1, image->width * image->height, out);//输出pgm格式的数据部分 

	fclose (out);
}



int main_taiji (int argc, char *argv[])
{
	Image *image;

	image = image_new (800, 800);

	image_fill (image, 0xaa);//先将图片的数据区域填满背景色(灰色)
	draw_Taijitu (image, 300, 0);//填满背景色(灰色)后再在上面画太极图 value值是0（黑色）
	image_save (image, "taiji_6.pgm");//保存图片
	image_free (image);

	return 0;
}
#pragma endregion


#pragma region 自己写的画太极图的程序



/*
  *目的是画出一个八卦形状
  */
 #include <stdio.h>
 #include <math.h>
 #include <stdlib.h>
 #include <string.h>
 
 struct MyImage{
     size_t Width;//宽度
     size_t Height;//高度
     char* P_Data;//数据区
 };
 
 struct MyImage* CreatImage(const size_t width,const size_t height)
 {   
     struct MyImage* p_img = (struct MyImage*)malloc(sizeof(struct MyImage));
     p_img->Width=width;
     p_img->Height=height;
     p_img->P_Data =(char*)malloc(width*height);
     return p_img;
 }
 
 void FillImage(struct MyImage* pimg,unsigned char value)
 {   
     memset(pimg->P_Data,value,pimg->Width*pimg->Height);
 }
 
 
 void DestoryImage(struct MyImage* pimg)
 {   
     free(pimg->P_Data);
     pimg->P_Data = NULL;
     free(pimg);
     pimg=NULL;
 
 }
 void SetImgPixel(struct MyImage* pimg,size_t posiX,size_t posiY,unsigned char val)
{   
    size_t ui_x = (pimg->Width/2)+posiX; 
    size_t ui_y = (pimg->Height/2)+posiY;
    char* p_c;
    
    p_c=pimg->P_Data+(pimg->Width*ui_y)+ui_x;
    *p_c=val;
}
void DrawImage(struct MyImage* pimg,const int radius)
{
    int x,y;
    size_t square_radius=radius*radius;//半径的平方

    for(x=-radius;x<radius;x++)//画个大白圆
    {
        for(y=-radius;y<radius;y++)
        {
            if((x*x+y*y)<=square_radius)
            {
                SetImgPixel(pimg,x,y,0xff);
            }
        }
    }



    for(y=-radius;y<radius;y++)//右面的半个圆全都画成黑色
    {
        for(x=0;x<radius;x++)
        {
            if((x*x)+(y*y)<=square_radius)
            {
                SetImgPixel(pimg,x,y,0);
            }
        }
    }


 for(y=-radius;y<radius;y++)//把第一象限的Y轴变成圆形
     {
         for(x=-radius;x<0;x++)
         {
             if((x*x)+(y*y)<=(radius/2)*(radius/2))
             {
                 //SetImgPixel(pimg,x,y,0);
                 SetImgPixel(pimg,x,(y-(radius/2)),0);
             }
         }
     }
 
 
 
 
     //虽然写出来了 但是有待理解
     for(y=-radius;y<radius;y++)//把第一象限的Y轴变成白色圆形
     {
         for(x=0;x<radius;x++)
         {
             if((x*x)+(y*y)<=(radius/2)*(radius/2))
             {
                 //SetImgPixel(pimg,x,y,0);
                 SetImgPixel(pimg,x,(y+(radius/2)),0xff);
             }
         }
     }
 


square_radius=(radius/8)*(radius/8);
     //从Y轴的正方向开始画 画黑色部分的小白圆
     for(y=-radius;y<radius;y++)
     {
 
            //小圆是以大圆的半径中间为原点，的半径是大圆半径的四分之一
       for(x=-(radius/8);x<(radius/8);x++)
       {
         if(((x*x)+(y*y))<square_radius)
         {
         SetImgPixel(pimg,x,(y-radius/2),0xff);
         }
       }
 
     }
 
 
 
 
 
 
 
 
 
     //从Y轴的负方向开始画 画白色部分的小黑圆
     for(y=-radius;y<radius;y++)
     {
       //小圆是以大圆的半径中间为原点，的半径是大圆半径的四分之一
       for(x=-(radius/8);x<(radius/8);x++)
       {
         if(((x*x)+(y*y))<square_radius)
         {
         SetImgPixel(pimg,x,(y+radius/2),0);
         }
       }
 
     }
  //至此全部画完
 
 
 
 
 
 
 
 
 
 }



void SaveImage(struct MyImage* pimg,const char* p_filename)
 {
     FILE* file = fopen(p_filename,"wb");
     if(file==NULL)
     {
         printf("open file failed\n");
         return ;
     }
     fprintf(file,"P5\n");//格式
     fprintf(file,"%zu %zu\n",pimg->Width,pimg->Height);//宽度和高度
     fprintf(file,"255\n");//最大的灰度值
     fwrite(pimg->P_Data,1,pimg->Width*pimg->Height,file);
     fclose(file);
 }
 
 
 int main(int arvc,int* argv[])
 {
     struct MyImage* p_img =CreatImage(800,800);
     if(p_img != NULL)
     {
         printf("not null\n");
     }
 
     FillImage(p_img,0xaa);//最大为全白 最小为全黑
     DrawImage(p_img,300);
     SaveImage(p_img,"../bagua.pgm");
     DestoryImage(p_img);
 
 
 
 
     return 1;
 }





#pragma endregion
