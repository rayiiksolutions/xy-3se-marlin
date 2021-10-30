#ifndef Y_WINDOWS_TYPE_H
#define	Y_WINDOWS_TYPE_H
#include "1inc/yType.h"

typedef struct
{
	int x;
	int y;
}POS;

typedef struct WINRECT
{
	int  xStart;
	int  yStart;
	int  xEnd;
	int  yEnd;
}WRECT;
	
typedef struct //BMP头
{
	WORD bfType;    //文件类型，BMP格式为字符串BM
	DWORD bfSize;		//整个图片文件的大小,单位为字节
	WORD bfReserved1;	//保留位
	WORD bfReserved2;	//保留位
	DWORD bfOffBits;  	//从文件头到实际图像数据之间的字节偏移量
} bmpHead_t;

typedef struct //BMP信息,紧接着BMP头
	{
		DWORD     biSize;//BMP信息头(也即本结构体)所占空间大小(以字节为单位)
		long    biWidth;//图片宽度
		long    biHeight;//图片高度(该值还可以表示位图是倒立还是正向,如果为正数[大多数是这样]则为倒立位图,如果为负数,则为正向位图)
		WORD     biPlanes;//目标设备颜色平面数,总是为1
		WORD     biBitCount;
		//位图类型,表示每个像素用多少位来表示,单位:bit/pixel,其值可以是1,4,8,16,24或32
		DWORD     biCompression;
		//压缩类型
		//0-BI_RGB无压缩
		//1-BI_RLE8 8bit游程编码,只用于8位位图
		//2-BI_RLE4 4bit游程编码,只用于4位位图
		//3-BI_BITFIELDS bit域,只用于16/32位位图
		//4-BI_JPEG		位图含JPEG图像(仅用于打印机)
		//5-BI_PNG		位图含PNG图像(仅用于打印机)
		DWORD     biSizeImage;//图像大小,以字节为单位,如果biCompression为BI_RGB,则这里可设为0
		long    biXPelsPerMeter;//水平分辩率,单位:像素/米
		long    biYPelsPerMeter;//垂直分辩率,单位:像素/米
		DWORD     biClrUsed;
		//实际使用颜色表中的颜色索引数(设为0,说明实用所有调色板项)
		//这里将决定调色板的大小
		DWORD     biClrImportant;
		//说明图像显示有重要影响的颜色索引的数目,如果是0,表示都重要
		//指调色板中的重要索引,早期显卡不一定能全显示所有调色板的颜色,所以将重要的颜色放在前面,这个量表示多少个是重要的
	}bmpInfo_t;
//调色板可以看成是一个下面数据类型的数组,表示一组颜色
//如果有调色板,则数据区的每一个值表示的是调色板中的数组下标,对应的数组元素表示的颜色即是数据区的那个值对应的颜色

typedef struct
{
  bmpHead_t head;
  bmpInfo_t info;
}BMPHead;

enum {
TYPE_BINARY, //二元色图(ascii字库),vSpace表示一个二元图(字符)的所占大小，wNum表示所含图(字符)数
TYPE_COLOR, //纯色填充,variable时,value1为起始颜色,value2为末端颜色
TYPE_PIE, //画圆
TYPE_PIC, //图片,value可以找到图片像素
TYPE_CPIC,//压缩图
TYPE_SELFFUN //是纯色自定义绘图,value指向绘制函数
};
typedef struct //16
{//如果是字符,则用横向取模
	struct
	{
		BYTE ifERom:1;//是否使用外部ROM,如果使用,则为1,否则为0
		BYTE type:3;//见上面宏定义
		BYTE typeTf:1;
		//当type==TYPE_BINARY时,1中文,0ascii码
		//当type==TYPE_CPIC时,0整,1行
		BYTE black_tf:1;//1遇黑透明
		BYTE ifFrame:1;//是否显示边框
		BYTE former:1;//置1后,更新该控件时,需要重绘它下面的控件
	}attr;
	BYTE zero;
	//如果是ascii码字库,则为首字符
	//如果是中文字库,则为字形码与中文码的偏移,通常是3
	//否则为背景透明度0~255
	WORD wNum;//总共收录多少个字符,普通图片或纯色这里必须为1
	int16_t xPixel;//x像素长度(字符一定要有效长度),纯色这里无效
	int16_t yPixel;//y像素长度,纯色这里无效
	WORD bytesPerLine;//一行像素多少字节,纯色这里无效
	//例如24*24的汉字,这里就是3,因为每行有24像素,每一位代表一像素,所以,每一行需要3byte来表示
	//如果是16位的彩色图,每个像素2个字节,那么200*200这样的图行,每行就是400byte来表示
	int vSpace;//图片:图片数据所占空间的大小,字符:每个字符所占空间的大小,纯色这里无效
	u32 value;//图片:地址,纯色:16位色时,低16位是填充颜色,32位时,表示填充颜色
}PICinfo;
typedef struct
{
	BYTE ifOpaque;//是否背景透明
	color_t fr,bg;//前景色和背景色
	int x,y;//显示字符时的坐标(左上角)
	const PICinfo*pic;
}BinaryInfo;
#endif
