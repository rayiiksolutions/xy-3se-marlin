#pragma once

#include "1inc/yType.h"

#include "1inc/yBoard.h"
#if EROM_CHIP_TYPE == EROM_W25QXX
/*******************************************************************************
-->需硬件平台提供以下定义或接口
1.EFlashMcuInit                    MCU上对FLASH初始化
2.EFlashCs()/EFlashUncs()          片选/不片选
3.EFlashSpiSendByte(byte)          向EFLASH发送(同时读)一个字节
4.EFlashSpiBatchRead(buf,nbyte)    批量读取EFlash数据

-->本驱提供以下接口
1.EFLASH_Init                      EFlash初始化,返回芯片ID
2.EFLAH_Read(addr,buff,len)        从地址addr读取长度为len字节的数据存放在buff处
3.EFlashErase(addr)                从addr处擦除一块扇区,addr必须是扇区大小EFLASH_SECTORSIZE整数倍
4.EFLAH_Write(addr,buff,len,exchangeBuff) 从buff将长度为len字节的数据存存放在addr处
5.EFLash_SWrite(addr,buff,len)     快速写入数据到FLASH,不保留扇区内其它数据
且地址必须是扇区首地址,即地址是EFLASH_SECTORSIZE整数倍
EN25Qxx和W25Qxx通用
*******************************************************************************/
//IDEN25Q80 ID 0x1C13,
//IDEN25Q16 ID 0x1C14,
//IDEN25Q32 ID 0x1C15,
//IDEN25Q64 ID 0xC816
//W25Q80 ID  0XEF13
//W25Q16 ID  0XEF14
//W25Q32 ID  0XEF15
//W25Q64 ID  0XEF16
//W25Q128 ID  0XEF17
#define EFLASH_ID                 0xC816
#define EFLASH_SECTORCOUNT        0x800 //扇区总数,64Mbit=8MB=0x800000Byte=0x1000*0x800即扇区数=0x800
#define EFLASH_SECTORSIZE         0x1000 //扇区大小:4K字节,擦除时,以扇区为单位进行擦除的
#define EFLASH_BLOCKSIZE          1 //每block有多少扇区
#define W25Q_PAGESIZE             0x100 //页大小,EN25Q64是以页为单位进行写入的

WORD W25QxxInit(void);
BYTE W25QxxReadOnebyte(DWORD addr);
void W25QxxBatchRead(DWORD addr,void* buff,int len);
void W25QxxBatchReadSyn(DWORD addr,void* buff,int len);
bool W25QxxBatchReadOk(void);

#endif //#if EROM_CHIP_TYPE == EROM_W25QXX
