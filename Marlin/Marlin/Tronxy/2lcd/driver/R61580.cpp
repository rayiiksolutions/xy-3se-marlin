// #include "yConfiguration.h"
#ifdef TFT_USE_R61580
unsigned short R61580_dirVar;
void R61580_SetDir(unsigned char dir)
{
	//只支持XY翻转,不支持其它方向调整
	//对R0001
	//b8:扫描反向
	//对R0003
	//b12:0写入颜色BGR,1写入颜色BGR(红蓝反转)
	//b4,b5决定写入颜色后地址自增或自减,b3地址方向是1垂直还是0水平
	//if(b3==0)
	//00,先水平自减,到行首后,水平回到行尾,垂直自减1。。。
	//01,先水平自增,到行尾后,水平回到行首,垂直自减1。。。
	//10,先水平自减,到行首后,水平回到行尾,垂直自增1。。。
	//11,先水平自增,到行尾后,水平回到行首,垂直自增1。。。
	//if(b3==1)
	//00,先垂直自减,到列首后,垂直回到列尾,水平自减1。。。
	//01,先垂直自增,到列尾后,垂直回到列首,水平自减1。。。
	//10,先垂直自减,到列首后,垂直回到列尾,水平自增1。。。
	//11,先垂直自增,到列尾后,垂直回到列首,水平自增1。。。
	if(dir&TFTDIR_SWAPXY)
	{
		RTFT_WCMD(0x0001);//R0001
		RTFT_WDAT(0x0000);
		R61580_dirVar=0x0038;
	}
	else
	{
		RTFT_WCMD(0x0001);
		RTFT_WDAT(0x0100);
		R61580_dirVar=0x0030;
	}
	if(dir&TFTDIR_INVERT_RB)R61580_dirVar|=0x1000;
	RTFT_WCMD(0x0003);//R0003
	RTFT_WDAT(R61580_dirVar);
	R61580_dirVar=dir;
}
/*******************************************************************************
	* 函数名		：Ili9486Init
	* 描述			：Ili9486屏初始化
	* 返回			：-
	* 参数			：
	* 编写者		：YSZ
	* 编写日期	：2016-12-13
	* 最近改动	：2017-05-08
	* 改动内容	：-
	*******************************************************************************/
void R61580_Init(unsigned char dir)
{
		RTFT_WCMD(0x0000);
		RTFT_WCMD(0x0000);
		DELAY(10);
		RTFT_WCMD(0x0000);
		RTFT_WCMD(0x0000);
		RTFT_WCMD(0x0000);
		RTFT_WCMD(0x0000);
		RTFT_WCMD(0x0000);
		RTFT_WCMD(0x0000);
		RTFT_WCMD(0x0000);
		RTFT_WCMD(0x0000);
		DELAY(10);
		RTFT_WCMD(0x00A4); 
		RTFT_WDAT(0x0001);
		DELAY(10);
		RTFT_WCMD(0x0060); 
		RTFT_WDAT(0xa700);//Driver Output Control NL
		RTFT_WCMD(0x0008); 
		RTFT_WDAT(0x0808);//Display Control 2 (R08h) FP BP
		//Gamma setting R30~39h: Gamma Control
		RTFT_WCMD(0x0030); 
		RTFT_WDAT(0x0203);
		RTFT_WCMD(0x0031); 
		RTFT_WDAT(0x080F);
		RTFT_WCMD(0x0032); 
		RTFT_WDAT(0x0401);
		RTFT_WCMD(0x0033); 
		RTFT_WDAT(0x0508);
		RTFT_WCMD(0x0034); 
		RTFT_WDAT(0x3330);
		RTFT_WCMD(0x0035); 
		RTFT_WDAT(0x0B05);
		RTFT_WCMD(0x0036); 
		RTFT_WDAT(0x0005);
		RTFT_WCMD(0x0037); 
		RTFT_WDAT(0x0F08);
		RTFT_WCMD(0x0038); 
		RTFT_WDAT(0x0302);
		RTFT_WCMD(0x0039); 
		RTFT_WDAT(0x3033);

		RTFT_WCMD(0x0090); 
		RTFT_WDAT(0x0018);//85HZ
		RTFT_WCMD(0x0010); 
		RTFT_WDAT(0x0530);//BT, AP
		RTFT_WCMD(0x0011); 
		RTFT_WDAT(0x0237);//DC,VC
		RTFT_WCMD(0x0012); 
		RTFT_WDAT(0x01BF);//Power Charge Pump Need 500ms
		RTFT_WCMD(0x0013); 
		RTFT_WDAT(0x1000);//vcom
		DELAY(100);
		RTFT_WCMD(0x0002); 
		RTFT_WDAT(0x0200); //Line Inversion
		RTFT_WCMD(0x0003); 
		RTFT_WDAT(0x1030); //TRIREG=1
		RTFT_WCMD(0x0009); 
		RTFT_WDAT(0x0001);
		RTFT_WCMD(0x000A); 
		RTFT_WDAT(0x0008);
		RTFT_WCMD(0x000C); 
		RTFT_WDAT(0x0000); //
		RTFT_WCMD(0x000D); 
		RTFT_WDAT(0x0000);
		RTFT_WCMD(0x000E); 
		RTFT_WDAT(0x0030);
		RTFT_WCMD(0x000F); 
		RTFT_WDAT(0x0000); //
		//RAM access instruction
		RTFT_WCMD(0x0020); 
		RTFT_WDAT(0x0000); //RAM Address Set (Horizontal Address) (R20h)
		RTFT_WCMD(0x0021); 
		RTFT_WDAT(0x0000); //RAM Address Set (Vertical Address) (R21h)
		//Window address control instruction
		RTFT_WCMD(0x0029); 
		RTFT_WDAT(0x002E);
		RTFT_WCMD(0x0050); 
		RTFT_WDAT(0x0000); //Window Horizontal RAM Address Start/End (R50h/ R51h)
		RTFT_WCMD(0x0051); 
		RTFT_WDAT(0x00EF);
		RTFT_WCMD(0x0052); 
		RTFT_WDAT(0x0000); //Window Vertical RAM Address Start/End (R52h/R53h)
		RTFT_WCMD(0x0053); 
		RTFT_WDAT(0x013F);
		//Base image display control instruction
		RTFT_WCMD(0x0061); 
		RTFT_WDAT(0x0001); //Base Image Display Control (R61h)0x0000
		RTFT_WCMD(0x006A); 
		RTFT_WDAT(0x0000); ///Vertical Scroll Control (R6Ah)
		//Partial display control instruction
		RTFT_WCMD(0x0080); 
		RTFT_WDAT(0x0000);
		RTFT_WCMD(0x0081); 
		RTFT_WDAT(0x0000);
		RTFT_WCMD(0x0082); 
		RTFT_WDAT(0x005F);
		//Panel interface control instruction

		RTFT_WCMD(0x0093); 
		RTFT_WDAT(0x0701);

		RTFT_WCMD(0x0007); 
		RTFT_WDAT(0x0100);
		DELAY(10);

		RTFT_WCMD(0x0029); 
		RTFT_WDAT(0x002E);
		
		R61580_SetDir(dir);
}
void R61580_Setpos(const int& x,const int& y)
{
	if(R61580_dirVar&TFTDIR_SWAPXY)
	{
		RTFT_WCMD(0x0020); 
		RTFT_WDAT(y); //RAM Address Set (Horizontal Address) (R20h)
		RTFT_WCMD(0x0021); 
		RTFT_WDAT(x); //RAM Address Set (Vertical Address) (R21h)
	}
	else
	{
		RTFT_WCMD(0x0020); 
		RTFT_WDAT(x); //RAM Address Set (Horizontal Address) (R20h)
		RTFT_WCMD(0x0021); 
		RTFT_WDAT(y); //RAM Address Set (Vertical Address) (R21h)
	}
	RTFT_WCMD(0x0022);
}
void R61580_Workplace(const int& xStart,const int& yStart,const int& xEnd,const int& yEnd)
{
	if(R61580_dirVar&TFTDIR_SWAPXY)
	{
		RTFT_WCMD(0x0050); 
		RTFT_WDAT(yStart); //Window Horizontal RAM Address Start/End (R50h/ R51h)
		RTFT_WCMD(0x0051); 
		RTFT_WDAT(yEnd);
		RTFT_WCMD(0x0052); 
		RTFT_WDAT(xStart); //Window Vertical RAM Address Start/End (R52h/R53h)
		RTFT_WCMD(0x0053); 
		RTFT_WDAT(xEnd);
		RTFT_WCMD(0x0020); 
		RTFT_WDAT(yStart); //RAM Address Set (Horizontal Address) (R20h)
		RTFT_WCMD(0x0021); 
		RTFT_WDAT(xStart); //RAM Address Set (Vertical Address) (R21h)
	}
	else
	{
		RTFT_WCMD(0x0050); 
		RTFT_WDAT(xStart); //Window Horizontal RAM Address Start/End (R50h/ R51h)
		RTFT_WCMD(0x0051); 
		RTFT_WDAT(xEnd);
		RTFT_WCMD(0x0052); 
		RTFT_WDAT(yStart); //Window Vertical RAM Address Start/End (R52h/R53h)
		RTFT_WCMD(0x0053); 
		RTFT_WDAT(yEnd);
		RTFT_WCMD(0x0020); 
		RTFT_WDAT(xStart); //RAM Address Set (Horizontal Address) (R20h)
		RTFT_WCMD(0x0021); 
		RTFT_WDAT(yStart); //RAM Address Set (Vertical Address) (R21h)
	}
	RTFT_WCMD(0x0022);
}
WORD R61580_ReadId()
{
	WORD id=0;
	RTFT_WCMD(0x0000);
	id=RTFT_RREG();
	id=RTFT_RREG();
	id=RTFT_RREG();
	id=RTFT_RREG();
	id=RTFT_RREG();
	id=RTFT_RREG();
	return id;
}
#endif
