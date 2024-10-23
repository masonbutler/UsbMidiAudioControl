#include <sam.h>
#include "Leds.h"
#include "SPI.h"

#define RESET_SIGNAL_LENGTH		50
#define NUM_BITS_PER_LED		24
#define LED_BYTESTREAM_SIZE		(NUM_BITS_PER_LED * NUM_LEDS + RESET_SIGNAL_LENGTH)

static sLedData leds[NUM_LEDS];
static uint8_t ledSpiBytestream[LED_BYTESTREAM_SIZE];

void InitLeds()
{
	for(int i = LED_NUM_START; i < LED_NUM_END; i++)
	{
		leds[i].ledState = LED_STATE_ON;
		leds[i].onColour = (sRGBColour) DEFAULT_COLOUR_ON;
		leds[i].offColour = (sRGBColour) DEFAULT_COLOUR_OFF;
	}	
	
	sRGBColour blue = {0, 0, 15};
	sRGBColour red = {15, 0, 0};
	sRGBColour orange = {15, 8, 0};
	sRGBColour yellow = {15, 15, 0};
	sRGBColour green = {0, 15, 0};
	
	sRGBColour table[12] = { blue,
							 blue,
							 green,
							 green,
							 green,
							 green,
							 green,
							 green,
							 yellow,
							 orange,
							 red,
							 red,
			
	};

	
	for(int i = 0; i < NUM_LEDS_PER_FADER; i++)
	{
		leds[LED_NUM_FADER1_0 + i].onColour = (sRGBColour) table[i];
		leds[LED_NUM_FADER1_0 + i].offColour = (sRGBColour) {0, 0, 0};
		// These are off because I only had 1 linear fader and I haven't tested these yet
		leds[LED_NUM_FADER2_0 + i].onColour = (sRGBColour) table[i];
		leds[LED_NUM_FADER2_0 + i].offColour = (sRGBColour) {0, 0, 0};
			
		leds[LED_NUM_FADER3_0 + i].onColour = (sRGBColour) table[i];
		leds[LED_NUM_FADER3_0 + i].offColour = (sRGBColour) {0, 0, 0};
			
		leds[LED_NUM_FADER4_0 + i].onColour = (sRGBColour) table[i];
		leds[LED_NUM_FADER4_0 + i].offColour = (sRGBColour) {0, 0, 0};
	}
	
	UpdateLedColourDataAll();
}

void UpdateLedColourDataAll()
{
	for(int l = 0; l < NUM_LEDS; l++)
	{
		UpdateLedColourData(l);
	}
	
	uint32_t resetIndex = NUM_LEDS * NUM_BITS_PER_LED;
	for(int i = 0; i < RESET_SIGNAL_LENGTH; i++)
	{
		ledSpiBytestream[resetIndex++] = 0;
	}
}

void UpdateLedColourData(LED_NUM ledNum)
{
	uint32_t color;
	uint32_t ledDataIndex = ledNum * NUM_BITS_PER_LED;

	
	if(leds[ledNum].ledState == LED_STATE_ON)
	{
		color = leds[ledNum].onColour.r << 16 | leds[ledNum].onColour.g << 8 | leds[ledNum].onColour.b;
	}
	else
	{
		color = leds[ledNum].offColour.r << 16 | leds[ledNum].offColour.g << 8 | leds[ledNum].offColour.b;
	}
	
	for(int bits = 23; bits >= 0; bits--)
	{
		if(((color >> bits) & 0x1) == 1)
		{
			ledSpiBytestream[ledDataIndex++] = 0b110;
		}
		else
		{
			ledSpiBytestream[ledDataIndex++] = 0b100;
		}
	}
}

void SendLedData(sSPIDevice *device)
{
	SPIWriteBuffer(device, ledSpiBytestream, LED_BYTESTREAM_SIZE);
}

void SetLed(LED_NUM ledNum, LED_STATE state, sRGBColour onCol, sRGBColour offCol)
{
	leds[ledNum].ledState = state;
	leds[ledNum].onColour = onCol;
	leds[ledNum].offColour = offCol;
	
	UpdateLedColourData(ledNum);
}

void SetLedColour1(LED_NUM ledNum, sRGBColour onCol)
{
	leds[ledNum].onColour = onCol;
	UpdateLedColourData(ledNum);
}

void SetLedColour2(LED_NUM ledNum, sRGBColour offCol)
{
	leds[ledNum].offColour = offCol;
	UpdateLedColourData(ledNum);
}

void SetLedState(LED_NUM ledNum, LED_STATE state)
{
	leds[ledNum].ledState = state;
	UpdateLedColourData(ledNum);
}

LED_STATE GetLedState(LED_NUM ledNum)
{
	return leds[ledNum].ledState;
}