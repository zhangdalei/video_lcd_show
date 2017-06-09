
#ifndef _FB_FUN_H
#define _FB_FUN_H


int openCamera(int id);
void capabilityCamera();
void enumfmtCamera();
int setfmtCamera();
int initmmap();
static int startcap();
int readfram();
static void closeCamera();


int openframebuffer();
void init_fbmmap();
void fb_drawback(uint height,uint width,uint color);
void fb_drawbmp(uint height,uint width,unsigned char* pdata);
void closefb(void);


#endif


