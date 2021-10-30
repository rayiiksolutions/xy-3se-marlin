// #include "yConfiguration.h"
#ifdef TFT_USE_LGDP4535
unsigned short LGDP4535_dirVar;
void LGDP4535_SetDir(unsigned char dir)
{
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
		LGDP4535_dirVar=0x0038;
	}
	else
	{
		RTFT_WCMD(0x0001);
		RTFT_WDAT(0x0100);
		LGDP4535_dirVar=0x0030;
	}
	if(dir&TFTDIR_INVERT_RB)LGDP4535_dirVar|=0x1000;
	RTFT_WCMD(0x0003);//R0003
	RTFT_WDAT(LGDP4535_dirVar);
	LGDP4535_dirVar=dir;
}
/*******************************************************************************
	* 函数名		：LGDP4535_Init
	* 描述			：LGDP4535屏初始化
	* 返回			：-
	* 参数			：
	* 编写者		：YSZ
	* 编写日期	：2017-05-17
	* 最近改动	：-
	* 改动内容	：-
	*******************************************************************************/
void LGDP4535_Init(unsigned char dir)
{
	RTFT_WCMD(0x0015);
	RTFT_WDAT(0x0030);
	RTFT_WCMD(0x009A);
	RTFT_WDAT(0x0010);
	RTFT_WCMD(0x0011);
	RTFT_WDAT(0x0020);
	RTFT_WCMD(0x0010);
	RTFT_WDAT(0x3428);
	RTFT_WCMD(0x0012);
	RTFT_WDAT(0x0002);
	RTFT_WCMD(0x0013);
	RTFT_WDAT(0x1038);
	DELAY(40);
	RTFT_WCMD(0x0012);
	RTFT_WDAT(0x0012);
	DELAY(40);
	RTFT_WCMD(0x0010);
	RTFT_WDAT(0x3420);
	RTFT_WCMD(0x0013);
	RTFT_WDAT(0x303E);
	DELAY(70);
	RTFT_WCMD(0x0030);
	RTFT_WDAT(0x0000);
	RTFT_WCMD(0x0031);
	RTFT_WDAT(0x0402);
	RTFT_WCMD(0x0032);
	RTFT_WDAT(0x0307);
	RTFT_WCMD(0x0033);
	RTFT_WDAT(0x0304);
	RTFT_WCMD(0x0034);
	RTFT_WDAT(0x0004);
	RTFT_WCMD(0x0035);
	RTFT_WDAT(0x0401);
	RTFT_WCMD(0x0036);
	RTFT_WDAT(0x0707);
	RTFT_WCMD(0x0037);
	RTFT_WDAT(0x0305);
	RTFT_WCMD(0x0038);
	RTFT_WDAT(0x0610);
	RTFT_WCMD(0x0039);
	RTFT_WDAT(0x0610);
	
	RTFT_WCMD(0x0002);
	RTFT_WDAT(0x030F);
	RTFT_WCMD(0x0008);
	RTFT_WDAT(0x0808);
	RTFT_WCMD(0x000A);
	RTFT_WDAT(0x0008);
	RTFT_WCMD(0x0060);
	RTFT_WDAT(0x2700);
	RTFT_WCMD(0x0061);
	RTFT_WDAT(0x0001);
	RTFT_WCMD(0x0090);
	RTFT_WDAT(0x013E);
	RTFT_WCMD(0x0092);
	RTFT_WDAT(0x0100);
	RTFT_WCMD(0x0093);
	RTFT_WDAT(0x0100);
	RTFT_WCMD(0x00A0);
	RTFT_WDAT(0x3000);
	RTFT_WCMD(0x00A3);
	RTFT_WDAT(0x0010);
	RTFT_WCMD(0x0007);
	RTFT_WDAT(0x0001);
	RTFT_WCMD(0x0007);
	RTFT_WDAT(0x0021);
	RTFT_WCMD(0x0007);
	RTFT_WDAT(0x0023);
	RTFT_WCMD(0x0007);
	RTFT_WDAT(0x0033);
	RTFT_WCMD(0x0007);
	RTFT_WDAT(0x0133);
	LGDP4535_SetDir(dir);
}
void LGDP4535_Setpos(const int& x,const int& y)//设置屏幕画点的位置
{
	#if 1
	if(LGDP4535_dirVar&TFTDIR_SWAPXY)
	{
		RTFT_WCMD(0x0020);//x
		RTFT_WDAT(y);
		RTFT_WCMD(0x0021);//y
		RTFT_WDAT(x);
	}
	else
	{
		RTFT_WCMD(0x0020);//x
		RTFT_WDAT(x);
		RTFT_WCMD(0x0021);//y
		RTFT_WDAT(y);
	}
	#else
	RTFT_WCMD(0x0020);//x
	RTFT_WDAT(y);
	RTFT_WCMD(0x0021);//y
	RTFT_WDAT(x);
	#endif
	RTFT_WCMD(0x0022);
}	
void LGDP4535_Workplace(const int& xStart,const int& yStart,const int& xEnd,const int& yEnd)
{
	if(LGDP4535_dirVar&TFTDIR_SWAPXY)
	{
		RTFT_WCMD(0x0050);//y  
		RTFT_WDAT(yStart);
		RTFT_WCMD(0x0051);
		RTFT_WDAT(yEnd);
		RTFT_WCMD(0x0052);//x
		RTFT_WDAT(xStart);
		RTFT_WCMD(0x0053);
		RTFT_WDAT(xEnd);
		RTFT_WCMD(0x0020);//x
		RTFT_WDAT(yStart);
		RTFT_WCMD(0x0021);//y
		RTFT_WDAT(xStart);
	}
	else
	{
		RTFT_WCMD(0x0050);//x  
		RTFT_WDAT(xStart);
		RTFT_WCMD(0x0051);
		RTFT_WDAT(xEnd);
		RTFT_WCMD(0x0052);//y 
		RTFT_WDAT(yStart);
		RTFT_WCMD(0x0053);
		RTFT_WDAT(yEnd);
		RTFT_WCMD(0x0020);//x
		RTFT_WDAT(xStart);
		RTFT_WCMD(0x0021);//y
		RTFT_WDAT(yStart);
	}
	RTFT_WCMD(0x0022);
}
#endif
