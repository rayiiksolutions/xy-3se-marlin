#include "yWin.hpp"

yWindows::yWindows(const WIN_Attribute* winAttr,yWindows*parent)
  :Components(winAttr->components),ComponentNum(winAttr->componentNum)
{
	winRect.xStart = winAttr->x;
	winRect.yStart = winAttr->y;
	winRect.xEnd = winRect.xStart + winAttr->width-1;
	winRect.yEnd = winRect.yStart + winAttr->height-1;
	Parent = parent;
	componentOffsetCount(winRect.xStart,winRect.yStart);
	#if defined(LCD_USE_TOUCH)
	touchReset();
	#endif
}
yWindows::~yWindows() {
	if(Parent){
		const WRECT tempRect = yWidget::VisualRect;
		if(getWinVisible()) {
			Parent->setUpdateSign();
			Parent->update();
			Parent->clrUpdateSign();
		}
		yWidget::VisualRect = tempRect;
	}
	for(int i = 0; i < ComponentNum; i++)delete Components[i];
}
	/*******************************************************************************
	* 函数名		: update
	* 描述			: 窗口更新
	* 返回			: -
	* 编写者		: YSZ
	* 编写日期	: 2017-03-15
	* 最近改动	: 2019-02-10
	* 改动内容	: -
	*******************************************************************************/
void yWindows::refresh()
{
	const WRECT tempRect = yWidget::VisualRect;
	int i = 0,j = -1;
	if(getWinVisible()) {
		#if 0
		for(i = 0;i < ComponentNum; i++)Components[i]->update();
		#else
		for(i = 0;i < ComponentNum; i++) {
			//当后面控件不显示或者需要强制重绘前面控件时,需要该控件所在区域被之前的控件显示填充
			if(j == -1 && !Components[i]->sign.content.widget && Components[i]->wgPic)j = i;//找到之前需要填充显示的控件
			if(Components[i]->update()) { //本控件不显示或者attr.former被置1强制重绘前面的控件
				if(j >= 0) { //如果该控件底下有控件,则重绘底下的控件
					const WRECT trect = yWidget::VisualRect;//将前面相关区域的控件显示来填充
					Components[i]->boundIntersect(yWidget::VisualRect.xStart,yWidget::VisualRect.yStart,yWidget::VisualRect.xEnd,yWidget::VisualRect.yEnd);
					for(int k = j;k <= i; k++)Components[k]->update(true);
					yWidget::VisualRect = trect;
				}
				else Components[i]->update(true);//如果没有,则强制更新本控件
			}
		}
		#endif
	}
	yWidget::VisualRect = tempRect;
}
void yWindows::update()
{
	refresh();
	if(Parent == nullptr)return;
	const WRECT tempRect = yWidget::VisualRect;
	if(yWidget::VisualRect.xStart < winRect.xStart)//需要重绘的区域x轴-方向上有部分在本层窗口之外
	{
		if(yWidget::VisualRect.xEnd >= winRect.xStart)yWidget::VisualRect.xEnd = winRect.xStart;
		Parent->update();
		yWidget::VisualRect.xStart = winRect.xStart;
		yWidget::VisualRect.xEnd = tempRect.xEnd;
	}
	if(yWidget::VisualRect.yStart < winRect.yStart)//需要重绘的区域y轴-方向上有部分在本层窗口之外
	{
		if(yWidget::VisualRect.yEnd >= winRect.yStart)yWidget::VisualRect.yEnd = winRect.yStart;
		Parent->update();
		yWidget::VisualRect.yStart = winRect.yStart;
		yWidget::VisualRect.yEnd = tempRect.yEnd;
	}
	if(yWidget::VisualRect.xEnd > winRect.xEnd)//需要重绘的区域x轴+方向上有部分在本层窗口之外
	{
		if(yWidget::VisualRect.xStart <= winRect.xEnd)yWidget::VisualRect.xStart = winRect.xEnd + 1;
		Parent->update();
		yWidget::VisualRect.xEnd = winRect.xEnd;
		yWidget::VisualRect.xStart = tempRect.xStart;
	}
	if(yWidget::VisualRect.yEnd > winRect.yEnd)//需要重绘的区域y轴+方向上有部分在本层窗口之外
	{
		if(yWidget::VisualRect.yStart <= winRect.yEnd)yWidget::VisualRect.yStart = winRect.yEnd + 1;
		Parent->update();
		//yWidget::VisualRect.yEnd=winRect.yEnd;//到这里结束了,注释掉不必浪费运行
		//yWidget::VisualRect.yStart=trect.yStart;
	}
	yWidget::VisualRect = tempRect;
}
/*******************************************************************************
	* 函数名		: move
	* 描述			: 移动窗口
	* 返回			: -
	* 编写者		: YSZ
	* 编写日期	: 2017-03-15
	* 最近改动	: 2019-02-10
	* 改动内容	: -
	*******************************************************************************/
void yWindows::move(int x,int y)
{
	const WRECT trect = yWidget::VisualRect;
	bool tf = getWinVisible();
	WRECT svrect = yWidget::VisualRect;//保留原可视区域
	setUpdateSign();
	componentOffsetCount(x,y);
	winRect.xStart += x;
	winRect.xEnd += x;
	winRect.yStart += y;
	winRect.yEnd += y;
	yWidget::VisualRect = trect;
	refresh();
	if(Parent && tf)
	{
		if(x != 0){
			if(svrect.xStart < winRect.xStart && svrect.xEnd > winRect.xStart){
				yWidget::VisualRect.xStart = svrect.xStart;
				yWidget::VisualRect.xEnd = winRect.xStart;
				svrect.xStart = winRect.xStart + 1;
			}
			else if(svrect.xStart < winRect.xEnd && svrect.xEnd > winRect.xEnd)
			{
				yWidget::VisualRect.xStart = winRect.xEnd;
				yWidget::VisualRect.xEnd = svrect.xEnd;
				svrect.xEnd = winRect.xEnd - 1;
			}
			else {
				yWidget::VisualRect.xStart = svrect.xStart;
				yWidget::VisualRect.xEnd = svrect.xEnd;
				svrect.xStart = svrect.xEnd;
			}
			yWidget::VisualRect.yStart = svrect.yStart;
			yWidget::VisualRect.yEnd = svrect.yEnd;
			Parent->update();
		}
		if(y != 0 && svrect.xEnd > svrect.xStart){
			if(svrect.yStart < winRect.yStart && svrect.yEnd > winRect.yStart){
				yWidget::VisualRect.yStart = svrect.yStart;
				yWidget::VisualRect.yEnd = winRect.yStart;
			}
			else if(svrect.yStart < winRect.yEnd && svrect.yEnd > winRect.yEnd){
				yWidget::VisualRect.yStart = winRect.yEnd;
				yWidget::VisualRect.yEnd = svrect.yEnd;
			}
			else{
				yWidget::VisualRect.yStart = svrect.yStart;
				yWidget::VisualRect.yEnd = svrect.yEnd;
			}
			yWidget::VisualRect.xStart = svrect.xStart;
			yWidget::VisualRect.xEnd = svrect.xEnd;
			Parent->update();
		}
	}
	clrUpdateSign();
	yWidget::VisualRect = trect;
}

/*******************************************************************************
	* 函数名		: clrUpdateSign
	* 描述			: 当绘制一次窗口后,清除所有需绘标记,等待其它
					:	应用标相应标记后再重绘,必须由顶层窗口指针执行
	* 返回			: -
	* 编写者		: YSZ
	* 编写日期	: 2019-02-10
	* 最近改动	: -
	* 改动内容	: -
	*******************************************************************************/
void yWindows::clrUpdateSign()
{
	for(int i=0;i<ComponentNum;i++)Components[i]->sign.all = 0;
	if(Parent)Parent->clrUpdateSign();
}
/*******************************************************************************
	* 函数名		: setUpdateSign
	* 描述			: 使能所有需绘标志,使所有窗口重绘
	* 返回			: -
	* 编写者		: YSZ
	* 编写日期	: 2019-02-10
	* 最近改动	: -
	* 改动内容	: -
	*******************************************************************************/
void yWindows::setUpdateSign()
{
	for(int i=0;i<ComponentNum;i++)Components[i]->sign.all=0xFF;
	if(Parent)Parent->setUpdateSign();
}
/*******************************************************************************
	* 函数名		: setTouch
	* 描述			: 设置触摸点坐标,从Touch芯片读取的原始值
	* 返回			: -
	* 编写者		: YSZ
	* 编写日期	: 2020-10-09
	* 最近改动	: -
	* 改动内容	: -
	*******************************************************************************/
#if defined(LCD_USE_TOUCH)
#define TOUCH_RANGE             1
#define TOUCH_CONFIRM_DELAY     20
TouchCalibrate_t yWindows::para_calibration = {
	XPT2046_X_CALIBRATION, 
	XPT2046_Y_CALIBRATION,
	XPT2046_X_OFFSET,
	XPT2046_Y_OFFSET,
};
void yWindows::touch(int &tx,int &ty)
{
	short x,y;
	int i,tnow = T_NOW_MS();
	switch(touchStep){
	case 0:
		if (GET_TOUCH_CHIP_COORD(x,y)){
			touchStep++;
			touchDelay = tnow + TOUCH_CONFIRM_DELAY;
		}
		break;
	case 1:
		if(!GET_TOUCH_CHIP_COORD(x,y))touchStep--;
		else if(tnow > touchDelay){
			tx = x;
			ty = y;
			getTouchPos(x,y);
			for(i = ComponentNum - 1;i >= 0;i--) {
				if(Components[i]->alwaysExecute) {
					focusIndex = i;
					if(Components[focusIndex]->fun)Components[focusIndex]->fun(Components[focusIndex],wevt_touch);
					touchStep++;
					break;
				}
				else {
					if(Components[i]->range(x,y,TOUCH_RANGE)) {
						tempIndex = i;
						touchStep = 10;
						break;
					}
				}
			}
		}
		break;
	case 2://全屏响应控件
		if(!GET_TOUCH_CHIP_COORD(x,y)) {
			touchStep++;
			// touchDelay = tnow + TOUCH_CONFIRM_DELAY;
		}
		break;
	case 3:
		if(GET_TOUCH_CHIP_COORD(x,y))touchStep--;
		else if(tnow >= touchDelay){
			if(Components[focusIndex]->fun)Components[focusIndex]->fun(Components[focusIndex],wevt_untouch);
			focusIndex = -1;
			tempIndex = -1;
			touchStep = 0;
		}
		break;
	case 10://控件触摸确认消抖
		if(!GET_TOUCH_CHIP_COORD(x,y)){//没到时间
			touchStep++;
			break;
		}
		getTouchPos(x,y);
		for(i = ComponentNum - 1;i >= 0;i--) {
			if(Components[i]->range(x,y,TOUCH_RANGE)){
				if(tempIndex != i){
					#if !USE_TOUCH_AWAY
					tempIndex = i;
					#endif
					touchDelay = tnow + TOUCH_CONFIRM_DELAY;
				}
				break;
			}
		}
		if(touchDelay > 0 && tnow >= touchDelay){
			if(tempIndex != focusIndex) {
				#if USE_TOUCH_AWAY
				if(focusIndex != -1){
					if(Components[focusIndex]->fun)Components[focusIndex]->fun(Components[focusIndex],wevt_away);
				}
				focusIndex = tempIndex;
				if(Components[focusIndex]->fun)Components[focusIndex]->fun(Components[focusIndex],wevt_touch);
				#else
				if(Components[tempIndex]->fun) {
					if(focusIndex != -1) {
						if(Components[focusIndex]->fun)Components[focusIndex]->fun(Components[focusIndex],wevt_away);
					}
					focusIndex = tempIndex;
					Components[focusIndex]->fun(Components[focusIndex],wevt_touch);
				}
				#endif
			}
			touchDelay = -1;
		}
		break;
	case 11://离开消抖
		if(GET_TOUCH_CHIP_COORD(x,y)){
			touchStep--;
			break;
		}
		if(tempIndex >= 0)
		{
			if(focusIndex >= 0) {
				if(Components[focusIndex]->fun)Components[focusIndex]->fun(Components[focusIndex],wevt_untouch);
			}
			tempIndex = -1;
		}
		focusIndex = -1;
		touchStep = 0;
		break;
	}
}
int yWindows::continueTouchTimes(int dis)
{
	short x,y;
	int tnow = T_NOW_MS();
	switch(touchStep){
	case 0:
		if (GET_TOUCH_CHIP_COORD(x,y)) {
			if(touchDelay < 0)tempIndex = 0;//初次点击
			touchDelay = tnow;
			touchStep = 1;
			return -1;
		}
		if(touchDelay > 0) {
			if(tnow > touchDelay + dis) {
				tnow = tempIndex;
				tempIndex = -1;
				touchDelay = -1;
				return tnow;
			}
			return tempIndex;
		}
		break;
	case 1:
		if(!GET_TOUCH_CHIP_COORD(x,y)) {
			if(tnow >= touchDelay + 2) { //按下超过2ms是有效点击,则按的次数+1
				tempIndex++;
				touchDelay = tnow;
			}
			else { //否则被认为是干扰
				touchDelay = -1;
				tempIndex = -1;
			}
			touchStep = 0;
		}
		return -1;
	}
	return 0;
}
bool yWindows::hasStopContinueTouch(int dis)
{
	short x,y;
	int tnow = T_NOW_MS();
	switch(touchStep){
	case 0:
		touchDelay = tnow;
		touchStep = 1;
		break;
	case 1:
		if (GET_TOUCH_CHIP_COORD(x,y)) {
			touchStep = 2;
			break;
		}
		if(tnow > touchDelay + dis) {
			touchStep = 3;
			return true;
		}
		break;
	case 2:
		if(!GET_TOUCH_CHIP_COORD(x,y)) {
			touchDelay = tnow;
			touchStep = 1;
		}
		break;
	case 3: return true;
	}
	return false;
}
#endif //#if defined(LCD_USE_TOUCH)
