#include "cxy_chitu_v6.h"
#if T2_STM32F103 && USE_SDIO_MAPLE == 2
#include <libmaple/sdio.h>
#include <libmaple/dma.h>
typedef struct {
  uint8_t version;   // Card version
  uint32_t type;     // Card Type
  uint32_t Class;         // Class of the card class
  uint32_t RelCardAdd;    // Relative Card Address
  uint32_t BlockNbr;      // Card Capacity in blocks
  uint32_t BlockSize;     // One block size in bytes
  uint32_t LogBlockNbr;   // Card logical Capacity in blocks
  uint32_t LogBlockSize;  // Logical block size in bytes
} SDIO_CardInfo;
typedef struct
{
  volatile uint32_t POWER;
  volatile uint32_t CLKCR;
  volatile uint32_t ARG;
  volatile uint32_t CMD;
  volatile uint32_t RESPCMD;
  volatile uint32_t RESP1;
  volatile uint32_t RESP2;
  volatile uint32_t RESP3;
  volatile uint32_t RESP4;
  volatile uint32_t DTIMER;
  volatile uint32_t DLEN;
  volatile uint32_t DCTRL;
  volatile uint32_t DCOUNT;
  volatile uint32_t STA;
  volatile uint32_t ICR;
  volatile uint32_t MASK;
  uint32_t  RESERVED0[2];
  volatile uint32_t FIFOCNT;
  uint32_t  RESERVED1[13];
  volatile uint32_t FIFO;
} SDIO_TypeDef;
typedef struct
{
  volatile uint32_t CCR;
  volatile uint32_t CNDTR;
  volatile uint32_t CPAR;
  volatile uint32_t CMAR;
} DMA_Channel_TypeDef;

#define F_HCLK                               72000000
#define SDIO_CMDTIMEOUT                      200U           /* Command send and response timeout */
#define SDIO_DATA_TIMEOUT                    100U           /* Read data transfer timeout */
#define SDIO_WRITE_TIMEOUT                   200U           /* Write data transfer timeout */
#define SDIO_RESP_ADDR                       ((uint32_t)(mSDIO + 0x14))
#define SDIO_RESP1                           ((uint32_t)0x00000000)
#define SDIO_RESP2                           ((uint32_t)0x00000004)
#define SDIO_RESP3                           ((uint32_t)0x00000008)
#define SDIO_RESP4                           ((uint32_t)0x0000000C)
#define PERIPH_BASE                          0x40000000UL /*!< Peripheral base address in the alias region */
#define SDIO_ADDR                            (PERIPH_BASE + 0x00018000UL)
#define mSDIO                                ((SDIO_TypeDef *)SDIO_ADDR)
#define AHBPERIPH_BASE                       (PERIPH_BASE + 0x00020000UL)
#define DMA2_Channel4_BASE                   (AHBPERIPH_BASE + 0x00000444UL)
#define DMA2_Channel4                        ((DMA_Channel_TypeDef *)DMA2_Channel4_BASE)

#define SDMMC_VOLTAGE_WINDOW_SD              0x80100000U
#define SDMMC_HIGH_CAPACITY                  0x40000000U
#define SDMMC_STD_CAPACITY                   0x00000000U
#define SDMMC_CHECK_PATTERN                  0x000001AAU
#define SDMMC_MAX_VOLT_TRIAL                 0x00000FFFU
#define SDIO_CARD_TRANSFER                   0x00000004U    /* Card is in transfer state */
#define SDIO_CARD_ERROR                      0x000000FFU    /* Card response Error */
#define SDMMC_ACMD_APP_SD_SET_BUSWIDTH                ((uint8_t)6)   /* (ACMD6) Defines the data bus width to be used for data transfer. The allowed data bus widths are given in SCR register. */
#define SDMMC_ACMD_SD_APP_OP_COND                     ((uint8_t)41)  /* (ACMD41) Sends host capacity support information (HCS) and asks the accessed card to send its operating condition register (OCR) content in the response on the CMD line. */
#define SDMMC_ACMD_SD_APP_SET_CLR_CARD_DETECT         ((uint8_t)42)  /* (ACMD42) Connect/Disconnect the 50 KOhm pull-up resistor on CD/DAT3 (pin 1) of the card  */

#define CMD0_GO_IDLE_STATE                            (uint16_t)(0UL | SDIO_CMD_WAIT_NO_RESP)
#define CMD2_ALL_SEND_CID                             (uint16_t)(2UL | SDIO_CMD_WAIT_LONG_RESP)
#define CMD3_SET_REL_ADDR                             (uint16_t)(3UL | SDIO_CMD_WAIT_SHORT_RESP)
#define CMD7_SEL_DESEL_CARD                           (uint16_t)(7UL | SDIO_CMD_WAIT_SHORT_RESP)
#define CMD8_HS_SEND_EXT_CSD                          (uint16_t)(8UL | SDIO_CMD_WAIT_SHORT_RESP)
#define CMD9_SEND_CSD                                 (uint16_t)(9UL | SDIO_CMD_WAIT_LONG_RESP)
#define CMD12_STOP_TRANSFER                           (uint16_t)(12UL | SDIO_CMD_WAIT_SHORT_RESP)
#define CMD13_SEND_STATUS                             (uint16_t)(13UL | SDIO_CMD_WAIT_SHORT_RESP)
#define CMD16_SEND_STATUS                             (uint16_t)(16UL | SDIO_CMD_WAIT_SHORT_RESP)
#define CMD17_READ_SINGLE_BLOCK                       (uint16_t)(17UL | SDIO_CMD_WAIT_SHORT_RESP)
#define CMD24_WRITE_SINGLE_BLOCK                      (uint16_t)(24UL | SDIO_CMD_WAIT_SHORT_RESP)
#define CMD55_APP_CMD                                 (uint16_t)(55UL | SDIO_CMD_WAIT_SHORT_RESP)
#define ACMD6_APP_SD_SET_BUSWIDTH                     (uint16_t)(SDMMC_ACMD_APP_SD_SET_BUSWIDTH | SDIO_CMD_WAIT_SHORT_RESP)
#define ACMD41_SD_APP_OP_COND                         (uint16_t)(SDMMC_ACMD_SD_APP_OP_COND | SDIO_CMD_WAIT_SHORT_RESP)
#define ACMD42_SD_APP_SET_CLR_CARD_DETECT             (uint16_t)(SDMMC_ACMD_SD_APP_SET_CLR_CARD_DETECT | SDIO_CMD_WAIT_SHORT_RESP)

#define RET_ERROR()   do{step = 100; return 2;}while(0)
#define RET_OK()      do{step = 99;  return 1;}while(0)
#define WRET_ERROR()  do{wstep = 100;return 2;}while(0)
#define WRET_OK()     do{wstep = 99; return 1;}while(0)
#define SDIO_GetResponse(x)        (*(volatile uint32_t *)(SDIO_RESP_ADDR + x))
#define SDIO_GetCommandResponse()  (uint8_t)(mSDIO->RESPCMD)

#define SDIO_GET_FLAG(__FLAG__)              !!((mSDIO->STA) & (__FLAG__))
#define SDIO_CLEAR_FLAG(__FLAG__)            (mSDIO->ICR = (__FLAG__))

#define SDIO_ICR_CMD_FLAGS   (SDIO_ICR_CEATAENDC | SDIO_ICR_SDIOITC | SDIO_ICR_CMDSENTC | SDIO_ICR_CMDRENDC | SDIO_ICR_CTIMEOUTC | SDIO_ICR_CCRCFAILC)
#define SDIO_ICR_DATA_FLAGS  (SDIO_ICR_DBCKENDC | SDIO_ICR_STBITERRC | SDIO_ICR_DATAENDC | SDIO_ICR_RXOVERRC | SDIO_ICR_TXUNDERRC | SDIO_ICR_DTIMEOUTC | SDIO_ICR_DCRCFAILC)

static uint8_t wstep,step,times;
static uint32_t m_count;
static int tdelay;
static SDIO_CardInfo info;
//version
#define CARD_V2_X         1
#define CARD_V1_X         0
//type
#define CARD_SDHC_SDXC    1
#define CARD_SDSC         0

static uint32_t getCmdError(void)
{
	switch (wstep){
	case 0:
		wstep++;
		m_count = 1 + (SDIO_CMDTIMEOUT) * (F_HCLK / 8U / 1000U);
		break;
	case 1:
		if (!--m_count) WRET_ERROR();
		if(SDIO_GET_FLAG(SDIO_STA_CMDSENT)){
  			SDIO_CLEAR_FLAG(SDIO_ICR_CMD_FLAGS);
			WRET_OK();
		}
		break;
	case 99:return 1;
	case 100:return 2;
	}
	return 0;
}
#define SDMMC_ALLZERO                        0x00000000U
#define SDMMC_OCR_ERRORBITS                  0xFDFFE008U
static uint32_t getCmdResp1(uint8_t command) {
	switch (wstep){
	case 0:
		wstep++;
		m_count = 1 + (SDIO_CMDTIMEOUT) * (F_HCLK / 8U / 1000U);
		break;
	case 1:
		if (!--m_count) {
			WRET_ERROR();
		}
		if(SDIO_GET_FLAG(SDIO_STA_CCRCFAIL | SDIO_STA_CMDREND | SDIO_STA_CTIMEOUT)){
  			if (SDIO_GET_FLAG(SDIO_STA_CCRCFAIL | SDIO_STA_CTIMEOUT)) {
				SDIO_CLEAR_FLAG(SDIO_STA_CCRCFAIL | SDIO_STA_CTIMEOUT);
				WRET_ERROR();
			}
			if (SDIO_GetCommandResponse() != command)WRET_ERROR();
			SDIO_CLEAR_FLAG(SDIO_ICR_CMD_FLAGS);
			if((SDIO_GetResponse(SDIO_RESP1) & SDMMC_OCR_ERRORBITS) == SDMMC_ALLZERO){
				WRET_OK();
			}
			else {
				WRET_ERROR();
			}
		}
		break;
	case 99:return 1;
	case 100:return 2;
	}
	return 0;
}

static uint32_t getCmdResp2(void) {
	switch (wstep){
	case 0:
		wstep++;
		m_count = 1 + (SDIO_CMDTIMEOUT) * (F_HCLK / 8U / 1000U);
		break;
	case 1:
		if (!--m_count) WRET_ERROR();
		if(SDIO_GET_FLAG(SDIO_STA_CCRCFAIL | SDIO_STA_CMDREND | SDIO_STA_CTIMEOUT)){
  			if (SDIO_GET_FLAG(SDIO_STA_CCRCFAIL | SDIO_STA_CTIMEOUT)) {
				SDIO_CLEAR_FLAG(SDIO_STA_CCRCFAIL | SDIO_STA_CTIMEOUT);
				WRET_ERROR();
			}
			SDIO_CLEAR_FLAG(SDIO_ICR_CMD_FLAGS);
			WRET_OK();
		}
		break;
	case 99:return 1;
	case 100:return 2;
	}
	return 0;
}

static uint32_t getCmdResp3(void) {
	switch (wstep){
	case 0:
		wstep++;
		m_count = 1 + (SDIO_CMDTIMEOUT) * (F_HCLK / 8U / 1000U);
		break;
	case 1:
		if (!--m_count) WRET_ERROR();
		if(SDIO_GET_FLAG(SDIO_STA_CCRCFAIL | SDIO_STA_CMDREND | SDIO_STA_CTIMEOUT)){
  			if (SDIO_GET_FLAG(SDIO_STA_CTIMEOUT)) {
				SDIO_CLEAR_FLAG(SDIO_STA_CTIMEOUT);
				WRET_ERROR();
			}
			SDIO_CLEAR_FLAG(SDIO_ICR_CMD_FLAGS);
			WRET_OK();
		}
		break;
	case 99:return 1;
	case 100:return 2;
	}
	return 0;
}

#define SDMMC_R6_GENERAL_UNKNOWN_ERROR       0x00002000U
#define SDMMC_R6_ILLEGAL_CMD                 0x00004000U
#define SDMMC_R6_COM_CRC_FAILED              0x00008000U
static uint32_t getCmdResp6(uint8_t command, uint32_t *rca) {
	switch (wstep){
	case 0:
		wstep++;
		m_count = 1 + (SDIO_CMDTIMEOUT) * (F_HCLK / 8U / 1000U);
		break;
	case 1:
		if (!--m_count) WRET_ERROR();
		if(SDIO_GET_FLAG(SDIO_STA_CCRCFAIL | SDIO_STA_CMDREND | SDIO_STA_CTIMEOUT)){
  			if (SDIO_GET_FLAG(SDIO_STA_CCRCFAIL | SDIO_STA_CTIMEOUT)) {
				SDIO_CLEAR_FLAG(SDIO_STA_CCRCFAIL | SDIO_STA_CTIMEOUT);
				WRET_ERROR();
			}
			if (SDIO_GetCommandResponse() != command) WRET_ERROR();
			SDIO_CLEAR_FLAG(SDIO_ICR_CMD_FLAGS);
			if (SDIO_GetResponse(SDIO_RESP1) & (SDMMC_R6_GENERAL_UNKNOWN_ERROR | SDMMC_R6_ILLEGAL_CMD | SDMMC_R6_COM_CRC_FAILED))WRET_ERROR();
			*rca = SDIO_GetResponse(SDIO_RESP1) >> 16;
			WRET_OK();
		}
		break;
	case 99: return 1;
	case 100:return 2;
	}
	return 0;
}

#define SDIO_WAIT(FLAGS) do{ \
  uint32_t total = 1 + (SDIO_CMDTIMEOUT) * (F_HCLK / 8U / 1000U); \
  do { if (!--total) return false; } while (!SDIO_GET_FLAG(FLAGS)); \
}while(0)
static bool getCmdResp7(void)
{
  SDIO_WAIT(SDIO_STA_CCRCFAIL | SDIO_STA_CMDREND | SDIO_STA_CTIMEOUT);
  if (SDIO_GET_FLAG(SDIO_STA_CTIMEOUT)) {
    SDIO_CLEAR_FLAG(SDIO_STA_CTIMEOUT);
    return false;
  }
  if (SDIO_GET_FLAG(SDIO_STA_CMDREND)) { SDIO_CLEAR_FLAG(SDIO_STA_CMDREND); }
  return true;
}

inline void start(){step = 0;times = 0;}
inline void reset(){RCC_BASE->AHBENR &= ~(1UL << 10);}
inline void powerOn(){mSDIO->POWER = 3UL << 0;myDelayUs(20);}
inline void powerOff(){mSDIO->POWER = 0;myDelayUs(20);}
inline void enable(){mSDIO->CLKCR |= SDIO_CLKCR_CLKEN;myDelayUs(tdelay);}
inline void disable(){mSDIO->CLKCR &= ~(uint32_t)SDIO_CLKCR_CLKEN;myDelayUs(tdelay);}
inline void sendCommand(uint16_t command, uint32_t argument) { mSDIO->ARG = argument; mSDIO->CMD = (uint32_t)(SDIO_CMD_CPSMEN | command);}
inline bool goIdleState() { sendCommand(CMD0_GO_IDLE_STATE, 0); return getCmdError(); }
inline bool sendCID() { sendCommand(CMD2_ALL_SEND_CID, 0); return getCmdResp2(); }
inline bool setRelAdd(uint32_t *rca) { sendCommand(CMD3_SET_REL_ADDR, 0); return getCmdResp6(3, rca); }
inline bool selDesel(uint32_t address) { sendCommand(CMD7_SEL_DESEL_CARD, address); return getCmdResp1(7); }
inline bool operCond() { sendCommand(CMD8_HS_SEND_EXT_CSD, SDMMC_CHECK_PATTERN); return getCmdResp7(); }
inline bool sendCSD(uint32_t argument) { sendCommand(CMD9_SEND_CSD, argument); return getCmdResp2(); }
inline bool sendStatus(uint32_t argument) { sendCommand(CMD13_SEND_STATUS, argument); return getCmdResp1(13); }
inline bool sendSize(uint32_t argument){sendCommand(CMD16_SEND_STATUS, argument); return getCmdResp1(16); }
inline bool readSingleBlock(uint32_t address) { sendCommand(CMD17_READ_SINGLE_BLOCK, address); return getCmdResp1(17); }
inline bool writeSingleBlock(uint32_t address) { sendCommand(CMD24_WRITE_SINGLE_BLOCK, address); return getCmdResp1(24); }
inline bool stopTransfer(void){sendCommand(CMD12_STOP_TRANSFER, 0);return getCmdResp1(12);}
inline bool appCommand(uint32_t rsa) {sendCommand(CMD55_APP_CMD, rsa); return getCmdResp1(55); }

static inline void setTransfer(uint32_t dtimer, uint32_t dlen, uint16_t flags)
{
	mSDIO->ICR = SDIO_ICR_DATA_FLAGS; // clear data access relevant flags
	mSDIO->DTIMER = dtimer;
	mSDIO->DLEN = dlen;
	mSDIO->DCTRL = flags;//SDIO_DCTRL_DTEN; // enable data transfer
}
static void setClk(uint32_t clk)
{
	if (clk < 1000000)tdelay = 20;
	else tdelay = 2;
	uint32_t div = 48000000 / clk + (48000000 % clk != 0) - 2;
	disable();
	mSDIO->CLKCR = (mSDIO->CLKCR & (~(SDIO_CLKCR_CLKDIV|SDIO_CLKCR_BYPASS))) | SDIO_CLKCR_PWRSAV | SDIO_CLKCR_HWFC_EN | SDIO_CLKCR_CLKEN  | (div & SDIO_CLKCR_CLKDIV);
	myDelayUs(tdelay);
}
static void setDbusWidth(uint16_t w)
{
	mSDIO->CLKCR = (mSDIO->CLKCR & (~SDIO_CLKCR_WIDBUS)) | w;
	myDelayUs(tdelay);
}
static uint32_t _init(void)
{
	uint32_t r;
	switch(step){
	case 0:
		//SDIO IO口初始化
		RCC_BASE->AHBENR |= 1UL << 10;//使能SDIO时钟
		powerOn();
		
		RCC_BASE->APB2ENR |= 1 << 4;    	//使能PORTC时钟	   	 
		RCC_BASE->APB2ENR |= 1 << 5;    	//使能PORTD时钟
		GPIOC_BASE->CRH &= 0XFFF00000;
		GPIOC_BASE->CRH |= 0X000BBBBB;	//PC.8~12 复用输出
		GPIOD_BASE->CRL &= 0XFFFFF0FF; 
		GPIOD_BASE->CRL |= 0X00000B00;	//PD2复用输出,PD7 上拉输入
		
		setClk(400000);
		myDelayUs(200);
		setDbusWidth(SDIO_CLKCR_WIDBUS_1BIT);
		sendCommand(CMD0_GO_IDLE_STATE, 0);//发送CMD0使SD卡进入IDLE状态
		step++;
		wstep = 0;
		break;
	case 1:
		r = getCmdError();
		if(r == 2)RET_ERROR();
		if(r == 0)return 0;
		if(times == 0){ //再次确认
			wstep = 0;
			times++;
			sendCommand(CMD0_GO_IDLE_STATE, 0);
		}
		else{
			step++;
			m_count = 0;
			info.version = operCond() ? CARD_V2_X : CARD_V1_X;
		}
		break;
	case 2:
		step++;
		wstep = 0;
		sendCommand(CMD55_APP_CMD, 0);
		break;
	case 3:
		r = getCmdResp1(55);
		if(r == 2)RET_ERROR();
		if(r == 0){
			SERIAL_ECHO("wstep:");
			SERIAL_ECHOLN(wstep);
			return 0;
		}
		step++;
		wstep = 0;
		sendCommand(ACMD41_SD_APP_OP_COND, SDMMC_VOLTAGE_WINDOW_SD | (info.version == CARD_V2_X ? SDMMC_HIGH_CAPACITY : SDMMC_STD_CAPACITY));
		break;
	case 4:
		r = getCmdResp3();
		if(r == 2)RET_ERROR();
		if(r == 0)break;
		if(SDIO_GetResponse(SDIO_RESP1) & 0x80000000){
			step++;
			info.type = (SDIO_GetResponse(SDIO_RESP1) & SDMMC_HIGH_CAPACITY) ? CARD_SDHC_SDXC : CARD_SDSC;
			wstep = 0;
			sendCommand(CMD2_ALL_SEND_CID, 0);//sendCID
		}
		else{
			if (m_count++ == SDMMC_MAX_VOLT_TRIAL)RET_ERROR();
			else step = 2;
		}
		break;
	case 5:
		r = getCmdResp2();
		if(r == 2)RET_ERROR();
		if(r == 0)return 0;
		wstep = 0;
		step++;
		sendCommand(CMD3_SET_REL_ADDR, 0);/* Send 3 SET_REL_ADDR with argument 0. SD Card publishes its RCA. */
		break;
	case 6:
		r = getCmdResp6(3, &info.RelCardAdd);
		if(r == 2)RET_ERROR();
		if(r == 0)return 0;
		wstep = 0;
		step++;
		sendCommand(CMD9_SEND_CSD, info.RelCardAdd << 16U);//sendCSD
		break;
	case 7:
		r = getCmdResp2();
		if(r == 2)RET_ERROR();
		if(r == 0)return 0;
		info.Class = (SDIO_GetResponse(SDIO_RESP2) >> 20U);
		if (info.type == CARD_SDHC_SDXC) {
			info.LogBlockNbr = info.BlockNbr = (((SDIO_GetResponse(SDIO_RESP2) & 0x0000003FU) << 26U) | ((SDIO_GetResponse(SDIO_RESP3) & 0xFFFF0000U) >> 6U)) + 1024;
			info.LogBlockSize = info.BlockSize = 512U;
		}
		else {
			info.BlockNbr  = ((((SDIO_GetResponse(SDIO_RESP2) & 0x000003FFU) << 2U ) | ((SDIO_GetResponse(SDIO_RESP3) & 0xC0000000U) >> 30U)) + 1U) * (4U << ((SDIO_GetResponse(SDIO_RESP3) & 0x00038000U) >> 15U));
			info.BlockSize = 1U << ((SDIO_GetResponse(SDIO_RESP2) >> 16) & 0x0FU);
			info.LogBlockNbr =  (info.BlockNbr) * ((info.BlockSize) / 512U);
			info.LogBlockSize = 512U;
		}
		wstep = 0;
		step++;
		sendCommand(CMD7_SEL_DESEL_CARD, info.RelCardAdd << 16U);//selDesel
		break;
	case 8:
		r = getCmdResp1(7);
		if(r == 2)RET_ERROR();
		if(r == 0)return 0;
		wstep = 0;
		step++;
		sendCommand(CMD55_APP_CMD, info.RelCardAdd << 16U);
		break;
	case 9:
		r = getCmdResp1(55);
		if(r == 2)RET_ERROR();
		if(r == 0)return 0;
		wstep = 0;
		step++;
		sendCommand(ACMD42_SD_APP_SET_CLR_CARD_DETECT, 0);
		break;
	case 10:
		r = getCmdResp2();
		if(r == 2)RET_ERROR();
		if(r == 0)return 0;
		wstep = 0;
		step++;
		sendCommand(CMD55_APP_CMD, info.RelCardAdd << 16U);
		break;
	case 11:
		r = getCmdResp1(55);
		if(r == 2)RET_ERROR();
		if(r == 0)return 0;
		wstep = 0;
		step++;
		sendCommand(ACMD6_APP_SD_SET_BUSWIDTH, 2);
		break;
	case 12:
		r = getCmdResp2();
		if(r == 2)RET_ERROR();
		if(r == 0)return 0;
		wstep = 0;
		step = 99;
		setDbusWidth(SDIO_CLKCR_WIDBUS_4BIT);
		setClk(4500000);
		RET_OK();
	case 99:return 1;
	case 100:return 2;
	}
	return 0;
}
#define CCR_CLEAR_Mask           ((uint32_t)0xFFFF800F)
void yDMAInit(int memDataBit,bool memInc,int perDataBit,bool perInc)
{
	RCC_BASE->AHBENR |= 1 << 1;//DMA2
	// RCC_BASE->AHBENR |= 1 << 0;//DMA1
	// DMA_DeInit(DMA_CHx);   //将DMA的通道1寄存器重设为缺省值
	uint32_t temp = DMA2_Channel4->CCR;
	temp &= CCR_CLEAR_Mask;
	if(perInc)temp |= 1UL << 6;//外设地址递增
	if(memInc)temp |= 1UL << 7;  //内存地址寄存器递增
	if(perDataBit == 16)temp |= 1UL << 8;//外设数据位
	else if(perDataBit == 32)temp |= 1UL << 9;
	if(memDataBit == 16)temp |= 1UL << 10;//内存数据位
	else if(memDataBit == 32)temp |= 1UL << 11;
	temp |= 1UL << 12; //DMA通道 x拥有中优先级
	/* Write to DMAy Channelx CCR */
	DMA2_Channel4->CCR = temp;
}

void yDMAStart(uint32_t mem,uint32_t per,uint16_t len,uint8_t mode)
{
	DMA2_Channel4->CCR &= 0xFFFE;//关闭USART1 TX DMA1 所指示的通道
	DMA2_Channel4->CPAR = per;
	DMA2_Channel4->CMAR = mem;
	DMA2_Channel4->CNDTR = len;//DMA通道的DMA缓存的大小
	if(mode == 2){
		DMA2_Channel4->CCR |= (1UL << 14);//开启mem->mem模式
		DMA2_Channel4->CCR &= ~(1UL << 4);
	}
	else {
		DMA2_Channel4->CCR &= ~(1UL << 14);
		if(mode == 1)DMA2_Channel4->CCR |= 1UL << 4; //mem->per
		else DMA2_Channel4->CCR &= ~(1UL << 4);
	}
	DMA2_Channel4->CCR |= 0x0001;//使能USART1 TX DMA1 所指示的通道
}

static void useSdioDMA(void)
{
	yDMAInit(32,true,32,false);//RX
	mSDIO->DCTRL &= ~((uint32_t)1 << 3);	 //SDIO DMA使能,如此可以使用正常的SDIO通信,在需要DMA发送时再使能
	DMA2_Channel4->CCR &= 0xFFFE;//失能DMA
}
#define HAL_SD_CARD_READY          0x00000001U  /*!< Card state is ready                     */
#define HAL_SD_CARD_IDENTIFICATION 0x00000002U  /*!< Card is in identification state         */
#define HAL_SD_CARD_STANDBY        0x00000003U  /*!< Card is in standby state                */
#define HAL_SD_CARD_TRANSFER       0x00000004U  /*!< Card is in transfer state               */
#define HAL_SD_CARD_SENDING        0x00000005U  /*!< Card is sending an operation            */
#define HAL_SD_CARD_RECEIVING      0x00000006U  /*!< Card is receiving operation information */
#define HAL_SD_CARD_PROGRAMMING    0x00000007U  /*!< Card is in programming state            */
#define HAL_SD_CARD_DISCONNECTED   0x00000008U  /*!< Card is disconnected                    */
static uint32_t _writeBlock(uint32_t blockAddress, const uint8_t *data)
{
	uint32_t r;
	switch(step){
	case 0:
		wstep = 0;
		step++;
		sendCommand(CMD13_SEND_STATUS, info.RelCardAdd << 16U);//获取状态
		break;
	case 1:
		r = getCmdResp1(13);
		if(r == 2)RET_ERROR();
		if(r == 0)return 0;
		r = (SDIO_GetResponse(SDIO_RESP1) >> 9U) & 0x0FU;
		if(r == SDIO_CARD_TRANSFER){//SD卡进入数据传输状态
			if (blockAddress >= info.LogBlockNbr || (0x03 & (uint32_t)data))RET_ERROR(); //misaligned data
			step++;
		}
		else{
			if(times < 8){//重试
				step = 0;
				myDelayUs(800);
				times++;
			}
			else RET_ERROR();
		}
		break;
	case 2:
		if (info.type != CARD_SDHC_SDXC) { blockAddress *= 512U; }
		DMA2_BASE->IFCR |= 0x0F << 3;//清除所有异常标志
		yDMAStart((uint32_t)data,(uint32_t)&mSDIO->FIFO,128,1);//一块是512字节,但是DMA采用32位传输,所以相当于128个32位
		step++;
		wstep = 0;
		sendCommand(CMD24_WRITE_SINGLE_BLOCK, blockAddress);
		break;
	case 3:
		r = getCmdResp1(24);
		if(r == 0)break;
		if(r == 2){
			SDIO_CLEAR_FLAG(SDIO_ICR_CMD_FLAGS);
			DMA2_Channel4->CCR &= 0xFFFE;//失能DMA
			mSDIO->DCTRL &= ~(uint32_t)(SDIO_DCTRL_DMAEN | SDIO_DCTRL_DTEN);
			RET_ERROR();
		}
		step++;
		setTransfer(SDIO_DATA_TIMEOUT * (F_HCLK / 1000U), 512, SDIO_BLOCKSIZE_512 | SDIO_DCTRL_DMAEN | SDIO_DCTRL_DTEN | SDIO_DIR_TX);
		break;
	case 4:
		if(SDIO_GET_FLAG(SDIO_STA_DATAEND | SDIO_STA_TRX_ERROR_FLAGS)){
			if (mSDIO->STA & SDIO_STA_TRX_ERROR_FLAGS) {//If there were SDIO errors, do not wait DMA.
				SDIO_CLEAR_FLAG(SDIO_ICR_CMD_FLAGS | SDIO_ICR_DATA_FLAGS);
				DMA2_Channel4->CCR &= 0xFFFE;//失能DMA
				mSDIO->DCTRL &= ~(uint32_t)(SDIO_DCTRL_DMAEN | SDIO_DCTRL_DTEN);
				RET_ERROR();
			}
			step++;
		}
		break;
	case 5:
		if(DMA2_BASE->ISR & (DMA_ISR_TEIF4 | DMA_ISR_TCIF4)){//Wait for DMA transaction to complete
			if (DMA2_BASE->ISR & DMA_ISR_TEIF4) {
				DMA2_Channel4->CCR &= 0xFFFE;//失能DMA
				mSDIO->DCTRL &= ~(uint32_t)(SDIO_DCTRL_DMAEN | SDIO_DCTRL_DTEN);
				SDIO_CLEAR_FLAG(SDIO_ICR_CMD_FLAGS | SDIO_ICR_DATA_FLAGS);
				RET_ERROR();
			}
			DMA2_Channel4->CCR &= 0xFFFE;//失能DMA
			mSDIO->DCTRL &= ~(uint32_t)(SDIO_DCTRL_DMAEN | SDIO_DCTRL_DTEN);
			if (mSDIO->STA & SDIO_STA_TXDAVL) {
				while (mSDIO->STA & SDIO_STA_RXDAVL) (void)mSDIO->FIFO;
				SDIO_CLEAR_FLAG(SDIO_ICR_CMD_FLAGS | SDIO_ICR_DATA_FLAGS);
				RET_ERROR();
			}
			if (SDIO_GET_FLAG(SDIO_STA_TRX_ERROR_FLAGS)) {
				SDIO_CLEAR_FLAG(SDIO_ICR_CMD_FLAGS | SDIO_ICR_DATA_FLAGS);
				RET_ERROR();
			}
			SDIO_CLEAR_FLAG(SDIO_ICR_CMD_FLAGS | SDIO_ICR_DATA_FLAGS);
			// RET_OK();//如果不需要判断SD卡是否programming,则这里直接RET_OK();
			step++;
		}
		break;
	case 6://6--7判断SD卡是否编程完毕
		sendCommand(CMD13_SEND_STATUS, info.RelCardAdd << 16U);//获取状态
		step++;
		break;
	case 7:
		if(mSDIO->STA & ((1 << 0) | (1 << 6) | (1 << 2)))//等待操作完成
		{
			if(mSDIO->STA & (1 << 0)) //CRC检测失败
			{
				mSDIO->ICR |= 1 << 0;		//清除错误标记
				RET_ERROR();
			}
			if(mSDIO->STA & (1 << 2)) //命令超时 
			{
				mSDIO->ICR |= 1 << 2; //清除错误标记
				RET_ERROR();
			}
			if(mSDIO->RESPCMD != 13U)RET_ERROR();
			mSDIO->ICR = 0X5FF; //清除所有标记
			r = mSDIO->RESP1;
			r = (r >> 9) & 0x0000000F;
			if(r == HAL_SD_CARD_RECEIVING || r == HAL_SD_CARD_SENDING || r == HAL_SD_CARD_PROGRAMMING)step = 6;
			else RET_OK();
		}
		break;
	case 99: return 1;
	case 100:return 2;
	}
	return 0;
}
static uint32_t _readBlock(uint32_t blockAddress, uint8_t *data)
{
	int r;
	switch(step){
	case 0:
		wstep = 0;
		step++;
		sendCommand(CMD13_SEND_STATUS, info.RelCardAdd << 16U);//获取状态
		break;
	case 1:
		r = getCmdResp1(13);
		if(r == 2)RET_ERROR();
		if(r == 0)return 0;
		r = (SDIO_GetResponse(SDIO_RESP1) >> 9U) & 0x0FU;
		if(r == SDIO_CARD_TRANSFER){
			if (blockAddress >= info.LogBlockNbr || (0x03 & (uint32_t)data))RET_ERROR(); //misaligned data
			step++;
		}
		else{
			if(times < 8){//重试
				step = 0;
				myDelayUs(800);
				times++;
			}
			else RET_ERROR();
		}
		break;
	case 2:
		if (info.type != CARD_SDHC_SDXC) { blockAddress *= 512U; }
		DMA2_BASE->IFCR |= 0x0F << 3;//清除所有异常标志
		yDMAStart((uint32_t)data,(uint32_t)&mSDIO->FIFO,128,0);//一块是512字节,但是DMA采用32位传输,所以相当于128个32位
		setTransfer(SDIO_DATA_TIMEOUT * (F_HCLK / 1000U), 512, SDIO_BLOCKSIZE_512 | SDIO_DCTRL_DMAEN | SDIO_DCTRL_DTEN | SDIO_DIR_RX);
		wstep = 0;
		step++;
		sendCommand(CMD17_READ_SINGLE_BLOCK, blockAddress);
		break;
	case 3:
		r = getCmdResp1(17);
		if(r == 0)break;
		if(r == 2){
			SDIO_CLEAR_FLAG(SDIO_ICR_CMD_FLAGS);
			DMA2_Channel4->CCR &= 0xFFFE;//失能DMA
			mSDIO->DCTRL &= ~(uint32_t)(SDIO_DCTRL_DMAEN | SDIO_DCTRL_DTEN);
			RET_ERROR();
		}
		step++;
		break;
	case 4:
		if(SDIO_GET_FLAG(SDIO_STA_DATAEND | SDIO_STA_TRX_ERROR_FLAGS)){
			if (mSDIO->STA & SDIO_STA_TRX_ERROR_FLAGS) {//If there were SDIO errors, do not wait DMA.
				SDIO_CLEAR_FLAG(SDIO_ICR_CMD_FLAGS | SDIO_ICR_DATA_FLAGS);
				DMA2_Channel4->CCR &= 0xFFFE;//失能DMA
				mSDIO->DCTRL &= ~(uint32_t)(SDIO_DCTRL_DMAEN | SDIO_DCTRL_DTEN);
				RET_ERROR();
			}
			step++;
		}
		break;
	case 5:
		if(DMA2_BASE->ISR & (DMA_ISR_TEIF4 | DMA_ISR_TCIF4)){//Wait for DMA transaction to complete
			if (DMA2_BASE->ISR & DMA_ISR_TEIF4) {
				DMA2_Channel4->CCR &= 0xFFFE;//失能DMA
				mSDIO->DCTRL &= ~(uint32_t)(SDIO_DCTRL_DMAEN | SDIO_DCTRL_DTEN);
				SDIO_CLEAR_FLAG(SDIO_ICR_CMD_FLAGS | SDIO_ICR_DATA_FLAGS);
				RET_ERROR();
			}
			DMA2_Channel4->CCR &= 0xFFFE;//失能DMA
			mSDIO->DCTRL &= ~(uint32_t)(SDIO_DCTRL_DMAEN | SDIO_DCTRL_DTEN);
			if (mSDIO->STA & SDIO_STA_RXDAVL) {
				while (mSDIO->STA & SDIO_STA_RXDAVL) (void)mSDIO->FIFO;
				SDIO_CLEAR_FLAG(SDIO_ICR_CMD_FLAGS | SDIO_ICR_DATA_FLAGS);
				RET_ERROR();
			}
			if (SDIO_GET_FLAG(SDIO_STA_TRX_ERROR_FLAGS)) {
				SDIO_CLEAR_FLAG(SDIO_ICR_CMD_FLAGS | SDIO_ICR_DATA_FLAGS);
				RET_ERROR();
			}
			SDIO_CLEAR_FLAG(SDIO_ICR_CMD_FLAGS | SDIO_ICR_DATA_FLAGS);
			RET_OK();
		}
		break;
	case 99: return 1;
	case 100:return 2;
	}
	return 0;
}
bool SDIO_Init()
{
	uint32_t r;
	useSdioDMA();
	start();
	do{
		watchdog_refresh();
		r = _init();
	}while(r == 0);
	return r == 1;
}
bool SDIO_WriteBlock(uint32_t block, const uint8_t *src)
{
	uint32_t r;
	start();
	do{
		r = _writeBlock(block, src);
		watchdog_refresh();
	}while(r == 0);
	return r == 1;
}
bool SDIO_ReadBlock(uint32_t block, uint8_t *dst)
{
	uint32_t r;
	start();
	do{
		r = _readBlock(block, dst);
		watchdog_refresh();
	}while(r == 0);
	return r == 1;
}
#endif
