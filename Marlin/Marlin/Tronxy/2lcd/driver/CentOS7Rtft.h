#ifndef _CENTOS7_LCD_H
#define _CENTOS7_LCD_H
#include "../../1inc/yBoard.h"
/*******************************************************************************
-->����Board.h���ṩ���¶���ͽӿ�
��:PAGE_SHIFT,PAGE_SIZE,PAGE_MASK
�ṹ:struct fb_fix_screeninfo,struct fb_var_screeninfo
�ӿ�:ioctl,mmap

-->�������ṩ���½ӿ�
1.LCD_Init(dir)	����ʼ��[��ǰʹ��Ĭ�Ϸ���]
2.LCD_SetDir(dir)	�������ķ���[��ǰû�õ�]
3.LCD_BatchWStart(x1,y1,x2,y2)	//������д׼��
4.LCD_SetWPos(x,y)	//��λ:����x,y��д,"����д׼��"�Ѿ������˶�λ
5.LCD_WColor(cl)	//дһ����ɫ,cl��һ��������ɫ��ֵ,3/4����һ��ִ�к󷽿�ִ��5
6.LCD_WBytesColor(bcAddr)  //дһ����ɫ,bcAddr����ɫ���ֽڵ�ַ,3/4����һ��ִ�к󷽿�ִ��6
7.LCD_WPixel(x,y,cl)	//������x,y��д��ɫ
8.LCD_BatchRStart(x1,y1,x2,y2)	//��������׼��
9.LCD_SetRPos(x,y)	//��λ:����x,y����,"������׼��"�Ѿ������˶�λ
10.LCD_RColor(r,g,b)	//��ȡ��ɫ,������r,g,b������,6/7����һ��ִ�к󷽿�ִ��8
11.LCD_RPixel(x,y,r,g,b,t)	//������x,y����ȡ��ɫ�����浽r,g,b������
12.LCD_Close() //�رմ���ϵͳʱ����[��Ƭ������]

-->�������ṩ���²���
LCD_LMAX	��������(��λ:����)
LCD_WMAX ���������(��λ:����)
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
* ������		: CentOS7Rtft_Moveto
* ����			: ����ָ��λ�ÿ�ʼ����(����д)
* ����			: -
* ��д��		: YSZ
* ��д����	: 2019-02-12
* ����Ķ�	: -
* �Ķ�����	: -
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
