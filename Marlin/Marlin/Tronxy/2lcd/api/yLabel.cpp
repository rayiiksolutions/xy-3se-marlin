#include "yLabel.hpp"

yLabel::yLabel(int xStart,int yStart,int width,int height,const PICinfo*picIdle,const PICinfo* picFocus,bool enfocus)
	:yWidget(xStart,yStart,width,height,picIdle,picFocus,enfocus),
wEnPic(&DEFAULT_AFONT),wCnPic(&DEFAULT_UFONT)	//默认文字字形库
{
	wxOffset = 1;
	wyOffset = 1;
	sign.content.textCurrent = 0;
	_fillMargin = false;
	u16_mode = 0;
	updateTextIfChange = false;
	textDistance = 100;
	char_size = 0;
	char_tar = nullptr;
	word_size = 0;
	word_tar = nullptr;
}
yLabel::~yLabel()
{
	if(char_tar) {
		delete[]char_tar;
	}
	if(word_tar){
		delete[]word_tar;
	}
}
	/*******************************************************************************
	* 函数名		: updateText
	* 描述			: 屏幕更新文字显示
	* 返回			: -
	* 编写者		: YSZ
	* 编写日期	: 20201111
	* 最近改动	: -
	* 改动内容	: -
	*******************************************************************************/
	bool yLabel::updateText(bool force,bool w){
		if(sign.content.textCurrent){
			sign.content.textCurrent = 0;
			sign.content.text = 1;
			if(w)return true;
		}
		if(sign.content.text || force) {
			// sign.content.text = 0; //由窗口管理者统一失效
			binaryInfo.x = w_xstart + wxOffset;
			binaryInfo.y = w_ystart + wyOffset;
			switch(u16_mode) {
			case 0://纯ASCII
				if(char_tar) {
					puts(char_tar);
					fillMargin();
				}
				break;
			case 1://utf16
				if(word_tar)
				{
					for(const WORD* t = word_tar;*t;putc(*t++));
					fillMargin();
				}
				break;
			}
		}
		return w;
	}
	/*******************************************************************************
	* 函数名		: setText
	* 描述			: 更换Label的字符串
	* 返回			: -
	* 编写者		: YSZ
	* 编写日期	: 2019-02-06
	* 最近改动	: -
	* 改动内容	: -
	*******************************************************************************/
	void yLabel::setCBuffer(uint16_t size,const char*str)
	{
		if(char_tar){
			delete[]char_tar;
		}
		if(size == 0 && str == nullptr)char_tar = nullptr;
		else {
			if(size == 0){
				const char*p = str;
				while(*p++)size++;
			}
			char_tar = new char[size + 1];
			char_size = size;
			if(str){
				int i = 0;
				while(*str && i < char_size){
					char_tar[i] = *str;
					str++;
					i++;
				}
				char_tar[i] = 0;
			}
			else char_tar[0] = 0;
			sign.content.text = 1;
			u16_mode = 0;
		}
	}
	void yLabel::setChars(const char*str)
	{
		if(char_tar) {
			if(str){
				int i = 0;
				while(*str && i < char_size){
					char_tar[i] = *str;
					str++;
					i++;
				}
				char_tar[i] = 0;
			}
			else char_tar[0] = 0;
			sign.content.text = 1;
		}
		u16_mode = 0;
	}

	void yLabel::setWBuffer(uint16_t size,const WORD*src)
	{
		if(word_tar){
			delete[]word_tar;
		}
		if(size == 0 && src == nullptr)word_tar = nullptr;
		else {
			if(size == 0){
				const WORD*p = src;
				while(*p++)size++;
			}
			word_tar = new WORD[size + 1];
			word_size = size;
			if(src){
				int i = 0;
				while(*src && i < word_size){
					word_tar[i] = *src;
					src++;
					i++;
				}
				word_tar[i] = 0;
			}
			else word_tar[0] = 0;
			sign.content.text = 1;
			u16_mode = 1;
		}
	}
	void yLabel::setChars(const WORD *src)
	{
		if(word_tar) {
			if(src) {
				int i = 0;
				while(*src && i < word_size){
					word_tar[i] = *src;
					src++;
					i++;
				}
				word_tar[i] = 0;
			}
			else word_tar[0] = 0;
			sign.content.text = 1;
		}
		u16_mode = 1;
	}
		
	/*******************************************************************************
	* 函数名		: putc
	* 描述			: 在指定区域显示二色像素块,二色像素的每一个字节代表8个像素
	* 返回			: 显示失败返回true
	* 编写者		: YSZ
	* 编写日期	: 2019-02-04
	* 最近改动	: -
	* 改动内容	: -
	*******************************************************************************/
	bool yLabel::putc(WORD c)//显示的字符
	{
		int xStart,yStart,xEnd,yEnd;
		if(c == 0)return false;
		if(c > 0x80)	//中文
		{
			if(wCnPic == nullptr)return true;
			binaryInfo.pic = wCnPic;
			DWORD eAddr;
			if(u16_mode) {
				if(c < 0x4E00 || c > 0x9FBB)return true;
				eAddr = binaryInfo.pic->value + wCnPic->vSpace * (c - 0x4E00);
			}
			else {
				int area = ((c >> 8) & 0xFF);
				area -= 0xB0;//区号:字库只存一级汉字,所以从B0开始
				int pos = (c & 0xFF) - 0xA1;//位号,汉字都是从A1开始的
				if(area > 39 || pos > 94)return true; //一级汉字是16--55共40个区
				eAddr = binaryInfo.pic->value + wCnPic->vSpace * ((area * 94) + pos);
			}
			if(textDistance < 100){
				getPureED8bScale(xStart,xEnd,eAddr);
				xEnd += textDistance;
			}
			else {
				xStart = binaryInfo.x;
				xEnd = xStart + binaryInfo.pic->xPixel - 1;
			}
			yStart = binaryInfo.y;
			yEnd = yStart + binaryInfo.pic->yPixel - 1;
			w_xend--;//文字显示不超过控件边界
			bool tf = boundIntersect3(&xStart,&yStart,&xEnd,&yEnd);//边界调整
			w_xend++;
			if(tf)
			{
				binaryInfo.x = xEnd + 1;//这里因边界没显示,但是同样跨过了一个字符
				return true;
			}
			if(binaryInfo.pic->attr.ifERom) { //字库在外部FLASH或内部FLASH
				showED8b(xStart,yStart,xEnd,yEnd,eAddr);
			}
			else {
				showID8b(xStart,yStart,xEnd,yEnd,(const BYTE*)eAddr);
			}
		}
		else	//ascii
		{
			if(wEnPic==nullptr)return true;
			binaryInfo.pic = wEnPic;
			DWORD eAddr = binaryInfo.pic->value + (c - binaryInfo.pic->zero) * wEnPic->vSpace;
			if(textDistance < 100){
				getPureID8bScale(xStart,xEnd,(const BYTE*)eAddr);
				xEnd += textDistance;
			}
			else {
				xStart = binaryInfo.x;
				xEnd = xStart + binaryInfo.pic->xPixel - 1;
			}
			yStart = binaryInfo.y;
			yEnd = yStart + binaryInfo.pic->yPixel - 1;
			w_xend--;//文字显示不超过控件边界
			bool tf = boundIntersect3(&xStart,&yStart,&xEnd,&yEnd);//边界调整
			w_xend++;
			if(tf)
			{
				binaryInfo.x = xEnd + 1;//这里因边界没显示,但是同样跨过了一个字符
				return true;
			}
			if(binaryInfo.pic->attr.ifERom){
				showED8b(xStart,yStart,xEnd,yEnd,eAddr);
			}
			else {
				showID8b(xStart,yStart,xEnd,yEnd,(const BYTE*)eAddr);
			}
		}
		return false;
	}
  
	/*******************************************************************************
	* 函数名		: puts
	* 描述			: 单行显行字符串
	* 返回			: -
	* 编写者		: YSZ
	* 编写日期	: 2017-01-02
	* 最近改动	: 2019-02-06
	* 改动内容	: -
	*******************************************************************************/
	void yLabel::puts(const char* target)
	{
		WORD c;
		while(*target)
		{
			c = *target++;
			if(c & 0x80)
			{
				c = (c << 8) | (*target);
				target++;
			}
			putc(c);
		}
	}
	
  /*******************************************************************************
	* 函数名		: fillMargin
	* 描述			: 填充字符串剩余区域
	* 返回			: -
	* 编写者		: YSZ
	* 编写日期	: 2017-01-02
	* 最近改动	: 2019-02-06
	* 改动内容	: -
	*******************************************************************************/
	void yLabel::fillMargin()
	{
		if(binaryInfo.ifOpaque && _fillMargin) {
			rect(binaryInfo.bg,binaryInfo.x,binaryInfo.y,w_xend - 1,binaryInfo.y + binaryInfo.pic->yPixel);
			// binaryInfo.x = w_xend;
		}
	}
	/*******************************************************************************
	* 函数名		: putn
	* 描述			: 显示整数
	* 返回			: -
	* 编写者		: YSZ
	* 编写日期	: 2017-01-02
	* 最近改动	: 2019-02-06
	* 改动内容	: -
	*******************************************************************************/
	void yLabel::putn(int n)
	{
		BYTE ch;
		if(n<0)
		{
			ch='-';
			n=-n;
			putc(ch);
		}
		if(n>=10)
		{
			putn(n/10);
			n%=10;
		}
		ch=n+'0';
		putc(ch);
	}
	
	/*******************************************************************************
	* 函数名		: putf
	* 描述			: 显示浮点数
	* 返回			: -
	* 编写者		: YSZ
	* 编写日期	: 2017-01-02
	* 最近改动	: 2019-02-06
	* 改动内容	: -
	*******************************************************************************/
	void yLabel::putf(float f,//f要显示的数
		char dotBits)//小数位数
	{
		WORD ch;
		int n;
		if(f<0)
		{
			f=-f;
			ch='-';
			putc(ch);
		}
		n=f;
		putn(n);
		ch='.';
		putc(ch);
		f-=n;
		for(;dotBits>0;dotBits--)f*=10;//只显示两位有效小数
		n=f;
		putn(n);
	}
