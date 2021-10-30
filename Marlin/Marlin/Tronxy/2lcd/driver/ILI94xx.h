#pragma once

#include "1inc/yType.h"
#include "1inc/yBoard.h"

/*******************************************************************************
-->需在硬件文件中提供以下接口
1.lcdMcuInit()	MCU对屏的IO初始化
2.lcdReset()    MCU对屏的复位
3.LCD_WReg(x)	写命令(字)
4.LCD_WData(x)	写数据(字)
5.LCD_RData()	读数据(字)
6.LCD_DelayMs(x)	毫秒等待接口
7.LCD_BatchStart(colors,len,autoInc) + LCD_BatchSyn(colors,len,autoInc) 批量写颜色
8.LCD_BatchOk() 批量写完成判断

-->本驱动提供以下接口
1.LCD_Init(dir)	屏初始化
2.LCD_SetDir(dir)	设置屏的方向
3.LCD_WArea(x,y,w,h) //屏区域写框定
4.LCD_RArea(x,y,w,h) //屏区域读框定
5.LCD_WPixel(x,y,cl) //在坐标x,y处写颜色
6.LCD_RPixel(x,y,r,g,b) //在坐标x,y处读取颜色并保存到r,g,b分量内
7.LCD_Close() //关闭窗口系统时调用(单片机无此定义)

-->本驱动提供以下参数
LCD_LMAX 屏的物理长(单位:像素)
LCD_WMAX 屏的物理宽(单位:像素)
LCD_LBuffer1,LCD_LBuffer2 一行最长边双缓冲内存
*******************************************************************************/

/************本驱动目录支持ILI9486-ILI9488,其它芯片没验证************/
#define	LCD_LMAX                                  480
#define	LCD_WMAX                                  320
#define	LCDDIR_INVERT_RB                          0x08	//红蓝交换
#define	LCDDIR_HVCHANGE                           0x20	//第5位是1时为横屏
#define	LCDDIR_REVERSE_W                          0x40	//第6位确定宽方向
#define	LCDDIR_REVERSE_L                          0x80	//第7位确定长方向

#define ILI94xx_WStart() LCD_WReg(0x2C) //开始写
#define ILI94xx_RStart() LCD_WReg(0x2E) //开始读
//不同的芯片RGB组合方式可能不一样
#define ILI94xx_RColor(r,g,b) LCD_RData(b);b >>= 11;LCD_RData(r);r >>= 11;LCD_RData(g);g >>= 10/*读取的颜色是顺序是BRG*/
#define ILI94xx_Area(xStart,yStart,xEnd,yEnd) do\
{\
	LCD_WReg(0x2A);/*x*/\
	LCD_WData((xStart)>>8);\
	LCD_WData((xStart)&0xFF);\
	LCD_WData((xEnd)>>8);\
	LCD_WData((xEnd)&0xFF);\
	LCD_WReg(0x2B);/*y*/\
	LCD_WData((yStart)>>8);\
	LCD_WData((yStart)&0xFF);\
	LCD_WData((yEnd)>>8);\
	LCD_WData((yEnd)&0xFF);\
}while(0)
#define ILI94xx_WArea(xStart,yStart,xEnd,yEnd) ILI94xx_Area(xStart,yStart,xEnd,yEnd);ILI94xx_WStart()
#define ILI94xx_RArea(xStart,yStart,xEnd,yEnd) ILI94xx_Area(xStart,yStart,xEnd,yEnd);ILI94xx_RStart()
#define ILI94xx_WPixel(x,y,color) do\
{\
	ILI94xx_WArea(x,y,x,y);\
	LCD_WData(color);/*如果是8位传输,这里需要写成先写color高8位,再写低8位*/\
}while(0)
#define ILI94xx_RPixel(x,y,R,G,B) do\
{\
	ILI94xx_RArea(x,y,x,y);\
	LCD_RData(R);/*第一次假读dummy*/\
	ILI94xx_RColor(R,G,B);\
}while(0)

extern color_t *LCD_LBuffer1, *LCD_LBuffer2;
extern color_t LCD_TOTAL_BUFF[];
void ILI94xx_Init(unsigned char dir = LCDDIR_HVCHANGE | LCDDIR_INVERT_RB | LCDDIR_REVERSE_W | LCDDIR_REVERSE_L);
void ILI94xx_SetDir(unsigned char dir);
void ILI94xx_rect(int x,int y,int w,int h,color_t color,BYTE transparent=0);
void ILI94xx_horizontalRect(int x,int y,int w,int h,color_t colorStart,color_t colorEnd,BYTE transparent=0);
void ILI94xx_verticalRect(int x,int y,int w,int h,color_t colorStart,color_t colorEnd,BYTE transparent=0);
void ILI94xx_frame(int x,int y,int w,int h,color_t color,BYTE bold=1);
void ILI94xx_draw(int x,int y,int w,int h,const color_t* pcolor);
void ILI94xx_putc(int xStart,int yStart,const int wide,const int height,const int bytesPerLine,const BYTE* caddr,color_t fr = COLOR_GREEN,color_t bg = COLOR_BLACK,bool ifTransparent = false);
//将底层函数重命名为公有接口函数,不可直接写公有接口函数,因为一个工程可能会有很多TFT屏底层接口,如果都用公有接口,会重名,并且调用会出错
#define LCD_Init                                                        ILI94xx_Init
#define	LCD_SetDir                                                      ILI94xx_SetDir
#define	LCD_WArea                                                       ILI94xx_WArea
#define	LCD_RArea                                                       ILI94xx_RArea
#define	LCD_WPixel                                                      ILI94xx_WPixel
#define	LCD_RPixel                                                      ILI94xx_RPixel
#define LCD_Close()
