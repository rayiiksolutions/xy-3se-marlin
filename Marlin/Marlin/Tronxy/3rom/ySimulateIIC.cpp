#include "ySimulateIIC.hpp"

#include "src/HAL/shared/Delay.h"
void si2cInit(void)
{
	#ifdef AT24CXX_WP
	OUT_WRITE(AT24CXX_WP,HIGH);
	#endif
	OUT_WRITE(AT24CXX_SCL,LOW);
	OUT_WRITE(AT24CXX_SDA,HIGH);
}
void si2cStart(void)
{
	OUT_WRITE(AT24CXX_SDA,HIGH);
	WRITE(AT24CXX_SCL,HIGH);
	DELAY_US(3);
	WRITE(AT24CXX_SDA,LOW);//START:when CLK is high,DATA change form high to low 
	DELAY_US(3);
	WRITE(AT24CXX_SCL,LOW);//钳住I2C总线，准备发送或接收数据 
}
void si2cStop(void)
{
	OUT_WRITE(AT24CXX_SDA,LOW);
	WRITE(AT24CXX_SCL,HIGH);//STOP:when CLK is high DATA change form low to high
	DELAY_US(3);
	WRITE(AT24CXX_SDA,HIGH);//发送I2C总线结束信号
	DELAY_US(3);
	WRITE(AT24CXX_SCL,LOW);
}

uint8_t si2cWaitAck(void)
{
	uint8_t ucErrTime = 0;
	WRITE(AT24CXX_SDA,HIGH);
	SET_INPUT_PULLUP(AT24CXX_SDA);
	DELAY_US(1);
	WRITE(AT24CXX_SCL,HIGH);
	DELAY_US(1); 
	while(READ(AT24CXX_SDA) == HIGH){
		ucErrTime++;
		if(ucErrTime > 250){
			si2cStop();
			return 1;
		}
	}
	WRITE(AT24CXX_SCL,LOW);//时钟输出0 	   
	return 0;  
}

void si2cSendAck(void)
{
	OUT_WRITE(AT24CXX_SDA,LOW);//ACK发送低电平
	DELAY_US(2);
	WRITE(AT24CXX_SCL,HIGH);
	DELAY_US(2);
	WRITE(AT24CXX_SCL,LOW);
}
void si2cSendNAck(void)
{
	OUT_WRITE(AT24CXX_SDA,HIGH);//NACK发送高电平
	DELAY_US(2);
	WRITE(AT24CXX_SCL,HIGH);
	DELAY_US(2);
	WRITE(AT24CXX_SCL,LOW);
}
void si2cSendByte(uint8_t data)
{                        
	uint8_t t;   
	_SET_OUTPUT(AT24CXX_SDA);
	for(t = 0; t < 8; t++){
		if((data & 0x80) >> 7)WRITE(AT24CXX_SDA,HIGH);
		else WRITE(AT24CXX_SDA,LOW);
		data <<= 1; 	  
		DELAY_US(1);   //对TEA5767这三个延时都是必须的
		WRITE(AT24CXX_SCL,HIGH);
		DELAY_US(4); 
		WRITE(AT24CXX_SCL,LOW);
		DELAY_US(1);
	} 
}
uint8_t si2cReadByte(uint8_t ack)
{
	uint8_t i,receive = 0;
	SET_INPUT_PULLUP(AT24CXX_SDA);
	for(i = 0; i < 8; i++){
		DELAY_US(3);
		WRITE(AT24CXX_SCL,HIGH);
		receive <<= 1;
		DELAY_US(1);
		if(READ(AT24CXX_SDA) == HIGH)receive++;
		WRITE(AT24CXX_SCL,LOW);
	}					 
	if (!ack)si2cSendNAck();//发送nACK
	else si2cSendAck();//发送ACK
  return receive;
}


