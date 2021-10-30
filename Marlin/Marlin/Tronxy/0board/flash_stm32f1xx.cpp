#include "cxy_chitu_v6.h"
#if T2_STM32F103
#include <libmaple/flash.h>
#define FLASH_BANK1_END_ADDRESS   ((uint32_t)0x807FFFF)
#ifndef FLASH_KEY1
#define FLASH_KEY1    0x45670123UL
#endif
#ifndef FLASH_KEY2
#define FLASH_KEY2    0xCDEF89ABUL
#endif
void FLASH_Unlock(void)
{
  /* Authorize the FPEC of Bank1 Access */
  FLASH_BASE->KEYR = FLASH_KEY1;
  FLASH_BASE->KEYR = FLASH_KEY2;
}

void FLASH_Lock(void)
{
  FLASH_BASE->CR |= 1UL << 7;//上锁
}

static uint8_t getStatus(void)
{
  uint32_t sr = FLASH_BASE->SR;
  if(sr & (1UL << 0))return FLASH_BUSY; //忙
  else if(sr & (1UL << 2))return 3;	//操作异常:编程错误
  else if(sr & (1UL << 4))return 4;	//操作异常:写保护
  return FLASH_OK;
}

static uint8_t waitDone(uint32_t time)
{
	uint8_t res;
  do {
    res = getStatus();
    if(res != FLASH_BUSY)break;//非忙,无需等待了,直接退出.
    myDelayUs(1);
  }while(time--);
  if(time == 0)res = FLASH_TIMEOUT;
  return res;
}

uint8_t IFlash::eraseSector(uint32_t addr)
{
  FLASH_Unlock();
  uint8_t status = waitDone(0x5FFF);
  if(status == FLASH_OK){
		FLASH_BASE->CR |= 1UL << 1;//页擦除
		FLASH_BASE->AR = addr;//设置页地址
		FLASH_BASE->CR |= 1 << 6;//开始擦除  
    status = waitDone(0x5FFF);//等待操作结束,最大2s  
    if(status != FLASH_BUSY) //非忙
      FLASH_BASE->CR &= ~(1UL << 1);//清除页擦除标志.
  }
  FLASH_Lock();
  return status;
}

inline uint8_t FLASH_WriteHalfWord(uint32_t addr,uint16_t data)
{
  uint8_t status = waitDone(0xFF);
  if(status == FLASH_OK){
		FLASH_BASE->CR |= 1UL << 0;		//编程使能
		*(volatile uint16_t*)addr = data;		//写入数据
		status = waitDone(0XFF);//等待操作完成
		if(status != FLASH_BUSY)//操作成功
		{
			FLASH_BASE->CR &= ~(1UL << 0);	//清除PG位.
		}
  }
  return status;
}

/*******************************************************************************
* 函数名    :vwrite
* 描述      :向FLASH中指定地址写入指定长度的数据,必须先擦除使该页为0xFF
* 参数      :addr要写入的地址,保证是2的整数倍
            :data要写入的FLASH数据地址
            :byteSize写入的数据大小,字节为单位,必须小于addr所在页addr地址后面剩余空间
* 返回      :-
* 编写者    ：YSZ
* 编写日期  ：2019-12-28
*******************************************************************************/
uint8_t IFlash::vwrite(uint32_t addr,const void* data,uint32_t byteSize)
{
	uint8_t res = FLASH_OK;
	uint16_t w;
	const uint8_t* buffer = (const uint8_t*)data;
	FLASH_Unlock();
	for(uint32_t i = 0;i < byteSize;i += 2) //重新将数据写入FLASH
	{
		if(i + 1 == byteSize){
			w = buffer[i];
		}
		else {
			w = buffer[i + 1];
			w <<= 8;
			w |= buffer[i];
		}
		res = FLASH_WriteHalfWord(addr,w);
		if(res != FLASH_OK)break;
		addr += 2;
	}
	FLASH_Lock();
	return res;
}

/*******************************************************************************
* 函数名    :swrite
* 描述      :向FLASH中指定地址写入指定长度的数据,会擦除掉关联page的数据
* 参数      :pageAddr写入地址,必须是pageSize整数倍
			:data要写入的FLASH数据地址
			:byteSize写入的数据大小,字节为单位
* 返回      :-
* 编写者    ：YSZ
* 编写日期  ：2019-12-28
*******************************************************************************/
uint8_t IFlash::swrite(uint32_t& addr,uint32_t& nextEraseAddr,const void* data,uint32_t byteSize)
{
	uint8_t res = FLASH_OK;
	const uint8_t* buffer = (const uint8_t*)data;
	uint32_t len;
	while(byteSize){
		if(addr >= nextEraseAddr){
			res = eraseSector(nextEraseAddr);
			if(res != FLASH_OK)break;
			nextEraseAddr = nextSectorAddr(addr);
		}
		len = nextEraseAddr - addr;
		if(len > byteSize)len = byteSize;
		res = vwrite(addr,buffer,len);
		if(res != FLASH_OK)break;
		addr += len;
		buffer += len;
		byteSize -= len;
	}
	return res;
}

/*******************************************************************************
* 函数名    :read
* 描述      :读取FLASH中指定地址指定长度的数据
* 参数      :addr要读取的地址
            :data读取的数据存放的内存地址
            :size读取的大小
* 返回      :-
* 编写者    ：YSZ
* 编写日期  ：2019-12-28
*******************************************************************************/
void IFlash::read(uint32_t addr,void* data,uint32_t byteSize)
{
	uint8_t *buffer = (uint8_t*)data;
	for(uint32_t i = 0; i < byteSize; i++,addr++)buffer[i] = *(volatile const uint8_t*)addr;
}
#endif
