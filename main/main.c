/*
*** ����������� ����������� �� ��� ��� 02.2022 �.
***
*** ��������� ��������� virtual com port
*** ��������� ���������� �����. 16 �������. ADC ��� AD7610
*** ���������� ������ usb �� vcp 
***
*** ����������� ��� ���� �� ���� 1986��92�
*** ����� =8��� PLL =10 USBCLK =48���
***
***icpanibul: 
***					icp - internal communication protocol
***					ani - analog input
***					up  - upper level
*/
#include "main.h"
#include "ringqueue.h"
#include "ad7610.h"
#include "adg439.h"

RingQueue_typedef QueUSB;
static uint8_t Buffer[BUFFER_LENGTH];
uint8_t RingBufferUSB[QUE_USB_NUMBER];

int main(void)
{	
	VCom_Configuration();

  USB_CDC_Init(Buffer, 1, SET);
	
	Setup_USB();
	
	RingQueInit(&QueUSB,RingBufferUSB,QUE_USB_NUMBER);
	
	ADCInit();
	
	while(1)
	{
		USB_DeviceDispatchEvent();
		
		Icpanibul();
	}
}
