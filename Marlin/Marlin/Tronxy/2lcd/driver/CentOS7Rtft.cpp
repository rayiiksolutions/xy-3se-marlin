#include "CentOS7Rtft.h"
#if 0
#include <iostream>
using std::cout;
using std::endl;
const char*DevName="/dev/fb0";
int FbId=-1;
int frameBuffer_RectWidth;
struct fb_var_screeninfo CentOS7FbVar;
struct fb_fix_screeninfo CentOS7FbFix;
u64 CentOS7FbMemOffset,CentOS7FbMem;
u64 frameBuffer_BaseAddr;
color32 *frameBuffer_Pen;
color32 *frameBuffer_xStart,*frameBuffer_Ultimate;
/*******************************************************************************
* 函数名		: CentOS7Rtft_Init
* 描述			: 打开窗口设备并映射framebuffer
* 返回			: -
* 编写者		: YSZ
* 编写日期	: 2019-02-12
* 最近改动	: -
* 改动内容	: -
*******************************************************************************/
int CentOS7Rtft_Init()
{
	FbId=open(DevName,O_RDWR);
	if(FbId<0)
	{
		cout<<"Open "<<DevName<<" fail."<<endl;
		return 1;
	}
	if(-1 == ioctl(FbId,FBIOGET_VSCREENINFO,&CentOS7FbVar))
	{
		cout<<"Get screen var info fail."<<endl;
		return 2;
	}
	if(-1 == ioctl(FbId,FBIOGET_FSCREENINFO,&CentOS7FbFix))
	{
		cout<<"Get screen fix info fail."<<endl;
		return 3;
	}
	CentOS7FbMemOffset=(unsigned long)(CentOS7FbFix.smem_start)&(~PAGE_MASK);
	CentOS7FbMem=(unsigned long)mmap(NULL,CentOS7FbFix.smem_len+CentOS7FbMemOffset,
			PROT_READ|PROT_WRITE,MAP_SHARED,FbId,0);
	if((unsigned long)MAP_FAILED == CentOS7FbMem)
	{
		cout<<"mmap error:mem:"<<CentOS7FbMem<<" offset:"<<CentOS7FbMemOffset<<endl;
		return 4;
	}
  CentOS7Rtft_SetVirtual(RTFT_LMAX,RTFT_WMAX);
  CentOS7Rtft_SetVisible(RTFT_LMAX,RTFT_WMAX);
  frameBuffer_BaseAddr=CentOS7FbMem+CentOS7FbVar.yoffset*(CentOS7FbFix.line_length>>2)+CentOS7FbVar.xoffset;
  frameBuffer_Ultimate=frameBuffer_xStart=(color32*)frameBuffer_BaseAddr;
  frameBuffer_RectWidth=0;
	return 0;
}
/*******************************************************************************
* 函数名		: CentOS7Rtft_Close
* 描述			: 关闭设备,关闭窗口系统时需调用
* 返回			: -
* 编写者		: YSZ
* 编写日期	: 2019-02-12
* 最近改动	: -
* 改动内容	: -
*******************************************************************************/
void CentOS7Rtft_Close()
{
  close(FbId);
	FbId=-1;
}
/*******************************************************************************
* 函数名		: CentOS7Rtft_SetVirtual
* 描述			: 设置分辩率
* 返回			: -
* 编写者		: YSZ
* 编写日期	: 2019-02-12
* 最近改动	: -
* 改动内容	: -
*******************************************************************************/
void CentOS7Rtft_SetVirtual(int width,int height)
{
	if(FbId<0)return;
	CentOS7FbVar.xres_virtual=width;
	CentOS7FbVar.yres_virtual=height;
	if(-1 == ioctl(FbId,FBIOPUT_VSCREENINFO,&CentOS7FbVar))
	{
		cout<<"ioctl set fail."<<endl;
		return;
	}
	if(-1 == ioctl(FbId,FBIOGET_VSCREENINFO,&CentOS7FbVar))
		cout<<"ioctl get fail."<<endl;
}
/*******************************************************************************
* 函数名		: CentOS7Rtft_SetVisible
* 描述			: 设置可见区域
* 返回			: -
* 编写者		: YSZ
* 编写日期	: 2019-02-12
* 最近改动	: -
* 改动内容	: -
*******************************************************************************/
void CentOS7Rtft_SetVisible(int width,int height)
{
	if(FbId<0)return;
	CentOS7FbVar.xres=width;
	CentOS7FbVar.yres=height;
	if(-1 == ioctl(FbId,FBIOPUT_VSCREENINFO,&CentOS7FbVar))
	{
		cout<<"ioctl set fail."<<endl;
		return;
	}
	if(-1 == ioctl(FbId,FBIOGET_VSCREENINFO,&CentOS7FbVar))
		cout<<"ioctl get fail."<<endl;
}
#endif //#if LCD_MODE == LCD_CENTOS7_V1
