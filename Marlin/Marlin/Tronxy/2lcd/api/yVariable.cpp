#include "yVariable.hpp"
yVariable::yVariable(int xStart,int yStart,int width,int height,const PICinfo*picIdle,const PICinfo*picFocus,bool enfocus)
	:yLabel(xStart,yStart,width,height,picIdle,picFocus,enfocus)
{
	horizon = true;
	fixedEdge = false;
	tail_char = 0;
}
bool yVariable::update(bool force)
{
	if(wgPic && (sign.content.widget || force)){
		switch(wgPic->attr.type) {
		case TYPE_COLOR:
			if(horizon)horizonRect();
			else verticalRect();
			break;
		case TYPE_BINARY:
			if(wgPic->attr.ifERom){}
			else {
				if(horizon)horizonIBinary();
				else verticalIBinary();
			}
			break;
		default:
			yWidget::update(false);
			break;
		}
	}
	bool tf = updateText(force,false);
	if(!tf && u16_mode != 2 && (sign.content.text || force))putc(tail_char);
	return tf;
}
void yVariable::horizonRect()
{
	if(pic_color1 == pic_color2)yWidget::rect(pic_color1,w_xstart,w_ystart,w_xend,w_yend);
	else{
		int xStart,yStart,xEnd,yEnd;
		if(boundIntersect(xStart,yStart,xEnd,yEnd))return;
		WORD cR3,cG3,cB3;
		int startR,startG,startB;
		int endR,endG,endB;
		if(fixedEdge){
			ColorToRGB(pic_color2,startR,startG,startB);
			ColorToRGB(pic_color1,endR,endG,endB);
		}
		else {
			ColorToRGB(pic_color1,startR,startG,startB);
			ColorToRGB(pic_color2,endR,endG,endB);
		}
		int disR = endR - startR;
		int disG = endG - startG;
		int disB = endB - startB;
		int width = w_xend - w_xstart + 1;
		int w = xEnd - xStart + 1,xpos,len,i;
		if(wgPic->zero > 0){
			WORD cR4,cG4,cB4;
			int ratio = 256 - wgPic->zero;
			do
			{
				while(!LCD_BatchOk()){}
				for(xpos = xStart, i = 0; xpos <= xEnd; xpos++, i++)
				{
					len = xpos - w_xstart;
					cR3 = (len * disR + startR * width) / width;
					cG3 = (len * disG + startG * width) / width;
					cB3 = (len * disB + startB * width) / width;
					LCD_RPixel(xpos, yStart, cR4, cG4, cB4);
					cR4 = (cR3 * ratio + cR4 * wgPic->zero + 128) >> 8;
					cG4 = (cG3 * ratio + cG4 * wgPic->zero + 128) >> 8;
					cB4 = (cB3 * ratio + cB4 * wgPic->zero + 128) >> 8;
					LCD_LBuffer2[i] = RGBToColor(cR4, cG4, cB4);
				}
				LCD_WArea(xStart,yStart,xEnd,yStart);
				LCD_BatchStart(LCD_LBuffer2,w,true);
			}while(++yStart <= yEnd);
		}
		else {
			do{
				while(!LCD_BatchOk()){}
				for(xpos = xStart, i = 0; xpos <= xEnd; xpos++, i++){
					len = xpos - w_xstart;
					cR3 = (len * disR + startR * width) / width;
					cG3 = (len * disG + startG * width) / width;
					cB3 = (len * disB + startB * width) / width;
					LCD_LBuffer2[i] = RGBToColor(cR3, cG3, cB3);
				}
				LCD_WArea(xStart,yStart,xEnd,yStart);
				LCD_BatchStart(LCD_LBuffer2,w,true);
			}while(++yStart <= yEnd);
		}
	}
}
void yVariable::horizonIBinary()
{
	int xStart = w_xstart + pxOffset,yStart = w_ystart + pyOffset;
	binaryInfo.x = xStart;
	binaryInfo.y = yStart;
	int xEnd = xStart + wgPic->xPixel - 1;
	int yEnd = yStart + wgPic->yPixel - 1;
	if(boundIntersect3(&xStart,&yStart,&xEnd,&yEnd))return;//求本图像范围与外可视范围及本widget三方的交集
	if(pic_color1 == pic_color2){
		BYTE opaque = binaryInfo.ifOpaque;
		color_t tcolor = binaryInfo.fr;
		const PICinfo *tpic = binaryInfo.pic;
		binaryInfo.ifOpaque = 0;//显示二元图时,必须为0
		binaryInfo.fr = pic_color1;
		binaryInfo.pic = wgPic;
		showID8b(xStart,yStart,xEnd,yEnd,(const BYTE*)wgPic->value);
		binaryInfo.ifOpaque = opaque;
		binaryInfo.fr = tcolor;
		binaryInfo.pic = tpic;
	}
	else{
		BYTE ch,t;
		WORD cR3,cG3,cB3,cR4,cG4,cB4;
		int startR,startG,startB;
		int endR,endG,endB;
		if(fixedEdge){
			ColorToRGB(pic_color2,startR,startG,startB);
			ColorToRGB(pic_color1,endR,endG,endB);
		}
		else {
			ColorToRGB(pic_color1,startR,startG,startB);
			ColorToRGB(pic_color2,endR,endG,endB);
		}
		int disR = endR - startR;
		int disG = endG - startG;
		int disB = endB - startB;
		int x,len,ratio = 256 - wgPic->zero;
		int n = xStart - binaryInfo.x;//不可见的像素长度
		const BYTE r = n & 0x07;//字形码除n个字节不可见外,有r位也不可见,余数就是不可见的位
		n /= 8;//每个字节8位代表8个像素,将不可见的像素除以8,即得不可见的字节
		int jump = ((yStart - binaryInfo.y) * wgPic->bytesPerLine) + n;//字形码跳过不可见的yStart-binaryInfo.y行,再跳过不可见的字节
		const BYTE* idat = (const BYTE*)wgPic->value + jump;
		while(!LCD_BatchOk()){}	
		do
		{
			ch = idat[0] << r;//移除不显示的位
			t = r;
			n = 0;
			LCD_WArea(xStart,yStart,xEnd,yStart);
			for(x = xStart;x <= xEnd;)//显示一行
			{
				for(;t < 8 && x <= xEnd; t++)
				{
					len = x - binaryInfo.x;
					x++;
					if(ch & 0x80){
						cR3 = (len * disR + startR * wgPic->xPixel) / wgPic->xPixel;
						cG3 = (len * disG + startG * wgPic->xPixel) / wgPic->xPixel;
						cB3 = (len * disB + startB * wgPic->xPixel) / wgPic->xPixel;
						if(wgPic->zero > 0){
							LCD_RPixel(x, yStart, cR4, cG4, cB4);
							cR4 = (cR3 * ratio + cR4 * wgPic->zero + 128) >> 8;
							cG4 = (cG3 * ratio + cG4 * wgPic->zero + 128) >> 8;
							cB4 = (cB3 * ratio + cB4 * wgPic->zero + 128) >> 8;
							LCD_LBuffer2[0] = RGBToColor(cR4, cG4, cB4);
						}
						else LCD_LBuffer2[0] = RGBToColor(cR3, cG3, cB3);
						LCD_WData(LCD_LBuffer2[0]);
					}
					else {
						LCD_WArea(x,yStart,xEnd,yStart);
					}
					ch <<= 1;
				}
				t = 0;
				n++;
				ch = idat[n];//下一个字节
			}
			idat += wgPic->bytesPerLine;
		}while(++yStart <= yEnd);
	}
}
void yVariable::verticalRect()
{
	if(pic_color1 == pic_color2)yWidget::rect(pic_color1,w_xstart,w_ystart,w_xend,w_yend);
	else{
		int xStart,yStart,xEnd,yEnd;
		if(boundIntersect(xStart,yStart,xEnd,yEnd))return;
		WORD cR3,cG3,cB3;
		int startR,startG,startB;
		int endR,endG,endB;
		if(fixedEdge){
			ColorToRGB(pic_color2,startR,startG,startB);
			ColorToRGB(pic_color1,endR,endG,endB);
		}
		else {
			ColorToRGB(pic_color1,startR,startG,startB);
			ColorToRGB(pic_color2,endR,endG,endB);
		}
		int disR = endR - startR;
		int disG = endG - startG;
		int disB = endB - startB;
		int h = w_yend - w_ystart + 1;
		int w = xEnd - xStart + 1,len;
		if(wgPic->zero > 0) {
			WORD cR4,cG4,cB4;
			int xpos,ratio = 256 - wgPic->zero,i;
			do
			{
				len = yStart - w_ystart;
				cR3=(len * disR + startR * h) / h;
				cG3=(len * disG + startG * h) / h;
				cB3=(len * disB + startB * h) / h;
				while(!LCD_BatchOk()){}
				for(xpos = xStart,i = 0; i < w; xpos++,i++) {
					LCD_RPixel(xpos, yStart, cR4, cG4, cB4);
					cR4=(cR3 * ratio + cR4 * wgPic->zero + 128) >> 8;
					cG4=(cG3 * ratio + cG4 * wgPic->zero + 128) >> 8;
					cB4=(cB3 * ratio + cB4 * wgPic->zero + 128) >> 8;
					LCD_LBuffer2[i] = RGBToColor(cR4,cG4,cB4);
				}
				LCD_WArea(xStart,yStart,xEnd,yStart);
				LCD_BatchStart(LCD_LBuffer2,w,true);
			}while(++yStart <= yEnd);
		}
		else{
			do
			{
				len = yStart - w_ystart;
				cR3=(len * disR + startR * h) / h;
				cG3=(len * disG + startG * h) / h;
				cB3=(len * disB + startB * h) / h;
				while(!LCD_BatchOk()){}
				LCD_LBuffer2[0] = RGBToColor(cR3,cG3,cB3);
				LCD_WArea(xStart,yStart,xEnd,yStart);
				LCD_BatchStart(LCD_LBuffer2,w,false);
			}
			while(++yStart <= yEnd);
		}
	}
}
void yVariable::verticalIBinary()
{
	int xStart = w_xstart + pxOffset,yStart = w_ystart + pyOffset;
	binaryInfo.x = xStart;
	binaryInfo.y = yStart;
	int xEnd = xStart + wgPic->xPixel - 1;
	int yEnd = yStart + wgPic->yPixel - 1;
	if(boundIntersect3(&xStart,&yStart,&xEnd,&yEnd))return;//求本图像范围与外可视范围及本widget三方的交集
	if(pic_color1 == pic_color2) {
		BYTE opaque = binaryInfo.ifOpaque;
		color_t tcolor = binaryInfo.fr;
		const PICinfo *tpic = binaryInfo.pic;
		binaryInfo.ifOpaque = 0;//显示二元图时,必须为0
		binaryInfo.fr = pic_color1;
		binaryInfo.pic = wgPic;
		showID8b(xStart,yStart,xEnd,yEnd,(const BYTE*)wgPic->value);
		binaryInfo.ifOpaque = opaque;
		binaryInfo.fr = tcolor;
		binaryInfo.pic = tpic;
	}
	else{
		BYTE ch,t;
		WORD cR3,cG3,cB3,cR4,cG4,cB4;
		int startR,startG,startB;
		int endR,endG,endB;
		if(fixedEdge){
			ColorToRGB(pic_color2,startR,startG,startB);
			ColorToRGB(pic_color1,endR,endG,endB);
		}
		else {
			ColorToRGB(pic_color1,startR,startG,startB);
			ColorToRGB(pic_color2,endR,endG,endB);
		}
		int disR = endR - startR;
		int disG = endG - startG;
		int disB = endB - startB;
		int len,x,ratio = 256 - wgPic->zero;
		int n = xStart - binaryInfo.x;//不可见的像素长度
		const BYTE r = n & 0x07;//字形码除n个字节不可见外,有r位也不可见,余数就是不可见的位
		n /= 8;//每个字节8位代表8个像素,将不可见的像素除以8,即得不可见的字节
		int jump = ((yStart - binaryInfo.y) * wgPic->bytesPerLine) + n;//字形码跳过不可见的yStart-binaryInfo.y行,再跳过不可见的字节
		const BYTE* idat = (const BYTE*)wgPic->value + jump;
		while(!LCD_BatchOk()){}
		do
		{
			ch = idat[0] << r;//移除不显示的位
			t = r;
			n = 0;
			len = yStart - binaryInfo.y;
			cR3 = (len * disR + startR * wgPic->yPixel) / wgPic->yPixel;
			cG3 = (len * disG + startG * wgPic->yPixel) / wgPic->yPixel;
			cB3 = (len * disB + startB * wgPic->yPixel) / wgPic->yPixel;
			LCD_WArea(xStart,yStart,xEnd,yStart);
			for(x = xStart;x <= xEnd;)//显示一行
			{
				for(;t < 8 && x <= xEnd; t++)
				{
					x++;
					if(ch & 0x80){
						if(wgPic->zero > 0){
							LCD_RPixel(x, yStart, cR4, cG4, cB4);
							cR4 = (cR3 * ratio + cR4 * wgPic->zero + 128) >> 8;
							cG4 = (cG3 * ratio + cG4 * wgPic->zero + 128) >> 8;
							cB4 = (cB3 * ratio + cB4 * wgPic->zero + 128) >> 8;
							LCD_LBuffer2[0] = RGBToColor(cR4, cG4, cB4);
						}
						else LCD_LBuffer2[0] = RGBToColor(cR3, cG3, cB3);
						LCD_WData(LCD_LBuffer2[0]);
					}
					else {
						LCD_WArea(x,yStart,xEnd,yStart);
					}
					ch <<= 1;
				}
				t = 0;
				n++;
				ch = idat[n];//下一个字节
			}
			idat += wgPic->bytesPerLine;
		}while(++yStart <= yEnd);
	}
}
void yVariable::changeBorder(int v)
{
	if(horizon){
		if(fixedEdge){
			w_xstart += v;
			wxOffset -= v;
		}
		else w_xend += v;
	}
	else{
		if(fixedEdge)w_yend += v;
		else {
			w_ystart += v;
			wyOffset -= v;
		}
	}
	sign.content.widget = 1;
}
