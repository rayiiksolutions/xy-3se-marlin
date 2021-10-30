#include "yWidget.hpp"
#include "3rom/romdata.h"
#include "yCompress.h"
WRECT yWidget::VisualRect={0,0,LCD_LMAX,LCD_WMAX};
yWidget::yWidget(int xStart,int yStart,int width,int height,const PICinfo*picIdle,const PICinfo*picFocus,bool enfocus)
:w_xstart(xStart),w_ystart(yStart),w_xend(xStart + width - 1),w_yend(yStart + height - 1)
{
	binaryInfo.ifOpaque = 0;
	fr_idle = fr_focus = binaryInfo.fr = COLOR_BLACK;
	bg_idle = bg_focus = binaryInfo.bg = COLOR_WHITE;
	wgPic_focus = picFocus;
	wgPic = wgPic_idle = picIdle;
	pxOffset = 0;
	pyOffset = 0;
	alwaysExecute = false;
	fun = nullptr;
	funNext = nullptr;
	funParam = 0;
	focus = false;
	enFocus = enfocus;
	sign.content.widget = 1;
}
bool yWidget::update(bool force)
{
	bool ok = false;
	if(sign.content.widget || force) {
		if(wgPic)
		{
			if(wgPic->attr.former && sign.content.widget) {
				sign.content.widget = 0;
				return true;
			}
			switch(wgPic->attr.type) {
			case TYPE_BINARY:
				showBinary(w_xstart + pxOffset, w_ystart + pyOffset,wgPic->attr.ifERom);
				break;
			case TYPE_COLOR:
				rect(wgPic->value,w_xstart,w_ystart,w_xend,w_yend);
				break;
			case TYPE_PIE:
				pie(wgPic->value,(w_xend - w_xstart) / 2,(w_xstart + w_xend) / 2,(w_ystart + w_yend) / 2);
				break;
			case TYPE_PIC:
				if(wgPic->attr.ifERom) {
					showEBytes(w_xstart + pxOffset, w_ystart + pyOffset);
				}
				else {
					showIBytes(w_xstart + pxOffset, w_ystart + pyOffset);
				}
				break;
			case TYPE_CPIC:
				if(wgPic->attr.typeTf) { //行
					showECompressPic(w_xstart + pxOffset, w_ystart + pyOffset);
				}
				else { //整
					showECompressPic_t(w_xstart + pxOffset, w_ystart + pyOffset);
				}
				break;
			case TYPE_SELFFUN:
				((PicFun)wgPic->value)(this,w_xstart,w_ystart,w_xend,w_yend);
				break;
			default:
				sign.content.widget = 0;
				ok = true;
				break;
			}
			if(wgPic->attr.ifFrame && wgPic->attr.type != TYPE_PIE)
			{
				frame(WIDGET_FRAME_COLOR,w_xstart,w_ystart,w_xend,w_yend);
			}
		}
		else if(sign.content.widget){
			sign.content.widget = 0;
			ok = true;
		}
	}
	return ok;
}
/*******************************************************************************
* 函数名    : line
* 描述      : 任意方向画一条线
* 返回      : -
* 编写者    : YSZ
* 编写日期	: 2016-12-13
* 最近改动	: 2019-02-05
* 改动内容	: -
*******************************************************************************/
void yWidget::line(color_t color,
	int xStart,int yStart,
	int xEnd,int yEnd)
	{
		int yi,yx;
		if(yStart>yEnd)
		{
			yi=yEnd;
			yx=yStart;
		}
		else {
			yi=yStart;
			yx=yEnd;
		}
		if(boundIntersect2(xStart,yi,xEnd,yx))return;
		int xLen=xEnd-xStart;
		if(xLen==0)verticalLine(color,xStart,yStart,yEnd);
		else {
			float k = yEnd - yStart,d;
			k /= xLen;
			if(yEnd>yStart){
				d = 0.5;
				yStart = yi;
			}
			else{
				d = -0.5;
				yStart = yx;
			}
			for(int x = 0;x <= xLen; x++)
			{
				xEnd = k * x + yStart + d;
				LCD_WPixel(x + xStart,xEnd,color);
			}
		}
	}
/*******************************************************************************
* 函数名    : rect
* 描述      : 填充一个区域
* 返回      : -
* 编写者    : YSZ
* 编写日期  : 2016-12-13
*******************************************************************************/
	void yWidget::rect(color_t color,int xStart,int yStart,int xEnd,int yEnd)
	{
		if(boundIntersect3(&xStart,&yStart,&xEnd,&yEnd)) return;
		if(wgPic && wgPic->zero > 0) {
			color_t cR1,cG1,cB1;
			color_t cR2,cG2,cB2;
			int xpos = 0,i,w = xEnd - xStart + 1;
			ColorToRGB(color,cR1,cG1,cB1);
			cR1 *= (256 - wgPic->zero);
			cG1 *= (256 - wgPic->zero);
			cB1 *= (256 - wgPic->zero);
			do{
				while(!LCD_BatchOk()){}
				for(i = 0,xpos = xStart;i < w;i++, xpos++){
					LCD_RPixel(xpos,yStart,cR2,cG2,cB2);
					cR2 = (cR1 + cR2 * wgPic->zero + 128) >> 8;
					cG2 = (cG1 + cG2 * wgPic->zero + 128) >> 8;
					cB2 = (cB1 + cB2 * wgPic->zero + 128) >> 8;
					LCD_LBuffer2[i] = RGBToColor(cR2,cG2,cB2);
				}
				LCD_WArea(xStart,yStart,xEnd,yStart);
				LCD_BatchStart(LCD_LBuffer2,w,true);
			}while(++yStart <= yEnd);
		}
		else {
			int len = (xEnd - xStart + 1) * (yEnd - yStart + 1);
			while(!LCD_BatchOk()){}
			LCD_WArea(xStart,yStart,xEnd,yEnd);
			LCD_BatchSyn(&color,len,false);
		}
	}
	/*******************************************************************************
	* 函数名    : pie
	* 描述      : 绘制一个圆盘
	* 参数      : color颜色,r半径,x,y圆心坐标
	* 返回      : -
	* 编写者    : YSZ
	* 编写日期  : 2021-03-31
	*******************************************************************************/
	void yWidget::pie(color_t color,int r,int x,int y)
	{
		auto isInCircle = [&](int xpos,int ypos) {
			int xr = xpos - x;
			int yr = ypos - y,t;
			xr = xr * xr + yr * yr;
			yr = r * r;
			t = (r - 1) * (r - 1);
			if(xr == yr || (xr >= t && xr < yr))return 2;
			if(xr < yr)return 1;
			return 0;
		};
		int xStart = x - r, yStart = y - r;
		int xEnd = x + r, yEnd = y + r;
		int t,s;
		for(; yStart < yEnd; yStart++) {
			for(t = xStart; t < xEnd; t++) {
				s = isInCircle(t,yStart);
				if(wgPic->attr.ifFrame && s == 2)LCD_WPixel(t,yStart,WIDGET_FRAME_COLOR);
				else if(s)LCD_WPixel(t,yStart,color);
			}
		}
	}
  #if defined(LCD_USE_EROM)
/*******************************************************************************
* 函数名    : showEBytes
* 描述      : 在指定位置显示图像(数据在外部FLASH中,数据以字节形式保存)
* 返回      : 显示失败返回true
* 编写者    : YSZ
* 编写日期  : 2019-02-05
* 最近改动  : -
* 改动内容  : -
*******************************************************************************/
	void yWidget::showEBytes(int x,int y)
	{
		int xStart = x,yStart = y;
		int xEnd = x + wgPic->xPixel - 1;
		int yEnd = y + wgPic->yPixel - 1;
		if(boundIntersect3(&xStart,&yStart,&xEnd,&yEnd))return;
		DWORD eAddr = wgPic->value;
		eAddr += ((yStart - y) * wgPic->xPixel + xStart - x) << 1;
		int w = xEnd - xStart + 1;
		color_t *pbuff1 = (color_t *)LCD_LBuffer1,*pbuff2 = (color_t *)LCD_LBuffer2;
		bool tf = false;
		#ifdef USE_COLOR32
		EROM_ReadStart(eAddr,LCD_LBuffer1,w << 2);
		#else
		EROM_ReadStart(eAddr,LCD_LBuffer1,w << 1);
		#endif
		while(1)
		{
			while(!EROM_ReadOk()){}
			while(!LCD_BatchOk()){}
			LCD_WArea(xStart,yStart,xEnd,yStart);
			LCD_BatchStart(pbuff1,w,true);
			yStart++;
			if(yStart > yEnd)break;
			eAddr += wgPic->bytesPerLine;
			#ifdef USE_COLOR32
			EROM_ReadStart(eAddr,pbuff2,w << 2);
			#else
			EROM_ReadStart(eAddr,pbuff2,w << 1);
			#endif
			tf = !tf;
			if(tf) {
				pbuff1 = (color_t *)LCD_LBuffer2;
				pbuff2 = (color_t *)LCD_LBuffer1;
			}
			else{
				pbuff1 = (color_t *)LCD_LBuffer1;
				pbuff2 = (color_t *)LCD_LBuffer2;
			}
		}
	}
	
	void yWidget::showBinary(int x,int y,bool erom)
	{
		int xStart,yStart,xEnd,yEnd;
		binaryInfo.x = x;
		binaryInfo.y = y;
		xStart = x;
		xEnd = x + wgPic->xPixel - 1;
		yStart = y;
		yEnd = y + wgPic->yPixel - 1;
		if(boundIntersect3(&xStart,&yStart,&xEnd,&yEnd))return;
		binaryInfo.pic = wgPic;
		if(erom)showED8b(xStart,yStart,xEnd,yEnd,wgPic->value);
		else showID8b(xStart,yStart,xEnd,yEnd,(const BYTE*)wgPic->value);
	}

	static inline uint32_t buffToInt_s(int bytes)
	{
		uint32_t ret = romdataGetOnebyte();
		if(bytes == 1)return ret;
		ret |= romdataGetOnebyte() << 8;
		if(bytes == 2)return ret;
		ret |= romdataGetOnebyte() << 16;
		if(bytes == 3)return ret;
		ret |= romdataGetOnebyte() << 24;
		return ret;
	}
	
	void yWidget::showECompressPic(int x,int y)
	{
		romdataReset(wgPic->value + 2);
		romdataUpdate();
		uint8_t buffChoose = 0;
		int xStart = x,yStart = y;
		int xEnd = x + wgPic->xPixel - 1;
		int yEnd = y + wgPic->yPixel - 1;
		if(boundIntersect3(&xStart,&yStart,&xEnd,&yEnd))return;
		uint16_t *srcsize;
		_NEW_W(srcsize,uint16_t[wgPic->yPixel]);
		uint32_t maxsize = 0;
		uint16_t i,offset = 0;
		for(i = 0; i < wgPic->yPixel; i++){
			srcsize[i] = buffToInt_s(2); //现大小
			buffToInt_s(2); //原大小
			if(srcsize[i] > maxsize)maxsize = srcsize[i];
		}
		uint8_t *tbuff;
		_NEW_W(tbuff,uint8_t[maxsize]);
		color_t *pbuff;
		for(i = 0;y < yStart;y++,i++) {//跳过不显示的行
			for(uint32_t j = 0; j < srcsize[i]; j++)romdataGetOnebyte();
		}
		offset = xStart - x;
		int w = xEnd - xStart + 1,width = wgPic->xPixel * sizeof(color_t);
		while(!LCD_BatchOk()){}
		for(; yStart <= yEnd; yStart++,i++) {
			for(uint32_t j = 0; j < srcsize[i]; j++)tbuff[j] = romdataGetOnebyte();
			if(buffChoose == 2)pbuff = LCD_LBuffer2;
			else pbuff = LCD_LBuffer1;
			x = _decompress(tbuff,pbuff,width);
			if(buffChoose == 1)buffChoose = 2;
			else buffChoose = 1;
			while(!LCD_BatchOk()){}
			LCD_WArea(xStart,yStart,xEnd,yStart);
			LCD_BatchStart(&pbuff[offset],w,true);
		}
		delete[]tbuff;
		delete[]srcsize;
	}
	void yWidget::showECompressPic_t(int x,int y)
	{
		romdataReset(wgPic->value + 2);
		romdataUpdate();
		uint16_t srcsize = buffToInt_s(4);//现大小
		buffToInt_s(4);//原大小
		int xStart = x,yStart = y;
		int xEnd = x + wgPic->xPixel - 1;
		int yEnd = y + wgPic->yPixel - 1;
		uint8_t *tbuff = new uint8_t[srcsize];
		int total = wgPic->xPixel * wgPic->yPixel;
		watchdog_refresh();
		for(uint32_t j = 0; j < srcsize; j++)tbuff[j] = romdataGetOnebyte();//读取数据
		watchdog_refresh();
		while(!LCD_BatchOk()){}
		_decompress(tbuff,LCD_TOTAL_BUFF,total * sizeof(color_t));
		delete[]tbuff;
		if(wgPic->attr.black_tf) {
			color_t *p = LCD_TOTAL_BUFF;
			for(; yStart <= yEnd; yStart++){
				LCD_WArea(xStart,yStart,xEnd,yStart);
				for(int x = xStart; x <= xEnd; p++){
					x++;
					if(*p == 0x0000){
						LCD_WArea(x,yStart,xEnd,yStart);
					}
					else {
						LCD_WData(*p);
					}
				}
			}
		}
		else {
			LCD_WArea(xStart,yStart,xEnd,yEnd);
			LCD_BatchSyn(LCD_TOTAL_BUFF,total,true);
		}
	}
#endif
/*******************************************************************************
* 函数名    : showID16m
* 描述      : 在指定位置显示图像(数据在内部FLASH中,数据以字节形式保存)
* 返回      : 显示失败返回true
* 编写者    : YSZ
* 编写日期  : 2019-02-05
* 最近改动  : -
* 改动内容  : -
*******************************************************************************/
	void yWidget::showIBytes(int x,int y)
	{
		int xStart = x, yStart = y;
		int xEnd = x + wgPic->xPixel - 1;
		int yEnd = y + wgPic->yPixel - 1;
		if(boundIntersect3(&xStart,&yStart,&xEnd,&yEnd))return;//求本图像范围与外可视范围及本widget三方的交集
		const BYTE* iAddr = (const BYTE*)wgPic->value;
		#ifdef USE_COLOR32
			iAddr += ((yStart - y) * wgPic->xPixel + xStart - x) << 2;//跳过不可见的行和不可见的列,32位图一像素4字节,故乘以4
		#else
			iAddr += ((yStart - y) * wgPic->xPixel + xStart - x) << 1;
		#endif
		int w = xEnd - xStart + 1;
		while(1)
		{
			while(!LCD_BatchOk()){}
			LCD_WArea(xStart,yStart,xEnd,yStart);//定位
			LCD_BatchStart(iAddr,w,true);
			yStart++;
			if(yStart > yEnd)break;
			iAddr += wgPic->bytesPerLine;
		}
	}
	
	/*******************************************************************************
	* 函数名		: showED8b
	* 描述			: 显示EFLASH中含有的二位图
	* 返回			: -
	* 编写者		: YSZ
	* 编写日期	: 2019-02-04
	* 最近改动	: -
	* 改动内容	: -
	*******************************************************************************/
#if defined(LCD_USE_EROM) //存在外部FLASH时,启用
	void yWidget::showED8b(int xStart,int yStart,int xEnd,int yEnd,DWORD eAddr)
	{
		BYTE ch,t,*tBuff = (BYTE*)LCD_LBuffer2,r;
		if(yEnd < binaryInfo.y || xEnd < binaryInfo.x)return;
		int n = xStart - binaryInfo.x;//不可见的像素长度
		if(n > 0){
			r = n & 0x07;//字形码除n个字节不可见外,有r位也不可见,余数就是不可见的位
			n /= 8;//每个字节8位代表8个像素,将不可见的像素除以8,即得不可见的字节
		}
		else {
			r = 0;
			n = 0;
			xStart = binaryInfo.x;
		}
		if(yStart > binaryInfo.y){
			n = ((yStart - binaryInfo.y) * binaryInfo.pic->bytesPerLine) + n;//字形码跳过不可见的yStart-binaryInfo.y行,再跳过不可见的字节
		}
		else yStart = binaryInfo.y;
		while(!LCD_BatchOk()){}
		eAddr += n;
		for(;yStart <= yEnd; yStart++)
		{
			EROM_ReadStart(eAddr, tBuff, binaryInfo.pic->bytesPerLine);//读取一行字行码
			while(!EROM_ReadOk()){}
			ch = tBuff[0] << r;//移除不显示的位
			t = r;
			n = 0;
			LCD_WArea(xStart,yStart,xEnd,yStart);
			for(int ii = xStart;ii <= xEnd;)//显示一行
			{
				for(;t < 8 && ii <= xEnd; t++)
				{
					ii++;
					if(ch & 0x80){
						LCD_WData(binaryInfo.fr);
					}
					else{
						if(binaryInfo.ifOpaque){
							LCD_WData(binaryInfo.bg);
						}
						else {
							LCD_WArea(ii, yStart, xEnd, yStart);
						}
					}
					ch <<= 1;
				}
				t = 0;
				ch = tBuff[++n];//下一个字节
			}
			eAddr += binaryInfo.pic->bytesPerLine;
		}
		binaryInfo.x = xEnd + 1;
	}
	void yWidget::getPureED8bScale(int& xStart,int& xEnd,uint32_t eAddr)
	{
		
		int n;
		int w = binaryInfo.pic->xPixel;
		int xmin = w;
		int xmax = 0;
		BYTE* tBuff = (BYTE*)LCD_LBuffer2,ch;
		for(int y = 0;y < binaryInfo.pic->yPixel; y++)
		{
			EROM_ReadStart(eAddr, tBuff, binaryInfo.pic->bytesPerLine);//读取一行字行码
			while(!EROM_ReadOk()){}
			n = 0;
			for(int ii = 0;ii < w;)//显示一行
			{
				ch = tBuff[n];//下一个字节
				for(int t = 0;t < 8 && ii < w; t++, ii++)
				{
					if(ch & 0x80){
						if(xmax < ii)xmax = ii;
						if(xmin > ii)xmin = ii;
					}
					ch <<= 1;
				}
				n++;
			}
			eAddr += binaryInfo.pic->bytesPerLine;
		}
		xStart = binaryInfo.x;
		if(xmin > xmax) {
			xEnd = xStart + w / 2 - 1;
		}
		else {
			binaryInfo.x -= xmin;
			xEnd = xStart + xmax - xmin;
		}
	}
#endif
	/*******************************************************************************
	* 函数名		: showID8b
	* 描述			: 显示IFLASH中含有的二图
	* 返回			: -
	* 编写者		: YSZ
	* 编写日期	: 2019-02-04
	* 最近改动	: -
	* 改动内容	: -
	*******************************************************************************/	
	void yWidget::showID8b(
	int xStart,int yStart,int xEnd,int yEnd,//可视范围参数
	const BYTE *idat)
	{
		BYTE ch,t,r;
		if(yEnd < binaryInfo.y || xEnd < binaryInfo.x)return;
		int n = xStart - binaryInfo.x;//不可见的像素长度
		if(n > 0){
			r = n & 0x07;//字形码除n个字节不可见外,有r位也不可见,余数就是不可见的位
			n /= 8;//每个字节8位代表8个像素,将不可见的像素除以8,即得不可见的字节
		}
		else {
			r = 0;
			n = 0;
			xStart = binaryInfo.x;
		}
		if(yStart > binaryInfo.y){
			n = ((yStart - binaryInfo.y) * binaryInfo.pic->bytesPerLine) + n;//字形码跳过不可见的yStart-binaryInfo.y行,再跳过不可见的字节
		}
		else yStart = binaryInfo.y;
		while(!LCD_BatchOk()){}
		idat = idat + n;
		for(;yStart <= yEnd; yStart++)
		{
			ch = idat[0] << r;//移除不显示的位
			t = r;
			n = 0;
			LCD_WArea(xStart,yStart,xEnd,yStart);
			for(int ii = xStart;ii <= xEnd;)//显示一行
			{
				for(;t < 8 && ii <= xEnd; t++)
				{
					ii++;
					if(ch & 0x80){
						LCD_WData(binaryInfo.fr);
					}
					else {
						if(binaryInfo.ifOpaque){
							LCD_WData(binaryInfo.bg);
						}
						else {
							LCD_WArea(ii,yStart,xEnd,yStart);
						}
					}
					ch <<= 1;
				}
				t = 0;
				n++;
				ch = idat[n];//下一个字节
			}
			idat += binaryInfo.pic->bytesPerLine;
		}
		binaryInfo.x = xEnd + 1;
	}
	void yWidget::getPureID8bScale(int& xStart,int& xEnd,const BYTE* idat)
	{
		BYTE ch;
		int n;
		int w = binaryInfo.pic->xPixel;
		int xmin = w;
		int xmax = 0;
		for(int y = 0;y < binaryInfo.pic->yPixel; y++)
		{
			n = 0;
			for(int ii = 0;ii < w;)//显示一行
			{
				ch = idat[n];//下一个字节
				for(int t = 0; t < 8 && ii < w; t++, ii++)
				{
					if(ch & 0x80){
						if(xmax < ii)xmax = ii;
						if(xmin > ii)xmin = ii;
					}
					ch <<= 1;
				}
				n++;
			}
			idat += binaryInfo.pic->bytesPerLine;
		}
		xStart = binaryInfo.x;
		if(xmin > xmax) {
			xEnd = xStart + w / 2 - 1;
		}
		else {
			binaryInfo.x -= xmin;
			xEnd = xStart + xmax - xmin;
		}
	}

