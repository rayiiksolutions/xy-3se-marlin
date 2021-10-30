#ifndef _CENTOS7_LCD_H
#define _CENTOS7_LCD_H
#include "../../1inc/yBoard.h"
/*******************************************************************************
-->需在Board.h中提供以下定义和接口
宏:PAGE_SHIFT,PAGE_SIZE,PAGE_MASK
结构:struct fb_fix_screeninfo,struct fb_var_screeninfo
接口:ioctl,mmap

-->本驱动提供以下接口
1.LCD_Init(dir)	屏初始化[当前使用默认方向]
2.LCD_SetDir(dir)	设置屏的方向[当前没用到]
3.LCD_BatchWStart(x1,y1,x2,y2)	//屏批量写准备
4.LCD_SetWPos(x,y)	//定位:跳到x,y处写,"批量写准备"已经包含了定位
5.LCD_WColor(cl)	//写一个颜色,cl是一个像素颜色的值,3/4其中一个执行后方可执行5
6.LCD_WBytesColor(bcAddr)  //写一个颜色,bcAddr是颜色的字节地址,3/4其中一个执行后方可执行6
7.LCD_WPixel(x,y,cl)	//在坐标x,y处写颜色
8.LCD_BatchRStart(x1,y1,x2,y2)	//屏批量读准备
9.LCD_SetRPos(x,y)	//定位:跳到x,y处读,"批量读准备"已经包含了定位
10.LCD_RColor(r,g,b)	//读取颜色,保存在r,g,b变量内,6/7任意一个执行后方可执行8
11.LCD_RPixel(x,y,r,g,b,t)	//在坐标x,y处读取颜色并保存到r,g,b分量内
12.LCD_Close() //关闭窗口系统时调用[单片机不用]

-->本驱动提供以下参数
LCD_LMAX	屏的物理长(单位:像素)
LCD_WMAX 屏的物理宽(单位:像素)
*******************************************************************************/
#if 0
#define LCD_LMAX   800
#define LCD_WMAX   600

extern int frameBuffer_RectWidth;
extern u64 frameBuffer_BaseAddr;
extern color32 *frameBuffer_xStart,*frameBuffer_Ultimate;
extern color32 *frameBuffer_Pen;
extern struct fb_var_screeninfo CentOS7FbVar;
extern struct fb_fix_screeninfo CentOS7FbFix;
extern u64 CentOS7FbMemOffset,CentOS7FbMem;

int CentOS7Rtft_Init();
void CentOS7Rtft_Close();
void CentOS7Rtft_SetVisible(int width,int height);
void CentOS7Rtft_SetVirtual(int width,int height);
/*******************************************************************************
* 函数名		: CentOS7Rtft_Moveto
* 描述			: 跳到指定位置开始操作(读或写)
* 返回			: -
* 编写者		: YSZ
* 编写日期	: 2019-02-12
* 最近改动	: -
* 改动内容	: -
*******************************************************************************/
#define CentOS7Rtft_Moveto(xStart,yStart) \
do{\
	frameBuffer_Pen=(color32*)(frameBuffer_BaseAddr+(yStart)*(CentOS7FbFix.line_length>>2)+(xStart));\
}while(0)

#define CentOS7Rtft_frameBufferRectCheck  frameBuffer_Pen++;\
  if(frameBuffer_Pen>frameBuffer_xStart+frameBuffer_RectWidth)\
  {frameBuffer_Pen=(color32*)((u64)frameBuffer_xStart+(CentOS7FbFix.line_length>>2));\
    if(frameBuffer_Pen>frameBuffer_Ultimate)frameBuffer_Pen=frameBuffer_xStart;\
    else frameBuffer_xStart=frameBuffer_Pen;}
  
#define CentOS7Rtft_BatchWStart(xStart,yStart,xEnd,yEnd) \
do{CentOS7Rtft_Moveto(xStart,yStart);\
  frameBuffer_xStart=frameBuffer_Pen;\
  frameBuffer_RectWidth=xEnd-xStart;\
  frameBuffer_Ultimate=(color32*)((u64)frameBuffer_xStart+(CentOS7FbFix.line_length>>2)*(yEnd-yStart));\
}while(0)

#define CentOS7Rtft_WColor(cl) \
do{*frameBuffer_Pen=cl;\
  CentOS7Rtft_frameBufferRectCheck;\
}while(0)

#define CentOS7Rtft_WBytesColor(bcAddr) \
do{BYTE*p=(BYTE*)frameBuffer_Pen;\
  p[0]=bcAddr[0];p[1]=bcAddr[1];p[2]=bcAddr[2];p[3]=bcAddr[3];\
  CentOS7Rtft_frameBufferRectCheck;\
}while(0)

#define CentOS7Rtft_RColor(r,g,b,t) \
do{yFRGB32(*frameBuffer_Pen,r,g,b,t);\
  CentOS7Rtft_frameBufferRectCheck;\
}while(0)

#define LCD_Init(x)                  CentOS7Rtft_Init()
#define LCD_SetDir(dir)
#define LCD_BatchWStart(x1,y1,x2,y2) CentOS7Rtft_BatchWStart(x1,y1,x2,y2)
#define LCD_SetWPos(x,y)             CentOS7Rtft_Moveto(x,y)
#define LCD_WColor(cl)               CentOS7Rtft_WColor(cl)
#define LCD_WBytesColor(bcAddr)      CentOS7Rtft_WBytesColor(bcAddr)
#define LCD_WPixel(x,y,cl)           do{LCD_SetWPos(x,y);*frameBuffer_Pen=cl;}while(0)
#define LCD_BatchRStart(x1,y1,x2,y2) LCD_BatchWStart(x1,y1,x2,y2)
#define LCD_SetRPos(x,y)             CentOS7Rtft_Moveto(x,y)
#define LCD_RColor(r,g,b,t)          CentOS7Rtft_RColor(r,g,b,t)
#define LCD_RPixel(x,y,r,g,b)        do{CentOS7Rtft_Moveto(x,y);BYTE t;LCD_RColor(r,g,b,t);}while(0)
#define LCD_Close()                  CentOS7Rtft_Close()
#endif //#if LCD_MODE == LCD_CENTOS7_V1

#endif
