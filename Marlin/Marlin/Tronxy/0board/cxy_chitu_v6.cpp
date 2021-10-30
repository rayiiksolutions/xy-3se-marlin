#include "cxy_chitu_v6.h"

#include <libmaple/dma.h>
#include <libmaple/fsmc.h>
bool _lcd_writing = false;
FSMC_LCD_TypeDef* mLCD;
#define setPinOutput(IO)    gpio_set_mode(PIN_MAP[IO].gpio_device, PIN_MAP[IO].gpio_bit, GPIO_OUTPUT_PP)

#define FSMC_CS_NE1 PD7
#define FSMC_CS_NE2 PG9
#define FSMC_CS_NE3 PG10
#define FSMC_CS_NE4 PG12

#define FSMC_RS_A0  PF0
#define FSMC_RS_A1  PF1
#define FSMC_RS_A2  PF2
#define FSMC_RS_A3  PF3
#define FSMC_RS_A4  PF4
#define FSMC_RS_A5  PF5
#define FSMC_RS_A6  PF12
#define FSMC_RS_A7  PF13
#define FSMC_RS_A8  PF14
#define FSMC_RS_A9  PF15
#define FSMC_RS_A10 PG0
#define FSMC_RS_A11 PG1
#define FSMC_RS_A12 PG2
#define FSMC_RS_A13 PG3
#define FSMC_RS_A14 PG4
#define FSMC_RS_A15 PG5
#define FSMC_RS_A16 PD11
#define FSMC_RS_A17 PD12
#define FSMC_RS_A18 PD13
#define FSMC_RS_A19 PE3
#define FSMC_RS_A20 PE4
#define FSMC_RS_A21 PE5
#define FSMC_RS_A22 PE6
#define FSMC_RS_A23 PE2
#define FSMC_RS_A24 PG13
#define FSMC_RS_A25 PG14

#define FSMC_ADDRESS_SETUP_TIME   0  // AddressSetupTime
#define FSMC_DATA_SETUP_TIME      1  // DataSetupTime

int LCD_McuInit(void) {
    setPinOutput(LCD_BACKLIGHT_PIN);//背光灯
    WRITE(LCD_BACKLIGHT_PIN, HIGH);
    LCD_Reset();

    dma_init(FSMC_DMA_DEV);
    dma_disable(FSMC_DMA_DEV, FSMC_DMA_CHANNEL);
    dma_set_priority(FSMC_DMA_DEV, FSMC_DMA_CHANNEL, DMA_PRIORITY_MEDIUM);

    uint32_t controllerAddress;
    switch(FSMC_CS_PIN){
      case FSMC_CS_NE1:
        controllerAddress = (uint32_t)FSMC_NOR_PSRAM_REGION1;
        break;
      case FSMC_CS_NE2:
        controllerAddress = (uint32_t)FSMC_NOR_PSRAM_REGION2;
        break;
      case FSMC_CS_NE3:
        controllerAddress = (uint32_t)FSMC_NOR_PSRAM_REGION3;
        break;
      case FSMC_CS_NE4:
        controllerAddress = (uint32_t)FSMC_NOR_PSRAM_REGION4;
        break;
      default: return -1;
    }
    #define _ORADDR(N) controllerAddress |= (_BV32(N) - 2)

    switch (FSMC_RS_PIN) {
    #if ENABLED(STM32_XL_DENSITY)
      case FSMC_RS_A0:  _ORADDR( 1); break;
      case FSMC_RS_A1:  _ORADDR( 2); break;
      case FSMC_RS_A2:  _ORADDR( 3); break;
      case FSMC_RS_A3:  _ORADDR( 4); break;
      case FSMC_RS_A4:  _ORADDR( 5); break;
      case FSMC_RS_A5:  _ORADDR( 6); break;
      case FSMC_RS_A6:  _ORADDR( 7); break;
      case FSMC_RS_A7:  _ORADDR( 8); break;
      case FSMC_RS_A8:  _ORADDR( 9); break;
      case FSMC_RS_A9:  _ORADDR(10); break;
      case FSMC_RS_A10: _ORADDR(11); break;
      case FSMC_RS_A11: _ORADDR(12); break;
      case FSMC_RS_A12: _ORADDR(13); break;
      case FSMC_RS_A13: _ORADDR(14); break;
      case FSMC_RS_A14: _ORADDR(15); break;
      case FSMC_RS_A15: _ORADDR(16); break;
    #endif
    case FSMC_RS_A16: _ORADDR(17); break;
    case FSMC_RS_A17: _ORADDR(18); break;
    case FSMC_RS_A18: _ORADDR(19); break;
    case FSMC_RS_A19: _ORADDR(20); break;
    case FSMC_RS_A20: _ORADDR(21); break;
    case FSMC_RS_A21: _ORADDR(22); break;
    case FSMC_RS_A22: _ORADDR(23); break;
    case FSMC_RS_A23: _ORADDR(24); break;
    #if ENABLED(STM32_XL_DENSITY)
      case FSMC_RS_A24: _ORADDR(25); break;
      case FSMC_RS_A25: _ORADDR(26); break;
    #endif
    default: return -1;
  }
    rcc_clk_enable(RCC_FSMC);

    gpio_set_mode(GPIOD, 14, GPIO_AF_OUTPUT_PP);  // FSMC_D00
    gpio_set_mode(GPIOD, 15, GPIO_AF_OUTPUT_PP);  // FSMC_D01
    gpio_set_mode(GPIOD,  0, GPIO_AF_OUTPUT_PP);  // FSMC_D02
    gpio_set_mode(GPIOD,  1, GPIO_AF_OUTPUT_PP);  // FSMC_D03
    gpio_set_mode(GPIOE,  7, GPIO_AF_OUTPUT_PP);  // FSMC_D04
    gpio_set_mode(GPIOE,  8, GPIO_AF_OUTPUT_PP);  // FSMC_D05
    gpio_set_mode(GPIOE,  9, GPIO_AF_OUTPUT_PP);  // FSMC_D06
    gpio_set_mode(GPIOE, 10, GPIO_AF_OUTPUT_PP);  // FSMC_D07
    gpio_set_mode(GPIOE, 11, GPIO_AF_OUTPUT_PP);  // FSMC_D08
    gpio_set_mode(GPIOE, 12, GPIO_AF_OUTPUT_PP);  // FSMC_D09
    gpio_set_mode(GPIOE, 13, GPIO_AF_OUTPUT_PP);  // FSMC_D10
    gpio_set_mode(GPIOE, 14, GPIO_AF_OUTPUT_PP);  // FSMC_D11
    gpio_set_mode(GPIOE, 15, GPIO_AF_OUTPUT_PP);  // FSMC_D12
    gpio_set_mode(GPIOD,  8, GPIO_AF_OUTPUT_PP);  // FSMC_D13
    gpio_set_mode(GPIOD,  9, GPIO_AF_OUTPUT_PP);  // FSMC_D14
    gpio_set_mode(GPIOD, 10, GPIO_AF_OUTPUT_PP);  // FSMC_D15

    gpio_set_mode(GPIOD,  4, GPIO_AF_OUTPUT_PP);  // FSMC_NOE
    gpio_set_mode(GPIOD,  5, GPIO_AF_OUTPUT_PP);  // FSMC_NWE

    gpio_set_mode(PIN_MAP[FSMC_CS_PIN].gpio_device, PIN_MAP[FSMC_CS_PIN].gpio_bit, GPIO_AF_OUTPUT_PP);  //FSMC_CS_NEx
    gpio_set_mode(PIN_MAP[FSMC_RS_PIN].gpio_device, PIN_MAP[FSMC_RS_PIN].gpio_bit, GPIO_AF_OUTPUT_PP);  //FSMC_RS_Ax
  
    #if FSMC_CS_PIN == FSMC_CS_NE1
        FSMC_NOR_PSRAM1_BASE->BCR = FSMC_BCR_WREN | FSMC_BCR_MTYP_SRAM | FSMC_BCR_MWID_16BITS | FSMC_BCR_MBKEN;
        FSMC_NOR_PSRAM1_BASE->BTR = (FSMC_DATA_SETUP_TIME << 8) | FSMC_ADDRESS_SETUP_TIME;
    #elif FSMC_CS_PIN == FSMC_CS_NE2
        FSMC_NOR_PSRAM2_BASE->BCR = FSMC_BCR_WREN | FSMC_BCR_MTYP_SRAM | FSMC_BCR_MWID_16BITS | FSMC_BCR_MBKEN;
        FSMC_NOR_PSRAM2_BASE->BTR = (FSMC_DATA_SETUP_TIME << 8) | FSMC_ADDRESS_SETUP_TIME;
    #elif FSMC_CS_PIN == FSMC_CS_NE3
        FSMC_NOR_PSRAM3_BASE->BCR = FSMC_BCR_WREN | FSMC_BCR_MTYP_SRAM | FSMC_BCR_MWID_16BITS | FSMC_BCR_MBKEN;
        FSMC_NOR_PSRAM3_BASE->BTR = (FSMC_DATA_SETUP_TIME << 8) | FSMC_ADDRESS_SETUP_TIME;
    #else
        FSMC_NOR_PSRAM4_BASE->BCR = FSMC_BCR_WREN | FSMC_BCR_MTYP_SRAM | FSMC_BCR_MWID_16BITS | FSMC_BCR_MBKEN;
        FSMC_NOR_PSRAM4_BASE->BTR = (FSMC_DATA_SETUP_TIME << 8) | FSMC_ADDRESS_SETUP_TIME;
    #endif
    afio_remap(AFIO_REMAP_FSMC_NADV);
    mLCD = (FSMC_LCD_TypeDef*)controllerAddress;
    return 1;
}

void LCD_Reset(void)
{
  setPinOutput(LCD_RESET_PIN);
  WRITE(LCD_RESET_PIN,LOW);
  _delay_ms(5);
  WRITE(LCD_RESET_PIN,HIGH);
  _delay_ms(5);
}

void LCD_BatchSyn(const void*colors,int len,bool autoInc)
{
  int snum;
  if(autoInc)dma_setup_transfer(FSMC_DMA_DEV, FSMC_DMA_CHANNEL, (volatile void*)(colors), DMA_SIZE_16BITS, &mLCD->RAM, DMA_SIZE_16BITS, DMA_MEM_2_MEM | DMA_PINC_MODE);
  else dma_setup_transfer(FSMC_DMA_DEV, FSMC_DMA_CHANNEL, (volatile void*)(colors), DMA_SIZE_16BITS, &mLCD->RAM, DMA_SIZE_16BITS, DMA_MEM_2_MEM);
  while(len > 0){
      snum = len > LCD_BatchMaxNum ? LCD_BatchMaxNum : len;
      len -= snum;
      dma_set_num_transfers(FSMC_DMA_DEV, FSMC_DMA_CHANNEL, snum);
      dma_clear_isr_bits(FSMC_DMA_DEV, FSMC_DMA_CHANNEL);
      dma_enable(FSMC_DMA_DEV, FSMC_DMA_CHANNEL);
      while((dma_get_isr_bits(FSMC_DMA_DEV, FSMC_DMA_CHANNEL) & 0x0A) == 0){}
      dma_disable(FSMC_DMA_DEV, FSMC_DMA_CHANNEL);
    }
}

void W25Q16McuInit(void)
{
  spiInit(SPI_FULL_SPEED);
  setPinOutput(SS_PIN);//CS片选脚初始化
  WRITE(SS_PIN,HIGH);
}
void myDelayUs(uint32_t n)
{
  _delay_ms(1);
	n = n + n + n;
	while(n > 0){
		n --;
		__ASM volatile ("nop");
		__ASM volatile ("nop");
		__ASM volatile ("nop");
		__ASM volatile ("nop");
		__ASM volatile ("nop");
		__ASM volatile ("nop");
		__ASM volatile ("nop");
		__ASM volatile ("nop");
		__ASM volatile ("nop");
		__ASM volatile ("nop");
		__ASM volatile ("nop");
	}
}

