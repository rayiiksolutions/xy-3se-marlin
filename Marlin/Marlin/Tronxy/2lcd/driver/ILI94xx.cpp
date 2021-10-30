#include "ILI94xx.h"

#if LCD_LMAX > 512
#error LCD_LMAX > 512
#else
color_t LCD_TOTAL_BUFF[1024];
color_t *LCD_LBuffer1 = LCD_TOTAL_BUFF;//LCD_LMAX必须小于512
color_t *LCD_LBuffer2 = &LCD_TOTAL_BUFF[512];
#endif
void ILI94xx_SetDir(unsigned char dir)
{
	LCD_WReg(0x36); //set address mode
	LCD_WData(dir);
}
void ILI94xx_Init(unsigned char dir)
{
	int index = LCD_McuInit();
	if(index < 0)return;
	LCD_Reset();
	LCD_DelayMs(120);
	LCD_WReg(0xE0);
	LCD_WData(0x00);LCD_WData(0x03);LCD_WData(0x0C);LCD_WData(0x09);
	LCD_WData(0x17);LCD_WData(0x09);LCD_WData(0x3E);LCD_WData(0x89);
	LCD_WData(0x49);LCD_WData(0x08);LCD_WData(0x0D);LCD_WData(0x0A);
	LCD_WData(0x13);LCD_WData(0x15);LCD_WData(0x0F);
	LCD_WReg(0XE1); //N-Gamma
	LCD_WData(0x00);LCD_WData(0x11);LCD_WData(0x15);LCD_WData(0x03);
	LCD_WData(0x0F);LCD_WData(0x05);LCD_WData(0x2D);LCD_WData(0x0B);
	LCD_WData(0x0A);LCD_WData(0x33);LCD_WData(0x37);LCD_WData(0x0F);
	LCD_WReg(0XC0); //Power Control 1
	LCD_WData(0x17);LCD_WData(0x15);
	LCD_WReg(0xC1); //Power Control 2
	LCD_WData(0x41); //VGH,VGL
	LCD_WReg(0xC5); //Vcom
	LCD_WData(0x00);LCD_WData(0x12);LCD_WData(0x80);
	LCD_WReg(0x3A); // Interface Pixel Format
	LCD_WData(0x55);
	LCD_WReg(0XB0); // Interface Mode Control
	LCD_WData(0x00);
	LCD_WReg(0xB1); //Frame rate
	LCD_WData(0xB0); //60Hz
	LCD_WData(0x00);
	// LCD_WReg(0xB4); //Display Inversion Control
	// LCD_WData(0x02); //2-dot
	// LCD_WReg(0XB6); //RGB/MCU Interface Control
	// LCD_WData(0x02); //MCU
	// LCD_WData(0x02); //Source,Gate scan dieection
	LCD_WReg(0XE9); // Set Image Function
	LCD_WData(0x00); // Disable 24 bit data input
	// LCD_WReg(0xF7); // Adjust Control
	// LCD_WData(0xA9);
	// LCD_WData(0x51);
	// LCD_WData(0x2C);
	// LCD_WData(0x82); // D7 stream, loose
	LCD_WReg(0x11); //Sleep out
	LCD_DelayMs(120);
	// LCD_WReg(0x21);//反色显示
	LCD_WReg(0x29); //Display on
	ILI94xx_SetDir(dir);//置横屏
}

/*******************************************************************************
* 函数名    ：ILI94xx_rect
* 描述      ：填充一个区域
* 返回      ：-
* 参数      ：color填充的颜色
*          ：x,y左上角坐标
*          ：w,h区域宽高
*          ：transparent透明度
* 编写者    ：YSZ
* 编写日期	：2016-12-13
* 最近改动	：2019-02-02
* 改动内容	：修改透明算法,使速度更快
*******************************************************************************/
void ILI94xx_rect(int x,int y,int w,int h,color_t color,BYTE transparent)
{
	if(transparent > 0)
	{
		WORD cR1,cG1,cB1;
		WORD cR2,cG2,cB2;
		int xpos,i,xend = x + w - 1;
		ColorToRGB(color,cR1,cG1,cB1);
		cR1 *= (256 - transparent);
		cG1 *= (256 - transparent);
		cB1 *= (256 - transparent);
		h += y;
		for(; y < h; y++)
		{
			for(i = 0,xpos = x;i < w;i++, xpos++)
			{
				ILI94xx_RPixel(xpos,y,cR2,cG2,cB2);//只能一个一个的读,批量读实验未成功
				cR2 = (cR1 + cR2 * transparent + 128) >> 8;
				cG2 = (cG1 + cG2 * transparent + 128) >> 8;
				cB2 = (cB1 + cB2 * transparent + 128) >> 8;
				// LCD_LBuffer[i] = ((cB2 >> 11) << 11) | ((cG2 >> 10 ) << 5) | (cR2 >> 11);
				LCD_LBuffer2[i] = RGBToColor(cR2,cG2,cB2);
			}
			ILI94xx_Area(x,y,xend,y);//这里只能area
			ILI94xx_WStart();
			LCD_BatchStart(LCD_LBuffer2,w,true);
			while(!LCD_BatchOk()){}
		}
	}
	else {
		int total = w * h;
		w += x - 1;
		h += y - 1;
		ILI94xx_Area(x,y,w,h);
    	ILI94xx_WStart();
		LCD_BatchSyn(&color,total,false);
	}
}

void ILI94xx_horizontalRect(int x,int y,int w,int h,color_t colorStart,color_t colorEnd,BYTE transparent)
{
	WORD cR3,cG3,cB3;
	int startR,startG,startB;
	int endR,endG,endB;
	ColorToRGB(colorStart,startR,startG,startB);
	ColorToRGB(colorEnd,endR,endG,endB);
	int disR = endR - startR;
	int disG = endG - startG;
	int disB = endB - startB;
	int xend = x + w - 1;
	h += y;
	if(transparent>0)
	{
		WORD cR4,cG4,cB4;
		int i,xpos,ratio = 256 - transparent;
		for(;y < h; y++)
		{
			for(i = 0,xpos = x; i < w; i++, xpos ++)
			{
				cR3 = (i * disR + startR * w) / w;
				cG3 = (i * disG + startG * w) / w;
				cB3 = (i * disB + startB * w) / w;
				ILI94xx_RPixel(xpos, y, cR4, cG4, cB4);
				cR4 = (cR3 * ratio + cR4 * transparent + 128) >> 8;
				cG4 = (cG3 * ratio + cG4 * transparent + 128) >> 8;
				cB4 = (cB3 * ratio + cB4 * transparent + 128) >> 8;
				LCD_LBuffer2[i] = RGBToColor(cR4, cG4, cB4);
			}
			ILI94xx_WArea(x, y, xend, y);
			LCD_BatchStart(LCD_LBuffer2,w,true);
			while(!LCD_BatchOk()){}
		}
	}
	else{
		for(; y < h; y++)
		{
			for(int i = 0; i < w; i++)
			{
				cR3 = (i * disR + startR * w) / w;
				cG3 = (i * disG + startG * w) / w;
				cB3 = (i * disB + startB * w) / w;
				LCD_LBuffer2[i] = RGBToColor(cR3, cG3, cB3);
			}
			ILI94xx_WArea(x, y, xend, y);
			LCD_BatchStart(LCD_LBuffer2,w,true);
			while(!LCD_BatchOk()){}
		}
	}
}

void ILI94xx_verticalRect(int x,int y,int w,int h,color_t colorStart,color_t colorEnd,BYTE transparent)
{
	WORD cR3,cG3,cB3;
	int startR,startG,startB;
	int endR,endG,endB;
	ColorToRGB(colorStart,startR,startG,startB);
	ColorToRGB(colorEnd,endR,endG,endB);
	int disR = endR - startR;
	int disG = endG - startG;
	int disB = endB - startB;
	int xend = x + w - 1;
	if(transparent>0)
	{
		WORD cR4,cG4,cB4;
		int ratio = 255 - transparent,i,xpos;
		for(int j = 0; j < h; j++, y++)
		{
			cR3=(j * disR + startR * h) / h;
			cG3=(j * disG + startG * h) / h;
			cB3=(j * disB + startB * h) / h;
			for(i = 0, xpos = x; i < w; i++, xpos++)
			{
				ILI94xx_RPixel(xpos, y, cR4, cG4, cB4);
				cR4=(cR3 * ratio + cR4 * transparent + 128) >> 8;
				cG4=(cG3 * ratio + cG4 * transparent + 128) >> 8;
				cB4=(cB3 * ratio + cB4 * transparent + 128) >> 8;
				LCD_LBuffer2[i] = RGBToColor(cR4,cG4,cB4);
			}
			ILI94xx_Area(x,y,xend,y);
			ILI94xx_WStart();
			LCD_BatchStart(LCD_LBuffer2,w,true);
			while(!LCD_BatchOk()){}
		}
	}
	else{
		for(int i = 0; i < h; i++,y++)
		{
			cR3=(i * disR + startR * h) / h;
			cG3=(i * disG + startG * h) / h;
			cB3=(i * disB + startB * h) / h;
			LCD_LBuffer2[0] = RGBToColor(cR3,cG3,cB3);
			ILI94xx_Area(x,y,xend,y);
			ILI94xx_WStart();
			LCD_BatchStart(LCD_LBuffer2,w,false);
			while(!LCD_BatchOk()){}
		}
	}
}
/*******************************************************************************
* 函数名		：ILI9488_frame
* 描述			：画一个框
* 返回			：-
* 参数			：cl框的颜色
*						：x,y左上角坐标
*						：w,h区域宽高
*						：b框边厚度
* 编写者		：YSZ
* 编写日期	：2016-12-13
* 最近改动	：2019-02-02
* 改动内容	：
*******************************************************************************/
void ILI94xx_frame(int x,int y,int w,int h,color_t cl,BYTE b)
{
	ILI94xx_rect(x,y,w,b,cl);
	ILI94xx_rect(x+w-b,y,b,h,cl);
	ILI94xx_rect(x,y+h-b,w,b,cl);
	ILI94xx_rect(x,y,b,h,cl);
}
/*******************************************************************************
* 函数名    ：ILI9488_draw
* 描述      ：绘图一个区域
* 返回      ：-
* 参数      ：pcolor指定16位图像所在地址
*           ：x,y左上角坐标
*           ：w,h区域宽高
* 编写者    ：YSZ
* 编写日期  ：2016-12-13
*******************************************************************************/
void ILI94xx_draw(int x,int y,int w,int h,const color_t* pcolor)
{
	int xend = x + w - 1;
	int yend = y + h - 1;
    ILI94xx_Area(x, y, xend, yend);
	ILI94xx_WStart();
    int count = w * h, len;
    while(count > 0){
		if(count > 0xFFFF){
			len = 0xFFFF;
			count -= 0xFFFF;
		}
		else{
			len = count;
			count = 0;
		}
		LCD_BatchSyn(pcolor,len,true);
    }
}

/*******************************************************************************
* 函数名    ：ILI9488_putc
* 描述      ：显示纯色二位图
* 返回      ：-
* 参数      ：caddr二位图像素地址
*           ：x,y左上角坐标
*           ：cw,ch绘制区域宽和高
*           : 每BYTE数字的每1位是1则显示fr前景颜色
*           : 是0,则当ifTransparent为false时显示bg背景色,否则跳过
* 编写者    ：YSZ
* 编写日期  ：2016-12-13
*******************************************************************************/
void ILI94xx_putc(int xStart,int yStart,const int wide,const int height,const int bytesPerLine,const BYTE* caddr,color_t fr,color_t bg,bool ifTransparent)
{
	const int xEnd = xStart + wide - 1;
	const int yEnd = yStart + height - 1;
	int n,t;
	BYTE c;
	for(;yStart <= yEnd; yStart++)
	{
		n = 0;
		LCD_WArea(xStart,yStart,xEnd,yStart);
		for(int ii = xStart;ii <= xEnd;)//显示一行,xd记录这一行的字节数
		{
			c = caddr[n];
			for(t = 0;t < 8 && ii <= xEnd; t++)
			{
				ii++;
				if(c & 0x80){
					LCD_WData(fr);
				}
				else {
					if(ifTransparent){
						LCD_WArea(ii,yStart,xEnd,yStart);
					}
					else {
						LCD_WData(bg);
					}
				}
				c <<= 1;
			}
			n++;//下一个字节
		}
		caddr += bytesPerLine;
	}
}
