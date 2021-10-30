#pragma once

#include "yWidget.hpp"

#if defined(LCD_USE_TOUCH)
typedef struct __attribute__((__packed__)) {
  int x;
  int y;
  short offset_x;
  short offset_y;
} TouchCalibrate_t;
#define GET_TOUCH_CHIP_COORD(x,y)   XPT2046::getRawPoint(&x, &y)
#endif
#define T_NOW_MS()       millis()
struct WIN_Attribute
{
	int x,y;//初始位置
	int width,height;//宽高
	yWidget** components;
	int componentNum;
};
class yWindows
{
	public:
		class yWindows *Parent;
		yWindows(const WIN_Attribute*winAttr,yWindows*parent=0);
		~yWindows();
		void refresh();//刷新本窗口
		void update();//刷新窗口(包括父窗口)
	
		void clrUpdateSign();//当绘制一次窗口后,清除所有需绘标记,等待其它应用标相应标记后再重绘,必须有顶端窗口指针执行
		void setUpdateSign();
		void move(int x,int y);
		yWidget* getComponent(int index){if(index < ComponentNum)return Components[index];return nullptr;}
		#if defined(LCD_USE_TOUCH)
		int touchDelay;
		static TouchCalibrate_t para_calibration;
		void touch(int &x,int &y);
		int continueTouchTimes(int dis = 200);//检测屏幕被连续点击(两次点击时间小于dis ms)的次数,正在点击中返回负数,没有点击,返回0,点击完成返回点击的次数
		bool hasStopContinueTouch(int dis = 1000);//停止了连续点击
		inline void touchReset(void) {
			touchStep = 0;
			touchDelay = -1;
			focusIndex = -1;
			tempIndex = -1;
		}
		static inline void getTouchPos(short &x, short &y) {
			x = ((x * para_calibration.x) >> 16) + para_calibration.offset_x;
    		y = ((y * para_calibration.y) >> 16) + para_calibration.offset_y;
		}
		#endif
		
	private:
		#if defined(LCD_USE_TOUCH)
		volatile uint8_t touchStep;
		int tempIndex;
		#endif
		int focusIndex;
		WRECT winRect;
		yWidget** Components;
		const int ComponentNum;
		inline void componentOffsetCount(int x,int y)
		{
			for(int i = 0;i<ComponentNum;i++)Components[i]->setRectOffset(x,y);
		}
		inline bool getWinVisible()
		{
			if(yWidget::VisualRect.xStart < winRect.xStart)yWidget::VisualRect.xStart = winRect.xStart;
			if(yWidget::VisualRect.xEnd > winRect.xEnd)yWidget::VisualRect.xEnd=winRect.xEnd;
			if(yWidget::VisualRect.yStart < winRect.yStart)yWidget::VisualRect.yStart = winRect.yStart;
			if(yWidget::VisualRect.yEnd > winRect.yEnd)yWidget::VisualRect.yEnd = winRect.yEnd;
			return !(yWidget::VisualRect.xStart > yWidget::VisualRect.xEnd ||
				yWidget::VisualRect.yStart > yWidget::VisualRect.yEnd);
		}
};

