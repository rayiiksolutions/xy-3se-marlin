#ifndef Y_SIMULATE_IIC_HPP
#define	Y_SIMULATE_IIC_HPP
#if T2_STM32F103
#include "0board/cxy_chitu_v6.h"
#elif T2_STM32F446
#include "0board/tronxy_f446.h"
#endif

void si2cInit(void);
void si2cStart(void);
void si2cStop(void);
uint8_t si2cWaitAck(void);
void si2cSendAck(void);
void si2cSendNAck(void);
void si2cSendByte(uint8_t data);
uint8_t si2cReadByte(uint8_t ack);

#endif
