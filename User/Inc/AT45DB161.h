#ifndef __AT45DB161_H
#define __AT45DB161_H

#include "stm32f2xx_hal.h"

void SPI_Config(void);
void SPI_Init(void);
void SPI_MainMemoryPageRead(uint16_t Page,uint16_t startaddr,uint8_t *p ,uint16_t length);
void SPI_MainMemoryPageWrite(uint16_t Page,uint16_t startaddr,uint8_t *p ,uint16_t length);
void WriteHisRec(uint16_t recordno,uint8_t *p ,uint16_t length) ;
void ReadHisRec(uint16_t recordno,uint8_t *p ,uint16_t length) ;

#endif  /*__AT45DB161_H*/

