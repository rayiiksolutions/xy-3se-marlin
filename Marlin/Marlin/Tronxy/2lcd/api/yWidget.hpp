
#pragma once

#include "1inc/yBoard.h"
#include "2lcd/driver/ILI94xx.h"
/****************************************
-->驱动程序需要提供以下接口
1.LCD_Init(dir)	屏初始化
2.LCD_SetDir(dir)	设置屏的方向
3.LCD_WArea(x,y,w,h) //屏区域写框定
4.LCD_RArea(x,y,w,h) //屏区域读框定
5.LCD_WData(color) //向屏幕写一个颜色值,执行3,4后可以执行本接口
6.LCD_BatchStart(colors,len,autoInc) 批量写颜色,执行3,4后可以执行本接口
7.LCD_BatchOk() 批量写完成判断
8.LCD_WPixel(x,y,cl) //在坐标x,y处写颜色
9.LCD_RPixel(x,y,r,g,b) //在坐标x,y处读取颜色并保存到r,g,b分量内
10.LCD_Close() //关闭窗口系统时调用(单片机无此定义)

-->如果使用外部ROM读存数据的话,需要提供以下接口:
1.EROM_Init
2.EROM_ReadStart
3.EROM_ReadOneByte
4.EROM_ReadSyn
5.EROM_ReadOk

-->需要提供以下参数
LCD_LMAX 屏的物理长(单位:像素)
LCD_WMAX 屏的物理宽(单位:像素)
LCD_LBuffer1,LCD_LBuffer2 屏幕一行像素的一级(准备数据)和二级(显示数据)缓存空间
****************************************/
#include "yType.hpp"
typedef void (*VoidFun)(void);
#define _NEW_W(x,v)  x = new v
#define WIDGET_FRAME_COLOR               0xEE0E
#define HORIZONTAL_LINE(color,y,xStart,xEnd)  while(!LCD_BatchOk()){} LCD_WArea(xStart,y,xEnd,y);LCD_BatchStart(&color,xEnd - xStart + 1,false)
#define VERTICAL_LINE(color,x,yStart,yEnd)    while(!LCD_BatchOk()){} LCD_WArea(x,yStart,x,yEnd);LCD_BatchStart(&color,yEnd - yStart + 1,false)

#define USE_TOUCH_AWAY   0 //置1后,当触控件离开后再抬不执行,untouch任务,而是away任务,可以做到防误触,但是它可能会不灵敏
#define MY_TEST  0
enum WidgetEvent
{
	wevt_none = 0,
	wevt_touch,
	wevt_untouch,
	wevt_away,
};
class yWidget
{
	public:
		yWidget(int xStart = 0,int yStart = 0,int width = 0,int height = 0,
		const PICinfo*picIlde = 0,const PICinfo*picFocus = 0,bool enfocus = true);
		virtual ~yWidget(){}
		
	protected:
		bool focus;
		bool enFocus;
		int pxOffset,pyOffset;//当wgPic是图像时,图像相对wgRect的偏移由这里设定
		int w_xstart,w_ystart,w_xend,w_yend;//本控件范围
	public:
		union{
			struct{
				int widget:1;
				int text:1;
				int textCurrent:1;//第一次不显示,一般用在需要第一次刷背景,第二次显示文本的情况
				int scale:1;
			}content;//这里标记update绘制哪些部分,见宏定义
			BYTE all;
		}sign;
		bool alwaysExecute;//true一旦触摸,必然响应,false,只有触摸到本控件才响应
		uint32_t funParam;//fun的参数,有可能是个地址
		const PICinfo *wgPic;
		const PICinfo *wgPic_idle;//未选中
		const PICinfo *wgPic_focus;//选中
		void (*fun)(yWidget*w,WidgetEvent evt);//发生触控事件后,会立即调用本函数
		void (*funNext)(void);//发生触控后,调用fun后,在外面调用本函数,可能会释放本控件
		
		virtual bool update(bool force = false);//刷新界面,返回true表示该控件可视区域无显示,
		inline bool range(int x,int y,int r){return (x >= w_xstart + r && x <= w_xend - r) && (y >= w_ystart + r && y <= w_yend - r);}
		inline virtual void setRectOffset(int x,int y) {
			w_xstart += x;
			w_xend += x;
			w_ystart += y;
			w_yend += y;
		}
		inline void setWgpOffset(int x,int y){pxOffset = x;pyOffset = y;}//设置图标偏移,前提是wgPic为图像
		inline virtual void setfocus(bool tf) {
			if(enFocus){
				if(tf && !focus) {
					wgPic = wgPic_focus;
					binaryInfo.fr = fr_focus;
					binaryInfo.bg = bg_focus;
					sign.content.widget = 1;
					sign.content.scale = 1;
				}
				else if(!tf && focus) {
					wgPic = wgPic_idle;
					binaryInfo.fr = fr_idle;
					binaryInfo.bg = bg_idle;
					sign.content.widget = 1;
					sign.content.scale = 1;
				}
				focus = tf;
			}
		}
		//求wgRect与VisualRect求交集
		inline bool boundIntersect(int& xStart,int& yStart,int& xEnd,int& yEnd)
		{
			xStart = yMAX(w_xstart, VisualRect.xStart);
			xEnd = yMIN(w_xend, VisualRect.xEnd);
			yStart = yMAX(w_ystart, VisualRect.yStart);
			yEnd = yMIN(w_yend, VisualRect.yEnd);
			return (xEnd < xStart || yStart > yEnd);
		}
		void rect(color_t color,int xStart,int yStart,int xEnd,int yEnd);//填充一个矩形区域
	
	protected:
		//将x,y围与VisualRect求交集
		static inline bool boundIntersect2(int& xStart,int& yStart,int& xEnd,int& yEnd)
		{
			if(xStart<VisualRect.xStart)xStart=VisualRect.xStart;
			if(xEnd>VisualRect.xEnd)xEnd=VisualRect.xEnd;
			if(yStart<VisualRect.yStart)yStart=VisualRect.yStart;
			if(yEnd>VisualRect.yEnd)yEnd=VisualRect.yEnd;
			return (xEnd<xStart||yStart>yEnd);
		}
	//将x,y围与wgRect和VisualRect三方求交集
		inline bool boundIntersect3(int* xStart,int* yStart,int* xEnd,int* yEnd)
		{
			if(w_xstart < VisualRect.xStart)
			{
				if(*xStart < VisualRect.xStart)*xStart = VisualRect.xStart;
			}
			else
			{
				if(*xStart < w_xstart)*xStart = w_xstart;
			}
			if(w_ystart < VisualRect.yStart)
			{
				if(*yStart < VisualRect.yStart)*yStart = VisualRect.yStart;
			}
			else
			{
				if(*yStart < w_ystart)*yStart = w_ystart;
			}
			if(w_xend > VisualRect.xEnd)
			{
				if(*xEnd > VisualRect.xEnd)*xEnd = VisualRect.xEnd;
			}
			else
			{
				if(*xEnd > w_xend)*xEnd = w_xend;
			}
			if(w_yend > VisualRect.yEnd)
			{
				if(*yEnd > VisualRect.yEnd)*yEnd = VisualRect.yEnd;
			}
			else
			{
				if(*yEnd>w_yend)*yEnd = w_yend;
			}
			return (*xStart > *xEnd || *yStart > *yEnd);
		}
		#if defined(LCD_USE_EROM)
    	void showEBytes(int x,int y);//显示外部ROM中的字节图像-图像数据以字节形式保存
		void showECompressPic(int x,int y);//显示外部ROM中压缩图形,行
		void showECompressPic_t(int x,int y);//显示外部ROM中压缩图形,整(<=32x32)
		#endif
    	void showIBytes(int x,int y);//显示IFLASH中的字节图像-图像数据以字节形式保存
		inline void horizontalLine(color_t color,int xStart,int xEnd,int y)
			{//在屏幕上绘制一条横线
				if(y < w_ystart || y < VisualRect.yStart || y > w_yend || y > VisualRect.yEnd)return;
				if(xStart < w_xstart)xStart = w_xstart;
				if(xStart < VisualRect.xStart)xStart = VisualRect.xStart;
				if(xEnd > w_xend)xEnd = w_xend;
				if(xEnd > VisualRect.xEnd)xEnd = VisualRect.xEnd;
				if(xStart > xEnd)return;
				HORIZONTAL_LINE(color,y,xStart,xEnd);
			}
		inline void verticalLine(color_t color,int x,int yStart,int yEnd)
			{//在屏幕上绘制一条竖线
				if(x < VisualRect.xStart || x < w_xstart || x > VisualRect.xEnd || x > w_xend)return;
				if(yStart < w_ystart)yStart = w_ystart;
				if(yStart < VisualRect.yStart)yStart = VisualRect.yStart;
				if(yEnd > w_yend)yEnd = w_yend;
				if(yEnd > VisualRect.yEnd)yEnd = VisualRect.yEnd;
				if(yStart > yEnd)return;
				VERTICAL_LINE(color,x,yStart,yEnd);
			}
		void line(color_t color,int xStart,int yStart,int xEnd,int yEnd);//画线
		inline void frame(color_t color,int xStart,int yStart,int xEnd,int yEnd){
				verticalLine(color,xStart,yStart,yEnd);
				verticalLine(color,xEnd,yStart,yEnd);
				horizontalLine(color,xStart,xEnd,yStart);
				horizontalLine(color,xStart,xEnd,yEnd);
				while(!LCD_BatchOk()){}
			}
		void pie(color_t color,int r,int x,int y);

	protected:
		BinaryInfo binaryInfo;
		color_t fr_idle,fr_focus;
		color_t bg_idle,bg_focus;
		#if defined(LCD_USE_EROM) //存在外部FLASH时,启用
		void showED8b( //显示外存二位图,返回x末点位置
			int xStart,int yStart,int xEnd,int yEnd,//可视范围(字形范围与VisualRect的交集)
			DWORD eAddr);//二元色码外部FLASH地址
		void getPureED8bScale(int& xStart,int& xEnd,uint32_t eAddr);
		#endif
		void showID8b(//显示内存二位图,返回x末点位置
			int xStart,int yStart,int xEnd,int yEnd,//可视范围(字形范围与VisualRect的交集)
			const BYTE *idat);//二元色内部FLASH地址
		void getPureID8bScale(int& xStart,int& xEnd,const BYTE* idat);
		void showBinary(int x,int y,bool erom = false);

	public:
		inline void setBinaryOpaque(bool tf){if(tf)binaryInfo.ifOpaque = 1;else binaryInfo.ifOpaque = 0;}
		inline void setBinaryFrColor(color_t idle_color,color_t focus_color) {
			binaryInfo.fr = fr_idle = idle_color;
			fr_focus = focus_color;
		}
		inline void setBinaryBgColor(color_t idle_color,color_t focus_color) {
			binaryInfo.bg = bg_idle = idle_color;
			bg_focus = focus_color;
		}

	public:
		static WRECT VisualRect;//可见区域
};
typedef void(*PicFun)(yWidget*,int xStart,int yStart,int xEnd,int yEnd);
