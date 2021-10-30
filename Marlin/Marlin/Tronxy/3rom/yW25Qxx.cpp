#include "yW25QxxApi.h"

#if EROM_CHIP_TYPE == EROM_W25QXX
#define EN25Q64WAITTIME           0xFFFF
#define EN25X_ReadStatusReg       0x05 //读取状态寄存器
#define EN25X_WriteStatusReg      0x01 //写状态寄存器
#define EN25X_PageProgram         0x02 //页编辑
#define EN25X_ReadData            0x03 //读数据
#define EN25X_WriteDisable        0x04 //写失能
#define EN25X_WriteEnable         0x06 //写使能
#define EN25X_WriteDisable        0x04
#define EN25X_BlockErase          0xD8
#define EN25X_SectorErase         0x20 //擦除块
#define EN25X_ChipErase           0xC7 //擦除片	 
#define EN25X_FastReadData        0x0B 
#define EN25X_FastReadDual        0x3B 
#define EN25X_PowerDown           0xB9 
#define EN25X_ReleasePowerDown    0xAB 
#define EN25X_DeviceID            0xAB 
#define EN25X_ManufactDeviceID    0x90 
#define EN25X_JedecDeviceID       0x9F
#define setPinOutput(IO)    gpio_set_mode(PIN_MAP[IO].gpio_device, PIN_MAP[IO].gpio_bit, GPIO_OUTPUT_PP)
#define WriteEnable()       EFlashCs();EFlashSpiSendByte(EN25X_WriteEnable);EFlashUncs()
/*******************************************************************************
* 函数名    : EN25Q64SPIInit
* 描述      : EN25Q64初始化
* 返回      : 返回它的ID
* 编写者    : YSZ
* 编写日期	: 2016-12-22
* 最近改动	: 2019-02-13
* 改动内容	: -
*******************************************************************************/
	WORD W25QxxInit(void)
	{
		WORD id;
		EFlashMcuInit();
		EFlashCs();
		EFlashSpiSendByte(EN25X_ManufactDeviceID);
		EFlashSpiSendByte(0x00);
		EFlashSpiSendByte(0x00);
		EFlashSpiSendByte(0x00);
		id = EFlashSpiSendByte(0xff);
		id = (id << 8) | EFlashSpiSendByte(0xff);
		EFlashUncs();
		return id;
	}

/*******************************************************************************
* 函数名    ：W25QxxRead
* 描述      ：FLASH读取数据
* 返回      ：状态标志
* 编写者    ：YSZ
* 编写日期  ：2016-12-23
* 最近改动  ：-
* 改动内容  ：-
*******************************************************************************/
	bool reading = false;
	void W25QxxBatchRead(DWORD addr,void* buff,int len)
	{
		while(!W25QxxBatchReadOk()){}
		BYTE* tar=(BYTE*)buff;
		EFlashCs();
		EFlashSpiSendByte(EN25X_ReadData);
		EFlashSpiSendByte(addr >> 16);
		EFlashSpiSendByte(addr >> 8);
		EFlashSpiSendByte(addr);
		EFlashSpiBatchRead(tar,len);
		reading = true;
	}
	bool W25QxxBatchReadOk(void)
	{
		if(reading && EFlashSpiBatchReadOk()){
			EFlashUncs();
			reading = false;
		}
		return !reading;
	}
	BYTE W25QxxReadOnebyte(DWORD addr)
	{
		BYTE dat;
		while(!W25QxxBatchReadOk()){}
		EFlashCs();
		EFlashSpiSendByte(EN25X_ReadData);
		EFlashSpiSendByte(addr >> 16);
		EFlashSpiSendByte(addr >> 8);
		EFlashSpiSendByte(addr);
		dat = EFlashSpiSendByte(0xFF);
		EFlashUncs();
		return dat;
	}
	void W25QxxBatchReadSyn(DWORD addr,void* buff,int len)
	{
		uint8_t *tbuff = (uint8_t*)buff;
		while(!W25QxxBatchReadOk()){}
		EFlashCs();
		EFlashSpiSendByte(EN25X_ReadData);
		EFlashSpiSendByte(addr >> 16);
		EFlashSpiSendByte(addr >> 8);
		EFlashSpiSendByte(addr);
		for(int i = 0;i < len; i++)tbuff[i] = EFlashSpiSendByte(0xFF);
		EFlashUncs();
	}
#endif //#if EROM_TYPE == EROM_W25QXX
