#include "icpanibul.h"
#include "ringqueue.h"
#include <stdio.h>
#include <string.h>
#include "ad7610.h"
#include "utility.h"

static uint8_t bytes =0;
static uint8_t tmp[100];

static uint8_t GetVaSN(uint8_t p, char *vsn);
static uint8_t ReadConfig(char *conf);
static uint8_t SetConfig(char *conf);
static uint8_t SaveConfig(char *conf);

uint16_t cnt=0;
uint16_t tmp1=0;

void Icpanibul(void)
{
	uint8_t loctmp=0;
	uint8_t ch[2];
	
	if(QueUSB.avail==0)return;
	RingQueGet(&QueUSB,&loctmp,1);
	
	switch(loctmp)
	{
		case 'V':
			RingQueGet(&QueUSB,&loctmp,1);
			bytes=GetVaSN(loctmp,(char *)&tmp);
		break;
		case 'C':
			RingQueGet(&QueUSB,&loctmp,1);
			switch(loctmp)
			{
				case 'R':
					bytes=ReadConfig((char *)&tmp);
				break;
				case 'S':
					bytes=SaveConfig((char *)&tmp);
				break;
				case 't':
					bytes=SetConfig((char *)&tmp);
				break;
				default:
					bytes=0;
				break;
			}		
		break;
		case 'R':
			RingQueGet(&QueUSB,ch,2);
			loctmp=(uiAscii2byte(ch[0])<<4) | uiAscii2byte(ch[1]);
			tmp1=AD7610ConvertData(loctmp);
			tmp[0]=tmp1;		
			tmp[1]=tmp1>>8;
			tmp[2]='\r';
			bytes=3;
		cnt++;
		break;
		default:
			bytes = sprintf((char *)&tmp,"\r");
			bytes=0;
		break;				
	}
	USB_CDC_SendData(tmp,bytes);
}

static uint8_t GetVaSN(uint8_t p, char *vsn)
{
	if(p=='S') return sprintf(vsn,"1123456789ABCDEE\r");
	if(p=='N') return sprintf(vsn,"AnIn\r");
	return sprintf(vsn,"VF_0.1\r");
}

/**
  * @brief  Чтение установленной конфигурации микросхемы аналоговых входов
  * @param  conf: pointer на пользовательские данные, содержащие данные для отправки
	* @retval bnum: количество байт для отправки 
  */
static uint8_t ReadConfig(char *conf)
{
		uint8_t bnum=0;
		conf[bnum++]= AD7610ReadConfig();			//uiNibble2ascii(AD7610ReadConfig());
		conf[bnum++]='\r';
		return bnum;	
}

/**
  * @brief  Установка конфигурации микросхемы аналоговых входов
  * @param  conf: pointer на пользовательские данные, содержащие данные для отправки
	* @retval bnum: количество байт для отправки 
  */
static uint8_t SetConfig(char *conf)
{
		uint8_t bnum=0;
	
		//set conf
		RingQueGet(&QueUSB,tmp+1,1);
		uint8_t range=tmp[1]>>6; uint8_t pd=(tmp[1]>>5) & 0x01;
		AD7610SetConfig(range,pd);
	
		conf[bnum++]='\r';
		return bnum;	
}

/**
  * @brief  Сохрание ПЗУ конфигурации микросхемы аналоговых входов
  * @param  conf: pointer на пользовательские данные, содержащие данные для отправки
	* @retval bnum: количество байт для отправки 
  */
static uint8_t SaveConfig(char *conf)
{
		uint8_t bnum=0;
	
		//save conf to eeprom
		AD7610SaveConfig();	
	
		conf[bnum++]='\r';
		return bnum;
}
