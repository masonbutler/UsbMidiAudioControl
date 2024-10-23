#include <sam.h>
#include "System.h"
#include "SysTick.h"
#include "Buttons.h"
#include "SPI.h"
#include "UART.h"
#include "Leds.h"

// Debounce settle time, number of ticks from SysTick
#define SETTLE_TIME_TICKS	(5U)

static volatile uint8_t buttonData = 0;

static void ReadButtons(sSPIDevice *spiDevice);
static void DebounceButton(sButton *button, uint8_t currentButtonValue, sUARTDevice *dev);
static void UpdateButtonState(sButton *button, sUARTDevice *uartDev);

void InitButton(sButton *btn, uint8_t id, void (*callback)(uint8_t, uint8_t))
{
	btn->id = id;
	
	btn->debouncer.startCount = 0;
	btn->debouncer.startValue = 0;
	btn->debouncer.debouncedValue = 0;
	btn->debouncer.debounced = false;
	
	btn->buttonState.prevState = BUTTON_STATE_IDLE;
	btn->callback = callback;
}

static void ReadButtons(sSPIDevice *spiDevice)
{
	SPIClearSS(spiDevice);
	// Short delay for shift reg to load the data
	for(int i = 0; i < 5; i++) { __asm__("NOP"); }
	SPISetSS(spiDevice);
	for(int i = 0; i < 5; i++) { __asm__("NOP"); }
	
	// Buttons are active low so we just flip all the bits so pressed is 1
	buttonData = ~(SPIReadByte(spiDevice));
}

void ProcessButtons(sSPIDevice *spiDevice, sButton *btns, uint8_t numBtns, sUARTDevice *uartDev)
{
	ReadButtons(spiDevice);
	for(uint32_t i = 0; i < numBtns; i++)
	{
		volatile uint8_t currentButtonValue = (buttonData >> i) & 0x1;
		DebounceButton(&btns[i], currentButtonValue, uartDev);
		UpdateButtonState(&btns[i], uartDev);
	}
}

static void DebounceButton(sButton *button, uint8_t currentButtonValue, sUARTDevice *dev)
{
	uint32_t currentTick = SysTickGetNumTicks();
	uint32_t elapsedTicks = currentTick - button->debouncer.startCount;
	
	if(elapsedTicks > SETTLE_TIME_TICKS)
	{
		if(button->debouncer.startValue != currentButtonValue)
		{
			button->debouncer.startCount = currentTick;
			button->debouncer.startValue = currentButtonValue;
			button->debouncer.debounced = false;
		}
		else
		{
			button->debouncer.debouncedValue = currentButtonValue;
			button->debouncer.debounced = true;
		}
	}
}

static void UpdateButtonState(sButton *button, sUARTDevice *uartDev)
{	
	if(!button->debouncer.debounced)
		return;
	
	switch(button->debouncer.debouncedValue)
	{
	case BUTTON_LOGIC_UP:
		switch(button->buttonState.prevState)
		{
		case BUTTON_STATE_PRESSED:
			button->callback(button->id, BUTTON_STATE_RELEASED);
			button->buttonState.prevState = BUTTON_STATE_RELEASED;
		break;
		default:
			button->buttonState.prevState = BUTTON_STATE_RELEASED;
		break;
		}
	break;
		
	case BUTTON_LOGIC_DOWN:
		switch(button->buttonState.prevState)
		{
		case BUTTON_STATE_IDLE:
		case BUTTON_STATE_PRESSED:
		case BUTTON_STATE_RELEASED:
			button->buttonState.prevState = BUTTON_STATE_PRESSED;
		break;
		}
		break;
	default:
	break;	
	}
}
