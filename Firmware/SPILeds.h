#ifndef SPILEDS_H_
#define SPILEDS_H_
#include "RGBLeds.h"

void SPIInitLEDs();
void SPIEnableLEDs();
void SPIDisableLEDs();

void SPIWriteLEDs(RGBColour col);
void SPIWriteBytes(uint8_t *data, uint32_t numBytes);


#endif /* SPILEDS_H_ */