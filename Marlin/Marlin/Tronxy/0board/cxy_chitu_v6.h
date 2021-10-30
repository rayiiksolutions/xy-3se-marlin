#pragma once

#include "src/inc/MarlinConfig.h"
#include "src/HAL/STM32F1/SPI.h"
#include "src/HAL/STM32F1/tft/xpt2046.h"
#include "flash_stm32f1xx.hpp"

typedef struct {
  __IO uint16_t REG;
  __IO uint16_t RAM;
  } FSMC_LCD_TypeDef;
extern  FSMC_LCD_TypeDef *mLCD;

#define __ASM __asm
#define __STATIC_INLINE static inline
__attribute__((always_inline)) __STATIC_INLINE void __DSB() {
  __ASM volatile ("dsb 0xF":::"memory");
}
extern bool _lcd_writing;

void myDelayUs(uint32_t n);
int LCD_McuInit(void);
void LCD_Reset(void);
#define LCD_WReg(v)     mLCD->REG = (v);__DSB()
#define LCD_WData(v)    mLCD->RAM = (v);__DSB()
#define LCD_RData(v)    v = mLCD->RAM;__DSB()
#define LCD_DelayMs     _delay_ms
#define LCD_BatchOk()   (_lcd_writing ? \
((dma_get_isr_bits(FSMC_DMA_DEV, FSMC_DMA_CHANNEL) & 0x0A) ? (dma_disable(FSMC_DMA_DEV, FSMC_DMA_CHANNEL),_lcd_writing = false,true) : false) : true)
#define LCD_BatchStart(colors,len,autoInc)  do{\
if(autoInc)dma_setup_transfer(FSMC_DMA_DEV, FSMC_DMA_CHANNEL, (volatile void*)(colors), DMA_SIZE_16BITS, &mLCD->RAM, DMA_SIZE_16BITS, DMA_MEM_2_MEM | DMA_PINC_MODE);\
else dma_setup_transfer(FSMC_DMA_DEV, FSMC_DMA_CHANNEL, (volatile void*)(colors), DMA_SIZE_16BITS, &mLCD->RAM, DMA_SIZE_16BITS, DMA_MEM_2_MEM);\
dma_set_num_transfers(FSMC_DMA_DEV, FSMC_DMA_CHANNEL, len);\
dma_clear_isr_bits(FSMC_DMA_DEV, FSMC_DMA_CHANNEL);\
dma_enable(FSMC_DMA_DEV, FSMC_DMA_CHANNEL);\
_lcd_writing = true;}while(0)
#define LCD_BatchMaxNum            65535
void LCD_BatchSyn(const void*colors,int len,bool autoInc);

void W25Q16McuInit(void);
#define EFlashMcuInit                    W25Q16McuInit
#define EFlashCs()                       WRITE(SS_PIN,LOW)
#define EFlashUncs()                     WRITE(SS_PIN,HIGH)
#define EFlashSpiSendByte(byte)          SPI.transfer(byte)
#define EFlashSpiBatchRead(buf,nbyte)    SPI.dmaTransferStart(0, const_cast<uint8_t*>(buf), nbyte)
#define EFlashSpiBatchReadOk()           SPI.dmaTransferOk()

