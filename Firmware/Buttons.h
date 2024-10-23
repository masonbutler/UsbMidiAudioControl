#ifndef BUTTONS_H_
#define BUTTONS_H_
#include <sam.h>
#include "SPI.h"
#include "UART.h"
#include <stdbool.h>

enum BUTTON_NAMES {
	BUTTON_START = 0,
	BUTTON_CHAN1_MUTE = 0,
	BUTTON_CHAN1_SOLO,
	BUTTON_CHAN2_MUTE,
	BUTTON_CHAN2_SOLO,
	BUTTON_CHAN3_MUTE,
	BUTTON_CHAN3_SOLO,
	BUTTON_CHAN4_MUTE,
	BUTTON_CHAN4_SOLO,
	BUTTON_END
};

typedef enum  {
	BUTTON_LOGIC_UP,
	BUTTON_LOGIC_DOWN, 
} BUTTON_LOGIC_STATE;

typedef enum  {
	BUTTON_STATE_IDLE,
	BUTTON_STATE_PRESSED,
	BUTTON_STATE_RELEASED,
} BUTTON_STATE;

/* sButtonDebouncer
 * uint8_t startValue -> Value read from SPI Shift reg
 * uint8_t debouncedValue -> Value after debouncing
 * uin32_t startCount -> Count from SysTick on button action
 * bool debounced -> whether it is debounced
 */
typedef struct {
	uint8_t startValue;
	uint8_t debouncedValue;
	uint32_t startCount;
	bool debounced;
} sButtonDebouncer;

// The debounced value is our current state, so we only store previous state
typedef struct {
	uint8_t prevState;
} sButtonState;


typedef struct {
	uint8_t id;
	sButtonDebouncer debouncer;
	sButtonState buttonState;
	void (*callback)(uint8_t buttonId, uint8_t buttonEvent);
} sButton;

void InitButton(sButton *btn, uint8_t id, void (*callback)(uint8_t, uint8_t));
void ProcessButtons(sSPIDevice *spiDevice, sButton *btns, uint8_t numBtns, sUARTDevice *uartDev);

#endif