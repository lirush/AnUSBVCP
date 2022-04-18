#ifndef _SPI_H
#define _SPI_H

#include "MDR32F9Qx_ssp.h"
#include "MDR32F9Qx_port.h"

void SPIInit(void);

void OpenSSP(uint8_t ch);

void CloseSSP(uint8_t ch);

#endif
