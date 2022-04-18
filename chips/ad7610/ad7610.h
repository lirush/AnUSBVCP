#ifndef _AN7610_H
#define _AN7610_H

#define AD7610_ADDRESS_CONFIG	(0x0801F00C)

typedef struct{
	unsigned char reserv_2	:2;
	unsigned char ob_2cn		:1;
	unsigned char reserv_1	:2;
	unsigned char	pd				:1;
	unsigned char range_sel	:2;
	unsigned char start			:1;
	unsigned char 					:7;
}AD7610SCP_typedef;

void ADCInit(void);

unsigned int AD7610ConvertData(unsigned char ch);
void AD7610SetConfig(unsigned char range_sel, unsigned char pd);
unsigned short int AD7610ReadConfig(void);
void AD7610SaveConfig(void);

#endif
