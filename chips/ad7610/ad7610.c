#include "ad7610.h"
#include "adg439.h"
#include "spi.h"
#include "MDR32F9Qx_eeprom.h"

enum GETSETTING{MEM=0, DEF};

static AD7610SCP_typedef AD7610_SCP;

static volatile uint16_t data_ssp2=0;
static volatile uint8_t new_data=0;

void AD7610GetConfig(uint8_t mode);

void Timer3_IRQHandler(void)
{
	if(new_data==0){
		if(MDR_TIMER3->STATUS & (1<<3))
		{
				{
					data_ssp2 = SSP_ReceiveData(MDR_SSP2);				
				}
			new_data=1;
			MDR_TIMER3->STATUS &= ~(1<<3);
		}
	}
	NVIC_ClearPendingIRQ(Timer3_IRQn);
}

void TIMER3_ETR_Setup(void)
{ 
		MDR_TIMER3->CNTRL = 0;
		MDR_TIMER3->IE = 0;
		MDR_RST_CLK->PER_CLOCK |= (1<<16);
		MDR_RST_CLK->TIM_CLOCK |= ((0 << RST_CLK_TIM_CLOCK_TIM3_BRG_Pos) 									/* делитель тактовой частоты Таймера 3 */
		| (1 << RST_CLK_TIM_CLOCK_TIM3_CLK_EN_Pos)); 																			/* разрешение тактовой частоты Таймера 3 */

		MDR_TIMER3->BRKETR_CNTRL |= (0<<4|0<<2);  																				// не делить внешнюю частоту

		MDR_TIMER3->IE = (1<<3);     																											// разрешение прерывания по спаду ETR

		MDR_TIMER3->CNTRL = (8<<TIMER_CNTRL_EVENT_SEL_Pos) | (1<<TIMER_CNTRL_CNT_EN_Pos); //ETR режим2, таймер вкл. 

		NVIC_EnableIRQ(Timer3_IRQn);
}

void ADCInit(void)
{
	PORT_InitTypeDef port;
	
	MDR_RST_CLK->PER_CLOCK |= (1<<23) | (1<<29);				//port C, D
	
	PORT_DeInit(MDR_PORTB);
	
	/*CNVRT*/
	port.PORT_Pin = PORT_Pin_0;
	port.PORT_FUNC = PORT_FUNC_PORT;
	port.PORT_MODE = PORT_MODE_DIGITAL;
	port.PORT_OE = PORT_OE_OUT;
	port.PORT_SPEED = PORT_SPEED_FAST;
	PORT_Init(MDR_PORTB,&port);
	
	/*BUSY*/
	port.PORT_Pin = PORT_Pin_4;
	port.PORT_FUNC = PORT_FUNC_OVERRID;
	port.PORT_MODE = PORT_MODE_DIGITAL;
	port.PORT_OE = PORT_OE_IN;
	port.PORT_SPEED = PORT_SPEED_FAST;
	PORT_Init(MDR_PORTB,&port);	
	
	/*SPI2*/
	PORT_DeInit(MDR_PORTC);
	
	port.PORT_Pin = PORT_Pin_0 | PORT_Pin_1 | PORT_Pin_3;
	port.PORT_OE =PORT_OE_OUT;
	port.PORT_FUNC = PORT_FUNC_OVERRID;
	PORT_Init(MDR_PORTC,&port);
	
	port.PORT_Pin = PORT_Pin_2;
	port.PORT_OE =PORT_OE_IN;
	port.PORT_FUNC = PORT_FUNC_OVERRID;
	PORT_Init(MDR_PORTC,&port);
	
	/*SPI1*/
	PORT_DeInit(MDR_PORTF);
	
	port.PORT_Pin = PORT_Pin_0 | PORT_Pin_1 | PORT_Pin_2;
	port.PORT_OE = PORT_OE_OUT;
	port.PORT_FUNC = PORT_FUNC_ALTER;
	PORT_Init(MDR_PORTF,&port);
	
	port.PORT_Pin = PORT_Pin_3;
	port.PORT_OE =PORT_OE_IN;
	port.PORT_FUNC = PORT_FUNC_ALTER;
	PORT_Init(MDR_PORTF,&port);
	
	SPIInit();
	
	MDR_PORTB->RXTX |= (1<<0);
	
	TIMER3_ETR_Setup();
	
	AD7610GetConfig(MEM);
	
	ADG439Init();	
}

/**
  * @brief  чтение данных из ацп канала
	* @param  ch: номер канала
	*           @arg от 1 до 10 (включительно)
  */
unsigned int AD7610ConvertData(unsigned char ch)
{
	uint16_t tmp = 0x0000;
	static uint8_t old_ch=0;
	
	if(old_ch!=ch){		
		ADG439OnCh(ch);
	}
	
	SSP_SendData(MDR_SSP2,0xffff);
	
	while((MDR_SSP2->SR & (1<<4)));					
	
	MDR_PORTB->RXTX &= ~(1<<0);								//start convert		
	
	MDR_PORTB->RXTX |= (1<<0);								//stop convert				
	
	if(new_data)
	{
		tmp=data_ssp2;
		new_data=0;
	}
	
	old_ch=ch;
		
	return tmp;
}

/**
  * @brief  устанавливает конфигурацию для AD7610 (регистр SCP)
	* @param  range_sel: диапазон напряжения
	*           @arg 0 - 0-5V
  *           @arg 1 - 0-10V
	*           @arg 2 - +/-5V
	*           @arg 3 - +/-10V
  */
void AD7610SetConfig(unsigned char range_sel, unsigned char pd)
{
	AD7610SCP_typedef *ptr = &AD7610_SCP;
	uint16_t tmp=0;
	
	ptr->start=1;
	ptr->range_sel=range_sel;
	ptr->pd =pd;
	ptr->ob_2cn=1;
	
	tmp = *(uint16_t *)ptr;
	
	SSP_SendData(MDR_SSP1,tmp);
	
	while(MDR_SSP1->SR & (1<<4));
}

unsigned short int AD7610ReadConfig(void)
{
	AD7610SCP_typedef *ptr = &AD7610_SCP;
	
	return *(uint16_t *)ptr;
	
}

void AD7610SaveConfig(void)
{
	AD7610SCP_typedef *ptr = &AD7610_SCP;
	
	__disable_irq();
	
	EEPROM_ErasePage(0x0801F000,EEPROM_Main_Bank_Select);
	
	EEPROM_ProgramByte(AD7610_ADDRESS_CONFIG,EEPROM_Main_Bank_Select,*(uint8_t *)ptr);
	
	__enable_irq();
}

/**
  * @brief  установка конфигурации по дефолту или из пямяти
	* @param  mode: 
	*           @arg MEM
  *           @arg DEF
	* @retval none
  */
void AD7610GetConfig(uint8_t mode)
{
	AD7610SCP_typedef *ptr = &AD7610_SCP;
	
	__disable_irq();
	uint8_t tmp= EEPROM_ReadByte(AD7610_ADDRESS_CONFIG,EEPROM_Main_Bank_Select);
	__enable_irq();

	if(mode ==DEF) return;											//если параметр - установить по дефолту
	ptr->reserv_1=RESET;ptr->reserv_2=RESET;
	if(tmp !=0xff) 															//если в не памяти пусто -> применить установки
	{
		ptr->ob_2cn = tmp>>2;
		ptr->pd = tmp>>5;
		ptr->range_sel = tmp>>6;
	}
	else 
	{
		ptr->ob_2cn=SET;
		ptr->pd=RESET;		
		ptr->range_sel=RESET;
	}
	
	AD7610SetConfig(ptr->range_sel,ptr->pd);		//приминить настройки
}
