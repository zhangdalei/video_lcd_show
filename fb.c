
/* framebuffer显示bmp图片 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include "v4l2_fb.h"

static struct fb_fix_screeninfo fixinfo;
static struct fb_var_screeninfo varinfo;

static int fb_fd = 0;
static int* fbmmap;
typedef unsigned int uint;

/* 1、open打开framebuffer */
int openframebuffer()
{
	int ret = 0;
	fb_fd = open("/dev/fb0", O_RDWR);	// /dev/graphics/fb0
	if(fb_fd < 0){
		printf("open framebuffer error.\n");
		exit(0);
	}
	/* 获取可变参数 */
	printf("-------------info-----------------\n");
	ret = ioctl(fb_fd, FBIOGET_VSCREENINFO, &varinfo);
	if(ret != 0){
		printf("get varinfo error.\n");
		exit(0);
	}else
		printf("xres:%d  \nyres:%d  \nxres_virtual:%d  \nyres_virtual:%d \nbpp:%d\n",varinfo.xres,varinfo.yres,\
	varinfo.xres_virtual,varinfo.yres_virtual,varinfo.bits_per_pixel);
	/* 获取固定参数 */
	ret = ioctl(fb_fd, FBIOGET_FSCREENINFO, &fixinfo);
	if(0 != ret){
		printf("get fixinfo error.\n");
		exit(0);
	}else
		printf("buflen:%d \nline_length:%d\n",fixinfo.smem_len,fixinfo.line_length);
	return 0;
}

/* 2、映射mmap内存到用户空间 */
void init_fbmmap()
{
	fbmmap = (int*)mmap(NULL, fixinfo.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED,
		fb_fd, 0);
	if(NULL == fbmmap){
		printf("mmap memeory error.\n");
		exit(0);
	}
}
/* 绘制背景颜色 */
void fb_drawback(uint height,uint width,uint color)
{
	uint x,y;
	for(x=0; x<height; x++)
		for(y=0; y<width; y++)
		{
			*(fbmmap + x*(varinfo.xres)+y) = color;
		}
}
/* 绘制bmp数据到framebuffer */
void fb_drawbmp(uint height,uint width,unsigned char* pdata)
{
	uint x,y,cnt;
	uint a = height*width*3;
	uint cnt1 = 0;
	uint* p = fbmmap;
	for(x=0; x<height; x++)
		for(y=0; y<width; y++)
		{
			cnt = x*(varinfo.xres)+y;		// LCD像素位置
			cnt1 = (x*width+width-y)*3;			// 图片像素位置,width-y用于解决图像显示左右反了的问题
			*(p+cnt) = (pdata[cnt1]<<0) | (pdata[cnt1+1]<<8) | (pdata[cnt1+2]<<16);
		}
}


void closefb(void)
{
	munmap(fbmmap, fixinfo.smem_len);
	close(fb_fd);
}

/*
int main()
{
	openframebuffer();
	init_fbmmap();
	fb_drawback(varinfo.yres, varinfo.xres, 0x00f0);
	closefb();
	return 0;
}
*/

