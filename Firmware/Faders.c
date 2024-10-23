#include <sam.h>
#include "System.h"
#include "SysTick.h"
#include "Faders.h"
#include "ADC.h"
#include "UART.h"
#include "Leds.h"
#include "Utility.h"
#include <stdlib.h>

#define FADER_MIN_DELTA_VALUE     5				// Min change from ADC reading before processed
#define FADER_NUM_STEPS           101				// Number of discrete steps  0 to 100 = 101
/* Number of values per step   4080 is ADC max, 32 ADC min for slide potentiometer, divide by
 * number of steps for vol percent and we get roughly ~40
 */

#define FADER_SMOOTH_COEFF		(0.1f)
// The smoothing reduces the maximum value by a small percentage, so we need this to make it reach full max
#define FADER_SMOOTH_THRESHHOLD	(10)

#define POTENTIOMETER_MAX_VAL	  (4080)
#define POTENTIOMETER_MIN_VAL	  (32)
#define FADER_VALS_PER_STEP       ((POTENTIOMETER_MAX_VAL - POTENTIOMETER_MIN_VAL) / FADER_NUM_STEPS)
#define FADER_VOLUME_SHIFT_AMOUNT	(5)

uint32_t faderAdcChannel[4] = {ADC_INPUTCTRL_MUXPOS_AIN4, 
										ADC_INPUTCTRL_MUXPOS_AIN5,
										ADC_INPUTCTRL_MUXPOS_AIN6,
										ADC_INPUTCTRL_MUXPOS_AIN7};
										
static uint16_t smoothedValue = 0;

static uint16_t ReadFader(sADCDevice *adcDevice, sFader *fader);
static void UpdateFaderVolume(sFader *fader, uint16_t currentFaderValue, sUARTDevice *dev);
static void UpdateFaderLeds(sFader *fader, sUARTDevice *uartDev);
static int16_t ConvertToVolumePercent(uint16_t adcValue);
static uint16_t SmoothFader(uint16_t newVal);


void InitFader(sFader *fader, uint8_t id, void (*callback)(uint8_t, uint8_t, uint8_t))
{
	fader->id = id;
	
	fader->rawValue = 0;

	fader->callback = callback;
}

static uint16_t ReadFader(sADCDevice *adcDevice, sFader *fader)
{
	uint32_t channel = faderAdcChannel[fader->id];
	ADCSetChannelPositive(adcDevice, channel);
	for(int i = 0; i < 10; i++) { __asm__("NOP"); }
	
	return (uint16_t) ADCReadCurrentChannel(adcDevice);
}

void ProcessFader(sADCDevice *adcDevice, sFader *fader, sUARTDevice *uartDev)
{
	// To test the first fader code since unfortunately I only have 1 linear fader
	if(fader->id == 0)
	{
		uint16_t faderValue = ReadFader(adcDevice, fader);
		faderValue = SmoothFader(faderValue);
		UpdateFaderVolume(fader, faderValue, uartDev);
		UpdateFaderLeds(fader, uartDev);
	}
}

static void UpdateFaderVolume(sFader *fader, uint16_t currentFaderValue, sUARTDevice *dev)
{
	uint8_t prevVol = fader->processedValue;
	uint8_t newVol = ConvertToVolumePercent(currentFaderValue);
	uint8_t difference = abs(newVol - prevVol);
	
	/* We don't want to update unless there was a % change in volume
	 * or a real change in the value since the ADC will bounce around a bit
	 * because of noise */
	if((difference <= FADER_MIN_DELTA_VALUE) || (newVol == prevVol))
		return;
		
	fader->rawValue = currentFaderValue;
	fader->processedValue = newVol;
	
	if((newVol - prevVol) > 0)
	{
		for(int i = prevVol; i <= newVol; i++)
		{
			fader->callback(fader->id, FADER_EVENT_CHANGED, i);
		}
	}
	else if((newVol - prevVol) < 0)
	{
		for(int i = prevVol; i >= newVol; i--)
		{
			fader->callback(fader->id, FADER_EVENT_CHANGED, i);
		}
	}	
}

// TODO: do this function better and maybe table-ize it
static void UpdateFaderLeds(sFader *fader, sUARTDevice *uartDev)
{

	uint8_t ledNum = 0;
	uint8_t offset = ((NUM_LEDS_PER_FADER) * fader->id);
	if(fader->rawValue < 75)
		ledNum = LED_NUM_FADER1_11 + offset;
	else if(fader->rawValue < 175)
		ledNum = LED_NUM_FADER1_10 + offset;
	else if(fader->rawValue < 575)
		ledNum = LED_NUM_FADER1_9 + offset;
	else if(fader->rawValue < 1000)
		ledNum = LED_NUM_FADER1_8 + offset;
	else if(fader->rawValue < 1475)
		ledNum = LED_NUM_FADER1_7 + offset;
	else if(fader->rawValue < 1850)
		ledNum = LED_NUM_FADER1_6 + offset;
	else if(fader->rawValue < 2300)
		ledNum = LED_NUM_FADER1_5 + offset;
	else if(fader->rawValue < 2700)
		ledNum = LED_NUM_FADER1_4 + offset;
	else if(fader->rawValue < 3250)
		ledNum = LED_NUM_FADER1_3 + offset;
	else if(fader->rawValue < 3725)
		ledNum = LED_NUM_FADER1_2 + offset;
	else if(fader->rawValue < 3950)
		ledNum = LED_NUM_FADER1_1 + offset;
	else if(fader->rawValue < 4000)
		ledNum = LED_NUM_FADER1_0 + offset;
	else if(fader->rawValue < 4096)
	{
		for(uint8_t i = LED_NUM_FADER1_0 + offset; i <= LED_NUM_FADER1_11 + offset; i++)
		{
			SetLedState(i, LED_STATE_OFF);
		}
		
		return;
	}
		
	for(uint8_t i = LED_NUM_FADER1_0 + offset; i <= LED_NUM_FADER1_11 + offset; i++)
	{	
		if(ledNum < i)
		{
			SetLedState(i, LED_STATE_OFF);
		}
		else
		{
			SetLedState(i, LED_STATE_ON);
		}
	}
}

/* Volume is sent as a 7-bit value because that's how midi works
 * the adc value max is a 12 bit value so we shift 5 places
 * Also we have to substract the max value (127) from the value
 * and take the absolute of it because I'm a little dumb and 
 * designed the PCB so the fader is upside down (max num at bottom)
*/
static int16_t ConvertToVolumePercent(uint16_t adcValue)
{
	return abs(((adcValue >> FADER_VOLUME_SHIFT_AMOUNT) - 127));
}

/* Smooth out the fader value change to prevent sudden jumps, this just uses a generic
 * exponential smoothing algorithm
*/
static uint16_t SmoothFader(uint16_t newVal)
{
	if(newVal - smoothedValue < 10)
	{
		smoothedValue = newVal;
	}
	else
	{
		smoothedValue = (FADER_SMOOTH_COEFF * newVal) + ((1 - FADER_SMOOTH_COEFF) * smoothedValue);
	}
	
	return smoothedValue;
}