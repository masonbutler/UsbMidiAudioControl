#include <sam.h>
#include <stdlib.h>
#include "GPIO.h"
#include "UART.h"
#include "ADC.h"
#include "SPI.h"
#include "USB/UsbDevice.h"
#include "USB/Standard//UsbMidi.h"
#include "MidiMessages.h"
#include "Buttons.h"
#include "Faders.h"
#include "Leds.h"

#define MIDI_OUT_EP	(1)
#define NUM_BUTTONS	(8)
#define NUM_FADERS	(4)


sGPIOPin uartTXPin = {
	GPIO_PORT_A,
	22,
	GPIO_PIN_MODE_OUTPUT | GPIO_PIN_MODE_MUXED,
	GPIO_PULLUPDOWN_NONE,
	GPIO_MUX_SETTING_C
};

sGPIOPin uartRXPin = {
	GPIO_PORT_A,
	23,
	GPIO_PIN_MODE_INPUT | GPIO_PIN_MODE_MUXED,
	GPIO_PULLUPDOWN_NONE,
	GPIO_MUX_SETTING_C
};

sUARTDevice uart3 = {
	SERCOM3,
	UART_RX_PAD1,
	UART_TX_PAD_CONFIG0,
	UART_PARITY_EVEN,
	UART_STOPBITS_NUM_1,
	UART_CHARSIZE_8BITS,
	115200,
	UART_TRANSCEIVER_ENABLE_TX,
};

sGPIOPin spiBtnShiftRegClk = {
	GPIO_PORT_A,
	9,
	GPIO_PIN_MODE_OUTPUT | GPIO_PIN_MODE_MUXED,
	GPIO_PULLUPDOWN_NONE,
	GPIO_MUX_SETTING_C
};

sGPIOPin spiBtnShiftRegSS = {
	GPIO_PORT_A,
	10,
	GPIO_PIN_MODE_OUTPUT,
	GPIO_PULLUPDOWN_NONE,
	GPIO_MUX_SETTING_NONE,
};

sGPIOPin spiBtnShiftRegMISO = {
	GPIO_PORT_A,
	11,
	GPIO_PIN_MODE_INPUT | GPIO_PIN_MODE_MUXED,
	GPIO_PULLUPDOWN_NONE,
	GPIO_MUX_SETTING_C
};

sSPIDevice spiBtnShiftRegDevice = {
	SERCOM0,
	&spiBtnShiftRegSS,
	2000000U,
	SPI_OPERATING_MODE_HOST,
	SPI_DATA_ORDER_MSB,
	SPI_CLOCK_POL_IDLE_LOW,
	SPI_CLOCK_PHA_SAMPLE_LEADING,
	SPI_DATAIN_PAD3,
	SPI_DATAOUT_PAD_CONFIG0,
	SPI_DUPLEX_FULL,
};

sGPIOPin ledData = {
	GPIO_PORT_A,
	16,
	GPIO_PIN_MODE_OUTPUT | GPIO_PIN_MODE_MUXED,
	GPIO_PULLUPDOWN_NONE,
	GPIO_MUX_SETTING_C
};

sSPIDevice spiLeds = {
	SERCOM1,
	NULL,
	3000000U,
	SPI_OPERATING_MODE_HOST,
	SPI_DATA_ORDER_MSB,
	SPI_CLOCK_POL_IDLE_LOW,
	SPI_CLOCK_PHA_SAMPLE_LEADING,
	SPI_DATAIN_PAD3,
	SPI_DATAOUT_PAD_CONFIG0,
	SPI_DUPLEX_FULL,
};

sGPIOPin adcFader1 = {
	GPIO_PORT_A,
	4,
	GPIO_PIN_MODE_INPUT | GPIO_PIN_MODE_MUXED,
	GPIO_PULLUPDOWN_NONE,
	GPIO_MUX_SETTING_B
};

sGPIOPin adcFader2 = {
	GPIO_PORT_A,
	5,
	GPIO_PIN_MODE_INPUT | GPIO_PIN_MODE_MUXED,
	GPIO_PULLUPDOWN_NONE,
	GPIO_MUX_SETTING_B
};

sGPIOPin adcFader3 = {
	GPIO_PORT_A,
	6,
	GPIO_PIN_MODE_INPUT | GPIO_PIN_MODE_MUXED,
	GPIO_PULLUPDOWN_NONE,
	GPIO_MUX_SETTING_B
};

sGPIOPin adcFader4 = {
	GPIO_PORT_A,
	7,
	GPIO_PIN_MODE_INPUT | GPIO_PIN_MODE_MUXED,
	GPIO_PULLUPDOWN_NONE,
	GPIO_MUX_SETTING_B
};

sADCDevice adcDevice = {
	ADC,
	ADC_PRESCALER_DIV64,
	ADC_REF_SRC_INTVCC2,
	ADC_INPUTCTRL_MUXPOS_AIN8,
	ADC_NEGREF_SRC_GND,
	ADC_REFCOMP_ENABLED,
	ADC_RESOLUTION_12BIT,
	ADC_DIGITAL_CORR_ENABLED,
	ADC_RUN_MODE_SINGLE,
	0x80,
	ADC_SAMPLES_AVG_NUM_64
};

sGPIOPin UsbDM = {
	GPIO_PORT_A,
	24,
	GPIO_PIN_MODE_MUXED,
	GPIO_PULLUPDOWN_NONE,
	GPIO_MUX_SETTING_G
};

sGPIOPin UsbDP = {
	GPIO_PORT_A,
	25,
	GPIO_PIN_MODE_MUXED,
	GPIO_PULLUPDOWN_NONE,
	GPIO_MUX_SETTING_G
};

sUsbDevice usbDevice = {
	USB,
	USB_SPEED_FULLSPEED
};

static sButton buttons[8];
static sFader faders[4];

static void ButtonCallback(uint8_t buttonId, uint8_t buttonEvent);
static void FaderCallback(uint8_t faderId, uint8_t faderEvent, uint8_t value);

UsbMidiMessage midiData = { 0 };

void DawControllerInit()
{
	// Uart init
	GPIOPinInit(&uartTXPin);
	GPIOPinInit(&uartRXPin);
	UARTInit(&uart3, 3);
	
	// Button shift reg init
	GPIOPinInit(&spiBtnShiftRegSS);
	GPIOPinInit(&spiBtnShiftRegClk);
	GPIOPinInit(&spiBtnShiftRegMISO);
	SPIInit(&spiBtnShiftRegDevice, 0);
	
	// Leds init
	GPIOPinInit(&ledData);
	SPIInit(&spiLeds, 1);
	
	// ADC Faders init
	GPIOPinInit(&adcFader1);
	GPIOPinInit(&adcFader2);
	GPIOPinInit(&adcFader3);
	GPIOPinInit(&adcFader4);

	ADCInit(&adcDevice);

	UARTEnable(&uart3);
	SPIEnable(&spiLeds);
	SPIEnable(&spiBtnShiftRegDevice);
	ADCEnable(&adcDevice);
	
	GPIOPinInit(&UsbDM);
	GPIOPinInit(&UsbDP);
	UsbInit(&usbDevice);
	UsbEnable(&usbDevice);

	uint8_t btnNum = NUM_BUTTONS;
	for(int btn = 0; btn < NUM_BUTTONS; btn++)
	{	
		InitButton(&buttons[btn], --btnNum, &ButtonCallback);
	}
	
	for(int fader = 0; fader < NUM_FADERS; fader++)
	{
		InitFader(&faders[fader], fader, &FaderCallback);
	}
	
	InitLeds();
	UsbConnect(&usbDevice);
}

void DawControllerProcessInput()
{
	ProcessButtons(&spiBtnShiftRegDevice, buttons, NUM_BUTTONS, &uart3);
	
	for(int fader = 0; fader < NUM_FADERS; fader++)
	{
		ProcessFader(&adcDevice, &faders[fader], &uart3);
	}
}

void DawControllerDraw()
{
	// TODO: Switch to DMA
	SendLedData(&spiLeds);
}

static void ButtonCallback(uint8_t buttonId, uint8_t buttonEvent)
{
	if(buttonEvent == BUTTON_STATE_RELEASED)
	{
		uint8_t ledState = GetLedState(buttonId);
		if(ledState == LED_STATE_ON)
		{
			midiData.cableNum = 0;
			midiData.codeIndexNum = 0xB;
			midiData.data0 = MIDI_CC;
			midiData.data1 = buttonId + 14; // Button number
			midiData.data2 = 0x0; // Midi is data is 7bit, MSB is reserved

			UsbSend(&usbDevice, MIDI_OUT_EP, (uint8_t *) &midiData, sizeof(UsbMidiMessage));
			SetLedState(buttonId, LED_STATE_OFF);
		}
		else if(ledState == LED_STATE_OFF)
		{
			midiData.cableNum = 0;
			midiData.codeIndexNum = 0xB;
			midiData.data0 = MIDI_CC;
			midiData.data1 = buttonId + 14; // Button number
			midiData.data2 = 0x7F;
			
			UsbSend(&usbDevice, MIDI_OUT_EP, (uint8_t *) &midiData, sizeof(UsbMidiMessage));
			SetLedState(buttonId, LED_STATE_ON);
		}
	}
}

static void FaderCallback(uint8_t faderId, uint8_t faderEvent, uint8_t value)
{	
	if(faderEvent == FADER_EVENT_CHANGED)
	{
		switch(faderId)
		{
		case 0:
			{
				midiData.cableNum = 0;
				midiData.codeIndexNum = 0xB;
				midiData.data0 = MIDI_CC;
				midiData.data1 = faderId + 5;
				midiData.data2 = value;
				
				UsbSend(&usbDevice, MIDI_OUT_EP, (uint8_t *) &midiData, sizeof(UsbMidiMessage));
			}
			break;
		case 1:
		case 2:
		case 3:
		default:
			break;
		}
	}
}