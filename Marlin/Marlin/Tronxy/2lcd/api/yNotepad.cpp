#include "yNotepad.hpp"
yNotepad::yNotepad(int xStart,int yStart,int width,int height,const PICinfo*picIdle,const PICinfo*picFocus,bool enfocus)
	:yLabel(xStart,yStart,width,height,picIdle,picFocus,enfocus)
{
	Column = 0;
	Row = 0;
	Pads = 0;
}
yNotepad::~yNotepad()
{
	freePad();
}
bool yNotepad::update(bool force)
{
  	bool tf = yWidget::update(force);
	if(!Pads)return tf;
	if(sign.content.text || force)
	{
		// sign.content.text = 0; //由窗口管理者统一失效
		int highTotal = 0,yStart,n;
		int winHeight = w_yend - w_ystart;
		for(n = 0;n < Row;n++)highTotal += Pads[n].lineHighest;
		if(winHeight > highTotal)yStart = w_ystart + 1;//文字没有超过窗口,则顶页显示
		else yStart = w_ystart - (highTotal - winHeight) + 1;//计算出整页文字超出窗口部分
		n = curRow;
		do {
			if(++n >= Row)n = 0;
			if(Pads[n].lineHighest > 0)//只显示有文字的部分
			{
				binaryInfo.x = w_xstart + 1;
				binaryInfo.y = yStart;
				_fillMargin = binaryInfo.ifOpaque;
				for(const WORD* t = Pads[n].word;*t;putc(*t++));
				yStart += Pads[n].lineHighest;
			}
		} while(n != curRow);//本行是最后一行显示
    	if(yStart < w_yend && binaryInfo.ifOpaque)
		rect(binaryInfo.bg,w_xstart + 1,yStart,w_xend - 1,w_yend - 1);
	}
	return tf;
}

	/*******************************************************************************
	* 函数名		: freePad
	* 描述			: 释放面缓冲空间
	* 返回			: -
	* 编写者		: YSZ
	* 编写日期	: 2019-02-08
	* 最近改动	: -
	* 改动内容	: -
	*******************************************************************************/
	void yNotepad::freePad()
	{
		if(Pads)
		{
			for(int i = 0;i < Row;i++)
			if(Pads[i].word)delete[]Pads[i].word;
			delete[]Pads;
			Pads = 0;
		}
	}
	/*******************************************************************************
	* 函数名		: allocPad
	* 描述			: 分配面缓冲空间
	* 返回			: -
	* 编写者		: YSZ
	* 编写日期	: 2019-02-08
	* 最近改动	: -
	* 改动内容	: -
	*******************************************************************************/
	void yNotepad::allocPad()
	{
		if(Column && Row)
		{
			_NEW_W(Pads,PADS[Row]);
			for(int i = 0;i < Row; i++)
			{
				Pads[i].lineHighest = 0;
				_NEW_W(Pads[i].word,WORD[Column + 1]);
			}
			curRow = 0;
			curColumn = 0;
			curXPixel = w_xstart;
		}
		else Pads = 0;
		sign.content.text = 1;
	}
	/*******************************************************************************
	* 函数名		: setBuffSpace
	* 描述			: 设置面缓冲大小,并分配空间
	* 返回			: -
	* 编写者		: YSZ
	* 编写日期	: 2019-02-08
	* 最近改动	: -
	* 改动内容	: -
	*******************************************************************************/
	void yNotepad::setBuffSpace(int row,int column)
	{
		u16_mode = 1;
		freePad();
		Column = column;
		Row = row;
		allocPad();
	}
	/*******************************************************************************
	* 函数名		: lineFeed
	* 描述			: 换行
	* 返回			: -
	* 编写者		: YSZ
	* 编写日期	: 2019-02-08
	* 最近改动	: -
	* 改动内容	: -
	*******************************************************************************/
	void yNotepad::lineFeed()
	{
		Pads[curRow].word[curColumn] = 0;
		curRow++;
		if(curRow >= Row)curRow = 0;
		curColumn = 0;
		curXPixel = w_xstart;
		Pads[curRow].lineHighest = 0;
	}
	/*******************************************************************************
	* 函数名		: addChars
	* 描述			: 向面板中加入字符串
	* 返回			: -
	* 编写者		: YSZ
	* 编写日期	: 2019-02-08
	* 最近改动	: -
	* 改动内容	: -
	*******************************************************************************/
void yNotepad::addChars(const char*str)
{
	if(!Pads || str == nullptr || *str == 0 || wEnPic == nullptr)return;
	while(*str) {
		if(!(*str & 0x80))
		{
			if((curXPixel + wEnPic->xPixel) > w_xend - 1 || curColumn >= Column - 1){
				lineFeed();
				if(*str == '\n')str++;//刚好遇到还行符
			}
			if(*str == '\n')lineFeed();
			else {
				Pads[curRow].word[curColumn] = *str;
				curColumn++;
				if(Pads[curRow].lineHighest < wEnPic->yPixel)Pads[curRow].lineHighest = wEnPic->yPixel;
				curXPixel += wEnPic->xPixel;
			}
		}
		str++;
	}
	Pads[curRow].word[curColumn] = 0;
	sign.content.text = 1;
}
void yNotepad::addChars(const WORD*tar)
{
	if(!Pads || tar == nullptr || *tar == 0 || wCnPic == nullptr)return;
	int xEnd;
	DWORD eAddr;
	while(*tar)
	{
		if(*tar > 0x80)
		{
			if(wCnPic)
			{
				if(textDistance < 100) {
					WORD c = *tar;
					binaryInfo.pic = wCnPic;
					binaryInfo.x = curXPixel;
					if(u16_mode) {
						if(c < 0x4E00 || c > 0x9FBB)continue;
						eAddr = binaryInfo.pic->value + wCnPic->vSpace * (c - 0x4E00);
					}
					else {
						int area = ((c >> 8) & 0xFF);
						area -= 0xB0;//区号:字库只存一级汉字,所以从B0开始
						int pos = (c & 0xFF) - 0xA1;//位号,汉字都是从A1开始的
						if(area > 39 || pos > 94)continue; //一级汉字是16--55共40个区
						eAddr = binaryInfo.pic->value + wCnPic->vSpace * ((area * 94) + pos);
					}
					if(wCnPic->attr.ifERom)getPureED8bScale(curXPixel,xEnd,eAddr);
					else getPureID8bScale(curXPixel,xEnd,(const BYTE*)eAddr);
					curXPixel = xEnd + textDistance + 1;
				}
				else curXPixel += wCnPic->xPixel;
				if(curXPixel > w_xend - 1 || curColumn >= Column - 1){
					lineFeed();
				}
				Pads[curRow].word[curColumn++] = *tar;
				if(Pads[curRow].lineHighest < wCnPic->yPixel)Pads[curRow].lineHighest = wCnPic->yPixel;
			}
		}
		else
		{
			if(wEnPic)
			{
				if(textDistance < 100){
					binaryInfo.pic = wEnPic;
					binaryInfo.x = curXPixel;
					eAddr = binaryInfo.pic->value + (*tar - binaryInfo.pic->zero) * wEnPic->vSpace;
					getPureID8bScale(curXPixel,xEnd,(const BYTE*)eAddr);
					curXPixel = xEnd + textDistance + 1;
				}
				else curXPixel += wEnPic->xPixel;
				if(curXPixel > w_xend - 1 || curColumn >= Column - 1 || *tar == '\n'){
					lineFeed();
				}
				if(*tar != '\n')
				{
					Pads[curRow].word[curColumn++] = *tar;
					if(Pads[curRow].lineHighest < wEnPic->yPixel)Pads[curRow].lineHighest = wEnPic->yPixel;
				}
			}
		}
		tar++;
	}
	Pads[curRow].word[curColumn] = 0;
	sign.content.text = 1;
}
