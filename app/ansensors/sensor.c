#include "sensors.h"

void IO_Configure (void)
{
	PORT_InitTypeDef PORT_InitStructure;	
	PORT_StructInit(&PORT_InitStructure);
		
	PORT_InitStructure.PORT_Pin   = (PORT_Pin_0 | PORT_Pin_1 | PORT_Pin_2 | PORT_Pin_3 | PORT_Pin_4  | PORT_Pin_5  | PORT_Pin_6  | PORT_Pin_7);
  PORT_InitStructure.PORT_OE    = PORT_OE_IN;
  PORT_InitStructure.PORT_FUNC  = PORT_FUNC_PORT;
	PORT_InitStructure.PORT_PULL_UP = PORT_PULL_UP_ON;
  PORT_InitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
  PORT_InitStructure.PORT_SPEED = PORT_SPEED_SLOW;
	PORT_Init(MDR_PORTA, &PORT_InitStructure);
	
	PORT_InitStructure.PORT_Pin   = (PORT_Pin_0 | PORT_Pin_1 | PORT_Pin_2 | PORT_Pin_3 | PORT_Pin_4 | PORT_Pin_6 | PORT_Pin_7 | PORT_Pin_8 | PORT_Pin_9 | PORT_Pin_10);
  PORT_InitStructure.PORT_OE    = PORT_OE_IN;
  PORT_InitStructure.PORT_FUNC  = PORT_FUNC_PORT;
	PORT_InitStructure.PORT_PULL_UP = PORT_PULL_UP_ON;	
  PORT_InitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
  PORT_InitStructure.PORT_SPEED = PORT_SPEED_SLOW;
	PORT_Init(MDR_PORTB, &PORT_InitStructure);
	
	PORT_InitStructure.PORT_Pin   = (PORT_Pin_0 | PORT_Pin_1 | PORT_Pin_2);
  PORT_InitStructure.PORT_OE    = PORT_OE_IN;
  PORT_InitStructure.PORT_FUNC  = PORT_FUNC_PORT;
	PORT_InitStructure.PORT_PULL_UP = PORT_PULL_UP_ON;	
  PORT_InitStructure.PORT_MODE  = PORT_MODE_DIGITAL;	
  PORT_InitStructure.PORT_SPEED = PORT_SPEED_SLOW;
	PORT_Init(MDR_PORTC, &PORT_InitStructure);	
	
	PORT_InitStructure.PORT_Pin   = (PORT_Pin_7);
  PORT_InitStructure.PORT_OE    = PORT_OE_IN;
  PORT_InitStructure.PORT_FUNC  = PORT_FUNC_PORT;
	PORT_InitStructure.PORT_PULL_UP = PORT_PULL_UP_ON;	
  PORT_InitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
  PORT_InitStructure.PORT_SPEED = PORT_SPEED_SLOW;
	PORT_Init(MDR_PORTD, &PORT_InitStructure);
	
	PORT_InitStructure.PORT_Pin   = (PORT_Pin_0 | PORT_Pin_1 | PORT_Pin_2 | PORT_Pin_3 | PORT_Pin_6 | PORT_Pin_7);
  PORT_InitStructure.PORT_OE    = PORT_OE_IN;
  PORT_InitStructure.PORT_FUNC  = PORT_FUNC_PORT;
	PORT_InitStructure.PORT_PULL_UP = PORT_PULL_UP_ON;	
  PORT_InitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
  PORT_InitStructure.PORT_SPEED = PORT_SPEED_SLOW;
	PORT_Init(MDR_PORTE, &PORT_InitStructure);	
	
	PORT_InitStructure.PORT_Pin   = (PORT_Pin_0 | PORT_Pin_1 | PORT_Pin_2 | PORT_Pin_3);
  PORT_InitStructure.PORT_OE    = PORT_OE_IN;
  PORT_InitStructure.PORT_FUNC  = PORT_FUNC_PORT;
	PORT_InitStructure.PORT_PULL_UP = PORT_PULL_UP_ON;	
  PORT_InitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
  PORT_InitStructure.PORT_SPEED = PORT_SPEED_SLOW;
	PORT_Init(MDR_PORTF, &PORT_InitStructure);
}

uint32_t ReadDIInputs(void)
{
	uint32_t tmp=0;
	
	tmp = (uint8_t)PORT_ReadInputData(MDR_PORTA);
	tmp &= 0x000000FF;
	tmp |= PORT_ReadInputData(MDR_PORTB)<<8;
	tmp &= 0x00001fff;
	tmp |= ((PORT_ReadInputData(MDR_PORTB)>>6)<<13);
	tmp &= 0x0003ffff;
	tmp |= PORT_ReadInputData(MDR_PORTC)<<18;
	tmp &= 0x001fffff;
	tmp |= PORT_ReadInputDataBit(MDR_PORTD,PORT_Pin_7)<<21;
	tmp &= 0x003fffff;
	tmp |= PORT_ReadInputData(MDR_PORTE)<<22;
	tmp &= 0x03ffffff;
	tmp |= (PORT_ReadInputDataBit(MDR_PORTE,PORT_Pin_6)<<26 | PORT_ReadInputDataBit(MDR_PORTE,PORT_Pin_7)<<27);
	tmp &= 0x0fffffff;
	tmp |= PORT_ReadInputData(MDR_PORTF)<<28;
	
	return tmp;
}
