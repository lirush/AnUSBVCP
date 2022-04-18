#include "spi.h"

void SPIInit(void)
{
	SSP_InitTypeDef spi;
	
	SSP_DeInit(MDR_SSP1);
	
	MDR_RST_CLK->PER_CLOCK |= (1<<8);				//clocks SSP1
	
	SSP_BRGInit(MDR_SSP1,SSP_HCLKdiv2);
	
	spi.SSP_FRF = SSP_FRF_SPI_Motorola;
	spi.SSP_Mode = SSP_ModeMaster;
	spi.SSP_WordLength = SSP_WordLength9b;
	spi.SSP_SPO = SSP_SPO_Low;
	spi.SSP_SPH = SSP_SPH_1Edge;
	spi.SSP_HardwareFlowControl = SSP_HardwareFlowControl_SSE;
	
	spi.SSP_CPSDVSR = 2;											//1MHz
	spi.SSP_SCR =0x13;
	
	SSP_Init(MDR_SSP1,&spi);
	
	SSP_Cmd(MDR_SSP1,ENABLE);
	
	MDR_RST_CLK->SSP_CLOCK |= (1<<24);
	
	SSP_DeInit(MDR_SSP2);
	
	MDR_RST_CLK->PER_CLOCK |= (1<<20);				//clocks SSP2
	
	SSP_BRGInit(MDR_SSP2,SSP_HCLKdiv2);
	
	spi.SSP_FRF = SSP_FRF_SPI_Motorola;
	spi.SSP_Mode = SSP_ModeMaster;
	spi.SSP_WordLength = SSP_WordLength16b;
	spi.SSP_SPO = SSP_SPO_Low;
	spi.SSP_SPH = SSP_SPH_2Edge;
	spi.SSP_HardwareFlowControl = SSP_HardwareFlowControl_SSE;
	
	spi.SSP_CPSDVSR = 2;											//10MHz
	spi.SSP_SCR =1;
	
	SSP_Init(MDR_SSP2,&spi);
	
//	SSP_ITConfig(MDR_SSP2,SSP_IT_RX,ENABLE);
//	
//	NVIC_EnableIRQ(SSP2_IRQn);
	
	SSP_Cmd(MDR_SSP2,ENABLE);	
	MDR_RST_CLK->SSP_CLOCK |= (1<<25);
	
}
