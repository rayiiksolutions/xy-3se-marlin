#include "yAt24cxx.hpp"

#include "src/libs/crc16.h"
#define AT24C01  127
#define AT24C02  255
#define AT24C04  511
#define AT24C08  1023
#define AT24C16  2047
#define AT24C32  4095
#define AT24C64  8191
#define AT24C128 16383
#define AT24C256 32767
#define AT24C512 65535

#define AT24CXX_TYPE AT24C16
#if AT24CXX_TYPE <= AT24C02
#define PAGE_SIZE 8
#elif AT24CXX_TYPE <= AT24C16
#define PAGE_SIZE 16
#else
#define PAGE_SIZE 32
#endif
/*******************************************************************************
* 函数名		:readOneByte
* 描述		:在AT24CXX指定地址读出一个数据
* 参数		:ReadAddr:开始读数的地址
* 返回		:读取到的数据
* 编写者		:YSZ
* 编写日期	:2019-03-03
*******************************************************************************/
uint8_t readOneByte(uint16_t ReadAddr)
{				  
	uint8_t temp=0;		  	    																 
	si2cStart();
	if(AT24CXX_TYPE > AT24C16) {
		si2cSendByte(0xA0);//发送写命令
		si2cWaitAck();
		si2cSendByte(ReadAddr >> 8);//发送高地址
	}
	else {
		si2cSendByte(0xA0 | ((ReadAddr >> 8) << 1));   //发送器件地址0XA0,写数据
	}
	si2cWaitAck(); 
	si2cSendByte(ReadAddr);//发送低地址
	si2cWaitAck();
	si2cStart();
	si2cSendByte(0xA1);//进入接收模式
	si2cWaitAck();
	temp = si2cReadByte(0);		   
	si2cStop();//产生一个停止条件	    
	return temp;
}

/*******************************************************************************
* 函数名		:writeOneByte
* 描述		:在AT24CXX指定地址写入一个数据
* 参数		:WriteAddr写入数据的目的地址,DataToWrite要写入的数据
* 返回		:-
* 编写者		:YSZ
* 编写日期	:2019-03-03
*******************************************************************************/ 
static void writeOneByte(uint16_t WriteAddr,uint8_t Data)
{				   	  	    																 
	si2cStart();
	if(AT24CXX_TYPE > AT24C16){
		si2cSendByte(0xA0);//发送写命令
		si2cWaitAck();
		si2cSendByte(WriteAddr >> 8);//发送高地址
	}
	else {
		si2cSendByte(0xA0 | ((WriteAddr >> 8) << 1));   //发送器件地址0XA0,写数据
	}
	si2cWaitAck();
	si2cSendByte(WriteAddr);//发送低地址
	si2cWaitAck();							  		   
	si2cSendByte(Data);//发送字节							   
	si2cWaitAck();
	si2cStop();//产生一个停止条件 
	delay(10);
}

/*******************************************************************************
* 函数名		:read
* 描述		:在AT24CXX里面的指定地址开始读出指定个数的数据
* 参数		:ReadAddr要读取的地址,buffer读取后存放数据的地址,len要读取的长度
* 返回		:-
* 编写者		:YSZ
* 编写日期	:2019-03-03
*******************************************************************************/ 
void AT24CXX_Read(int& ReadAddr,void *buffer,int len, const bool writing)
{
	unsigned char*pBuffer=(unsigned char*)buffer,c;
	do{
		c = readOneByte(ReadAddr);
		if(writing)*pBuffer = c;
		pBuffer++;
		ReadAddr++;
	}while(--len);
}

/*******************************************************************************
* 函数名		:write
* 描述		:在AT24CXX里面的指定地址开始写入指定个数的数据
* 参数		:WriteAddr要写入的地址,buffer写入的数据地址,len要写入的长度
* 返回		:-
* 编写者		:YSZ
* 编写日期	:2019-03-03
*******************************************************************************/ 
void AT24CXX_Write(int& WriteAddr,const void *buffer,int len)
{
	#ifdef AT24CXX_WP
	WRITE(AT24CXX_WP,LOW);//解除写保护
	#endif
	unsigned char c,i = 0;
	const unsigned char*pBuffer=(const unsigned char*)buffer;
	do{
		c = readOneByte(WriteAddr);
		if(c != *pBuffer)writeOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
		if(++i > 10){
			watchdog_refresh();
			i = 0;
		}
	}while(--len);
	#ifdef AT24CXX_WP
	WRITE(AT24CXX_WP,HIGH);//复位写保护
	#endif
}

