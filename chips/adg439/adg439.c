#include "adg439.h"

uint16_t chanels[23]={
	CH0,
	CH1,
	CH2,
	CH3,
	CH4,
	CH5,
	CH6,
	CH7,
	CH8,
	CH9,
	CH0,
	CH0,
	CH0,
	CH0,
	CH0,
	CH0,
	CH10,
	CH11,
	CH12,
	CH13,
	CH14,
	CH15,
	CH16
};

void ADG439Init(void)
{
	PORT_InitTypeDef port;
	
  port.PORT_Pin = PORT_Pin_0 | PORT_Pin_1 | PORT_Pin_2 | PORT_Pin_3 | PORT_Pin_4 | PORT_Pin_5;
  port.PORT_OE    = PORT_OE_OUT;
  port.PORT_FUNC = PORT_FUNC_PORT;
  port.PORT_MODE = PORT_MODE_DIGITAL;
  port.PORT_SPEED = PORT_SPEED_SLOW;
  
  PORT_Init(MDR_PORTA, &port);
	
	MDR_PORTA->RXTX &= ~(1<<0) & ~(1<<1) & ~(1<<2) & ~(1<<3) & ~(1<<4) & ~(1<<5); 
	
}

void ADG439OnCh(uint8_t ch)
{
	uint16_t ena= chanels[ch];
	
	MDR_PORTA->RXTX &= ~(1<<0) & ~(1<<1) & ~(1<<2) & ~(1<<3) & ~(1<<4) & ~(1<<5);
	
	MDR_PORTA->RXTX = ((uint8_t)ena | (uint8_t)(ena>>8)) & 0x00ff;
	
}
