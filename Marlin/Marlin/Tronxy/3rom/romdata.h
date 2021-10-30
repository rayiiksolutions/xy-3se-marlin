#ifndef _ROMDATA_H
#define _ROMDATA_H

#include "1inc/yBoard.h"
typedef enum
{
	NUM2 = 2,//二进制
	NUM8 = 8,//八进制
	NUM10 = 10,//十进制
	NUM16 = 16 //十六进制
}NUM_MODE;
typedef const char* CPSTR;
namespace yString
{
	int len(const char*str);//计算字符串长度
	int cmp(const char*source,const char* target);//比较两字符串大小,source>target返回1,source<target返回-1,source==target返回0
	void cpy(char* tar,const char*sce);//复制source到target中
	void cat(char* tar,const char*sce);//将source追加到target中
	void memzero(void* target,int size);
	/*******************************************************************************
	* 类名	：  Command		*函数:	hasElement
	* 描述		：判断源串中是否存在独立的目标串
				: 独立:在源串的目标串后边,当end!=0xFF时,后边只可能是空格类或'\0'或end字符
				  当end==0xFF时可以是任何字符
	* 参数		: sce-源串,tar-目标串
	* 返回		:如果存在,返回目标串在源串中的位置,否则返回-1
	* 编写者		：YSZ		编写日期:2020-03-24
	*******************************************************************************/
	int hasElement(const char* sce,const char* tar,uint8_t end = 0);
	uint8_t startWith(const char*source,const char *target);//source以target作开头返回1,否则返回0
	uint8_t endWith(const char *source,const char *target);//source以target作结尾返回1,否则返回0
	/*******************************************************************************
	* 函数名		：intToStr
	* 描述			：数字转字符串
	*	参数	:num 源数字
					:str 转后的字符串存放地址
					:mode 数制
	*	返回	:转换后的字符串长度
	* 编写者		：YSZ
	* 编写日期	：2019-12-28
	*******************************************************************************/
	int intToStr(int num, char*str, NUM_MODE mode);
	int intToStr(int num, char*str, NUM_MODE mode,int width);

	/*******************************************************************************
	* 函数名        ：doubleToStr
	* 描述          ：浮点数转字符串
	* 参数          :num 源数字
                   :str 转后的字符串存放地址
				   :width 转后的总宽度,负数左对齐,正数右对齐
				   :dot_num 转后的小数位数
	* 返回          :转换后的字符串长度
	* 编写者        ：YSZ
	* 编写日期      ：2019-12-28
	*******************************************************************************/
	int doubleToStr(double num, char*str, int width,int dot_num);

	/*******************************************************************************
	* 函数名		：strToInt
	* 描述			：字符串转数字
	*	参数	:str 源字符串
					:mode 数制
	*	返回	:转换后的结果
	* 编写者		：YSZ
	* 编写日期	：2020-01-12
	*******************************************************************************/
	int strToInt(const char*str,NUM_MODE mode);
	/*******************************************************************************
	* 函数名		：strToDouble
	* 描述			：字符串转浮点数
	*	参数	:str 源字符串
	*	返回	:转换后的结果
	* 编写者		：YSZ
	* 编写日期	：2020-01-12
	*******************************************************************************/
	double strToDouble(const char*str);
	inline bool isspace(char x){return x == ' ' || x == '\t' || x == '\f' || x == '\r' || x == '\n';}
	inline bool isseparate(char x,char v){return isspace(x) || x == v;}
	inline void pass(CPSTR& x,char v){while(*x && !isseparate(*x,v))x++; while(isseparate(*x,v))x ++;}
};

void romdataUpdate(void);
void romdataReset(uint32_t taddr);
uint8_t romdataGetOnebyte(void);

#endif
