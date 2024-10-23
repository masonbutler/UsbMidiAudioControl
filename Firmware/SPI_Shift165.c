#include <sam.h>
#include "SPI_Shift165.h"
#include "System.h"

#define SHIFT_REG165_FREQ	2000000U

/* PA08 - MOSI (unused)
 * PA09 - CLK
 * PA10 - SS
 * PA11 - MISO
 */
#define SERCOM0_SPI_CLK		9U
#define SERCOM0_SPI_SS		10U
#define SERCOM0_SPI_MISO	11U

void SPIInit165()
{
	// Enable clock gating to SERCOM0
	MCLK->APBCMASK.reg |= MCLK_APBCMASK_SERCOM0;
	
	// Set SS and CLK to outputs, MISO to input
	PORT->Group[0].DIR.reg |= (1U << SERCOM0_SPI_SS);
	PORT->Group[0].DIR.reg |= (1U << SERCOM0_SPI_CLK);
	PORT->Group[0].DIR.reg &= ~(1U << SERCOM0_SPI_MISO);
	
	// Set pin configs to enable alternate pin config
	PORT->Group[0].PINCFG[SERCOM0_SPI_CLK].reg = PORT_PINCFG_PMUXEN;
	PORT->Group[0].PINCFG[SERCOM0_SPI_MISO].reg = PORT_PINCFG_INEN | PORT_PINCFG_PMUXEN;

	// Set MUX to C for PA09
	PORT->Group[0].PMUX[SERCOM0_SPI_CLK >> 1].reg = PORT_PMUX_PMUXO(0x2U);
	// Set MUX to C for PA11
	PORT->Group[0].PMUX[SERCOM0_SPI_MISO >> 1].reg = PORT_PMUX_PMUXO(0x2U);

	
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
	SERCOM0->SPI.CTRLB.reg = SERCOM_SPI_CTRLB_RXEN;
	
	// Formula from PG 527 in ATSAML21 datasheet
	SERCOM0->SPI.BAUD.reg = (SYS_CLK / (2 * SHIFT_REG165_FREQ)) - 1;
	
	
}

void SPIEnable165()
{
	SERCOM0->SPI.CTRLA.bit.ENABLE = 1;
	while (SERCOM0->SPI.SYNCBUSY.bit.ENABLE);
}

void SPIDisable165()
{
	SERCOM0->SPI.CTRLA.bit.ENABLE = 0;
	while (SERCOM0->SPI.SYNCBUSY.bit.ENABLE);
}

void SPIShiftSetStateLoad165()
{
	PORT->Group[0].OUTCLR.reg |= (1U << SERCOM0_SPI_SS);
}

void SPIShiftSetStateRead165()
{
	PORT->Group[0].OUTSET.reg |= (1U << SERCOM0_SPI_SS);
}

uint8_t SPIReadByte165()
{
	// Data reg empty
	while (!SERCOM0->SPI.INTFLAG.bit.DRE);
	// Dummy write to trigger clock so we can shift out from shift reg
	SERCOM0->SPI.DATA.reg = 0xFF;

	// Data received
	while (!SERCOM0->SPI.INTFLAG.bit.RXC);

	// Return received data
	return SERCOM0->SPI.DATA.reg;
}