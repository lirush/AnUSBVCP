#include "systick.h"

volatile uint32_t TimingDelay_ms;
uint32_t Ticks=0;

void SysTick_Handler(void)
{
  if (TimingDelay_ms > 0x00)
  {
    TimingDelay_ms--;
  }
	Ticks++;
	NVIC_ClearPendingIRQ(SysTick_IRQn);
}
