#ifndef _ADG439_H
#define _ADG439_H

#include "MDR32F9Qx_port.h"

enum chs{
	CH0		= 0x0000,
	CH1 	= 0x0004,
	CH2 	= 0x0104,
	CH3 	= 0x0204,
	CH4 	= 0x0304,
	CH5 	= 0x0008,
	CH6 	= 0x0108,
	CH7 	= 0x0208,
	CH8 	= 0x0308,
	CH9		= 0x0010,
	CH10 	= 0x0110,
	CH11 	= 0x0210,
	CH12 	= 0x0310,
	CH13 	= 0x0020,
	CH14 	= 0x0120,
	CH15 	= 0x0220,
	CH16 	= 0x0320
	
};

void ADG439Init(void);
void ADG439OnCh(uint8_t ch);

#endif
