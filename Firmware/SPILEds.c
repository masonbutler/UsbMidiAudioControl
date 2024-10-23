#include <sam.h>
#include "SPILeds.h"
#include "System.h"

#define LED_FREQ	2400000U

/* PA08 - MOSI (unused)
 * PA09 - CLK
 * PA10 - SS
 * PA11 - MISO
 */
#define SERCOM0_SPI_MOSI	8U
#define SERCOM0_SPI_CLK		9U
#define SERCOM0_SPI_SS		10U
#define SERCOM0_SPI_MISO	11U

void SPIInitLEDs()
{
	// Enable clock gating to SERCOM0
	MCLK->APBCMASK.reg |= MCLK_APBCMASK_SERCOM0;
	
	// Set SS and CLK to outputs, MISO to input
	PORT->Group[0].DIR.reg |= (1U << SERCOM0_SPI_MOSI);
	PORT->Group[0].DIR.reg |= (1U << SERCOM0_SPI_SS);
	PORT->Group[0].DIR.reg |= (1U << SERCOM0_SPI_CLK);
	PORT->Group[0].DIR.reg &= ~(1U << SERCOM0_SPI_MISO);
	
	// Set pin configs to enable alternate pin config
	PORT->Group[0].PINCFG[SERCOM0_SPI_MOSI].reg = PORT_PINCFG_PMUXEN;
	PORT->Group[0].PINCFG[SERCOM0_SPI_CLK].reg = PORT_PINCFG_PMUXEN;
	PORT->Group[0].PINCFG[SERCOM0_SPI_MISO].reg = PORT_PINCFG_INEN | PORT_PINCFG_PMUXEN | PORT_PINCFG_PULLEN;

	// Set MUX to C for PA09
	PORT->Group[0].PMUX[SERCOM0_SPI_CLK >> 1].reg = PORT_PMUX_PMUXO(0x2U);
	// Set MUX to C for PA11
	PORT->Group[0].PMUX[SERCOM0_SPI_MISO >> 1].reg = PORT_PMUX_PMUXO(0x2U);
	PORT->Group[0].PMUX[SERCOM0_SPI_MOSI >> 1].reg = PORT_PMUX_PMUXE(0x2U);


	
	// Set main clock as SERCOM0 clock and enable it
	GCLK->PCHCTRL[SERCOM0_GCLK_ID_CORE].reg |= GCLK_PCHCTRL_CHEN | GCLK_PCHCTRL_GEN_GCLK0;
	// Wait for clock to be enabled
	while(!(GCLK->PCHCTRL[SERCOM0_GCLK_ID_CORE].reg & GCLK_PCHCTRL_CHEN));
	
	
	// SS Pin to control Parallel Load
	// MISO Pin to read from Shift reg
	// CLK Pin to clock bit
	
	SERCOM0->SPI.CTRLA.bit.SWRST = 1;
	while(SERCOM0->SPI.CTRLA.reg & SERCOM_SPI_CTRLA_SWRST);
	
	// Host mode
	SERCOM0->SPI.CTRLA.reg = SERCOM_SPI_CTRLA_MODE(0x3) | SERCOM_SPI_CTRLA_DIPO(0x3) | SERCOM_SPI_CTRLA_DOPO(0x0);
	SERCOM0->SPI.CTRLB.bit.MSSEN = 0;
	SERCOM0->SPI.CTRLA.bit.CPOL = 0;
	SERCOM0->SPI.CTRLA.bit.CPHA = 0;
	
	// Formula from PG 527 in ATSAML21 datasheet
	SERCOM0->SPI.BAUD.reg = (SYS_CLK / (2 * LED_FREQ)) - 1;
	
	
}

void SPIEnableLEDs()
{
	SERCOM0->SPI.CTRLA.bit.ENABLE = 1;
	while (SERCOM0->SPI.SYNCBUSY.bit.ENABLE);
}

void SPIDisableLEDs()
{
	SERCOM0->SPI.CTRLA.bit.ENABLE = 0;
	while (SERCOM0->SPI.SYNCBUSY.bit.ENABLE);
}

void SPIWriteLEDs(RGBColour ledCol)
{
	uint32_t color = ledCol.g << 16 | ledCol.r << 8 | ledCol.b;
	uint8_t sendData[24];
	int index = 0;

	for(int i = 23; i >= 0; i--)
	{
		if(((color >> i) & 0x1) == 1)
		{
			sendData[index++] = 0b110;
		}
		else
		{
			sendData[index++] = 0b100;
		}
		//sendData[index++] = table1[((color >> i) & 0x1)];
	}

	SPIWriteBytes(sendData, 24);
}

void SPIWriteBytes(uint8_t *data, uint32_t numBytes)
{
	uint32_t bytesRemaining = numBytes;
	
	while(bytesRemaining > 0)
	{
		while (!SERCOM0->SPI.INTFLAG.bit.DRE);
		SERCOM0->SPI.DATA.reg = *data;
		
		//while (!SERCOM0->SPI.INTFLAG.bit.TXC); 
		data++;
		bytesRemaining--;
	}

}
