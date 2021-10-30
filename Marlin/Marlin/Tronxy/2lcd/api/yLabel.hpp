#pragma once

#include "yWidget.hpp"
#include "../picdata/yFonts.h"
#define DEFAULT_AFONT Font_En8x16Cambria
#define DEFAULT_UFONT Font_U16x16Songti
#define DEFAULT_FONT_XPIXEL  8
#define DEFAULT_FONT_YPIXEL  16
class yLabel:public yWidget
{
	public:
		yLabel(int xStart = 0,int yStart = 0,int width = 0,int height = 0,
		const PICinfo*picIlde = 0,const PICinfo*picFocus = 0,bool enfocus = true);
		virtual ~yLabel();
		virtual inline bool update(bool force = false){return updateText(force,yWidget::update(force));}
		inline void setWPos(int x,int y){wxOffset = x;wyOffset = y;}
		inline virtual void setfocus(bool tf){
			if(enFocus){
				if(tf && !focus){
					binaryInfo.fr = fr_focus;
					binaryInfo.bg = bg_focus;
					if(updateTextIfChange)sign.content.textCurrent = 1;
				}
				else if(!tf && focus) {
					binaryInfo.fr = fr_idle;
					binaryInfo.bg = bg_idle;
					if(updateTextIfChange)sign.content.textCurrent = 1;
				}
			}
			yWidget::setfocus(tf);
		}
		void setCBuffer(uint16_t size,const char*content = nullptr); //设置buffer空间,如果size为0,则据content来判定buffer大小
		void setWBuffer(uint16_t size,const WORD*content = nullptr);
		void setChars(const char* src);
		void setChars(const WORD *src);

	public:
		bool _fillMargin,updateTextIfChange;
		BYTE textDistance;
		BYTE u16_mode;//0,ASCII,1,utf16,2mul unit
		uint16_t char_size,word_size;
		const PICinfo *wEnPic,*wCnPic;
		char *char_tar;
		WORD *word_tar;
	protected:
		int wxOffset,wyOffset;
		bool putc(WORD c);//在调用putc前需要指定英文字库和中文字库(如果有中文的话)
		void puts(const char* tar);//显示字符串(单行)
		void putn(int n);//整数
		void putf(float f,char dotBits);//浮点数
		void fillMargin();
		bool updateText(bool force,bool w);
};
