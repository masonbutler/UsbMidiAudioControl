#ifndef FADERS_H_
#define FADERS_H_

#include <sam.h>
#include "ADC.h"
#include "UART.h"
#include <stdbool.h>

enum FADER_NAMES {
	FADER_START = 0,
	FADER_CHAN1 = 0,
	FADER_CHAN2,
	FADER_CHAN3,
	FADER_CHAN4,
	FADER_END
};

typedef enum  {
	FADER_EVENT_CHANGED
} FADER_EVENT;

typedef struct {
	uint8_t id;
	uint8_t state;
	uint16_t rawValue;
	uint8_t processedValue;
	void (*callback)(uint8_t faderId, uint8_t faderEvent, uint8_t value);
} sFader;

void InitFader(sFader *fader, uint8_t id, void (*callback)(uint8_t, uint8_t, uint8_t));
void ProcessFader(sADCDevice *adcDevice, sFader *fader, sUARTDevice *uartDev);


#endif