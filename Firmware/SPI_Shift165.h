#ifndef SPI_SHIFT165_H_
#define SPI_SHIFT165_H_
#include <stdint.h>

void SPIInit165();
void SPIEnable165();
void SPIDisable165();

void SPIShiftSetStateLoad165();
void SPIShiftSetStateRead165();

uint8_t SPIReadByte165();



#endif