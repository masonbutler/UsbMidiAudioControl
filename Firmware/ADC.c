#include <sam.h>
#include "ADC.h"
#include "GPIO.h"
#include "NVM_OTP.h"

static void ConfigureClocks();
static void ConfigureCalibration(sADCDevice *device);
static void ConfigureReference(sADCDevice *device);
static void ConfigureSampleCorrections(sADCDevice *device);

void ADCInit(sADCDevice *device)
{
	Adc *module = device->adc;
	ConfigureClocks();
	module->CTRLA.reg = ADC_CTRLA_SWRST;
	while (module->CTRLA.reg & ADC_CTRLA_SWRST);
	
	ConfigureCalibration(device);
	
	module->CTRLB.reg |= (device->prescaler << ADC_CTRLB_PRESCALER_Pos) & ADC_CTRLB_PRESCALER_Msk;

	ConfigureReference(device);

	module->CTRLC.reg |= ADC_CTRLC_RESSEL(device->resolution);
	while(module->SYNCBUSY.bit.CTRLC);
	
	module->INTFLAG.reg |= ADC_INTFLAG_RESRDY;
	
	ConfigureSampleCorrections(device);
	
	module->CTRLC.reg |= (device->adcRunMode << ADC_CTRLC_FREERUN_Pos) & ADC_CTRLC_FREERUN;
}

void ADCSetChannelPositive(sADCDevice *device, uint8_t channelNum)
{
	Adc *module = device->adc;
	device->positiveChannel = channelNum;
	
	module->INPUTCTRL.bit.MUXPOS = channelNum;
	while(module->SYNCBUSY.bit.INPUTCTRL);
}

void ADCEnable(sADCDevice *device)
{
	Adc *module = device->adc;
	module->CTRLA.bit.ENABLE = 1;
	while (module->SYNCBUSY.bit.ENABLE);
}

void ADCDisable(sADCDevice *device)
{
	Adc *module = device->adc;
	module->CTRLA.bit.ENABLE = 0;
	while (module->SYNCBUSY.bit.ENABLE);
}

int32_t ADCReadCurrentChannel(sADCDevice *device)
{
	Adc *module = device->adc;
	
	module->SWTRIG.reg = ADC_SWTRIG_START;
	
	while (!module->INTFLAG.reg & ADC_INTFLAG_RESRDY);
	module->INTFLAG.reg |= ADC_INTFLAG_RESRDY;

	return module->RESULT.reg;
}

static void ConfigureClocks()
{
	MCLK->APBDMASK.reg |= MCLK_APBDMASK_ADC;
	GCLK->PCHCTRL[ADC_GCLK_ID].reg = (GCLK_PCHCTRL_GEN(0) | GCLK_PCHCTRL_CHEN);
	while(!(GCLK->PCHCTRL[ADC_GCLK_ID].reg & GCLK_PCHCTRL_CHEN));
}

static void ConfigureCalibration(sADCDevice *device)
{
	Adc *module = device->adc;
	
	uint32_t biasRefBuf = *((uint32_t *)(NVMCTRL_OTP5)) >> NVM_OTP_BIASREFBUF_POS;
	uint32_t biasComp = *((uint32_t *)(NVMCTRL_OTP5)) >> NVM_OTP_BIASCOMP_POS;
	
	module->CALIB.reg = ADC_CALIB_BIASREFBUF(biasRefBuf) | ADC_CALIB_BIASCOMP(biasComp);
}

static void ConfigureReference(sADCDevice *device)
{
	Adc *module = device->adc;
	
	module->REFCTRL.reg |= (device->refComp << ADC_REFCTRL_REFCOMP_Pos) & ADC_REFCTRL_REFCOMP;
	module->REFCTRL.reg |= (device->analogRefSrc << ADC_REFCTRL_REFSEL_Pos) & ADC_REFCTRL_REFSEL_Msk;
	
	module->INPUTCTRL.reg |= (device->positiveChannel << ADC_INPUTCTRL_MUXPOS_Pos) & ADC_INPUTCTRL_MUXPOS_Msk;
	while(module->SYNCBUSY.bit.INPUTCTRL);
	
	module->INPUTCTRL.reg |= (device->negativeChannel << ADC_INPUTCTRL_MUXNEG_Pos) & ADC_INPUTCTRL_MUXNEG_Msk;
	while(module->SYNCBUSY.bit.INPUTCTRL);
}

static void ConfigureSampleCorrections(sADCDevice *device)
{
	Adc *module = device->adc;
	
	module->AVGCTRL.reg = (device->samplesNum << ADC_AVGCTRL_SAMPLENUM_Pos) & ADC_AVGCTRL_SAMPLENUM_Msk;
	
	module->CTRLC.reg |= (device->digitalCorr << ADC_CTRLC_CORREN_Pos) & ADC_CTRLC_CORREN;
	while(module->SYNCBUSY.bit.CTRLC);
	
	module->GAINCORR.reg = ADC_GAINCORR_GAINCORR(device->gainCorrectionNum);
	while(module->SYNCBUSY.bit.GAINCORR);
}