#ifndef ADC_H_
#define ADC_H_

typedef enum {
	ADC_PRESCALER_DIV2 = 0,
	ADC_PRESCALER_DIV4,
	ADC_PRESCALER_DIV8,
	ADC_PRESCALER_DIV16,
	ADC_PRESCALER_DIV32,
	ADC_PRESCALER_DIV64,
	ADC_PRESCALER_DIV128,
	ADC_PRESCALER_DIV256,
} ADC_PRESCALER;

typedef enum {
	ADC_REFCOMP_DISABLED = 0,		/**< No reference compensation */
	ADC_REFCOMP_ENABLED,			/**< Reference compensation enabled */
} ADC_REFCOMP;

typedef enum {
	ADC_REF_SRC_INTREF = 0,			/**< Use internal voltage ref */	
	ADC_REF_SRC_INTVCC0,			/**< 1/1.6 VDDANA */
	ADC_REF_SRC_INTVCC1,			/**< 1/2 VDDANA */
	ADC_REF_SRC_VREFA,				/**< External ref A */
	ADC_REF_SRC_VREFB,				/**< External ref B */
	ADC_REF_SRC_INTVCC2,			/**< VDDANA */
} ADC_REF_SRC;

typedef enum {
	ADC_RESOLUTION_12BIT = 0,
	ADC_RESOLUTION_16BIT,
	ADC_RESOLUTION_10BIT,
	ADC_RESOLUTION_8BIT,
} ADC_RESOLUTION;

typedef enum {	
	ADC_DIGITAL_CORR_DISABLED = 0,	
	ADC_DIGITAL_CORR_ENABLED
} ADC_DIGITAL_CORR;

typedef enum {
	ADC_RUN_MODE_SINGLE = 0,	/**< Single run mode, samples by request only */
	ADC_RUN_MODE_FREERUN,		/**< Freerun mode, continually generates samples*/
} ADC_RUN_MODE;

typedef enum {
	ADC_SAMPLES_AVG_NUM_1 = 0,		/**< Average 1 sample */
	ADC_SAMPLES_AVG_NUM_2,			/**< Average 2 samples */
	ADC_SAMPLES_AVG_NUM_4,   
	ADC_SAMPLES_AVG_NUM_8,   
	ADC_SAMPLES_AVG_NUM_16,  
	ADC_SAMPLES_AVG_NUM_32,  
	ADC_SAMPLES_AVG_NUM_64,  
	ADC_SAMPLES_AVG_NUM_128, 
	ADC_SAMPLES_AVG_NUM_256,
	ADC_SAMPLES_AVG_NUM_512, 
	ADC_SAMPLES_AVG_NUM_1024,
} ADC_SAMPLES_AVG_NUM;

typedef enum {
	ADC_NEGREF_SRC_GND = 0x18,	/**< Use GND as negative voltage ref */
}ADC_NEGREF_SRC;

/**
* A structure of a the ADC device, its configuration settings
*/
typedef struct {
	Adc *adc;					/**< Pointer to the ADC peripheral address */
	uint8_t prescaler : 3;		/**< Prescaler value for ADC clock */	
	uint8_t analogRefSrc : 3;	/**< Analog reference source */
	uint8_t positiveChannel;	/**< ADC_INPUTCTRL_MUXPOS_AIN channel number  */
	uint8_t negativeChannel;	/**< The negative channel number (usually ADC_NEGREF_SRC_GND unless differential) */
	uint8_t refComp : 1;		/**< Enabled reference compensation for better reading */
	uint8_t resolution : 2;		/**< ADC bit resolution */
	uint8_t digitalCorr : 1;	/**< Enable digital correction */
	uint8_t adcRunMode : 1;		/**< ADC run mode settings */
	uint16_t gainCorrectionNum;	/**< Gain correction setting applied to reading */
	uint8_t samplesNum : 4;		/**< Number of samples to average to reading */
} sADCDevice;

/*!
* Initialize the ADC
* @param[in] device The ADCDevice configuration struct
*/
void ADCInit(sADCDevice *device);

/*!
* Enable the ADC
* @param[in] device The ADC configuration struct
*/
void ADCEnable(sADCDevice *device);

/*!
* Disable the ADC
* @param[in] device The ADC configuration struct
*/
void ADCDisable(sADCDevice *device);

/*!
* Set the positve ADC channel to the selected channel
* @param[in] device The ADC configuration struct
* @param[in] channelNum The ADC_INPUTCTRL_MUXPOS_AIN define for the channel num
*/
void ADCSetChannelPositive(sADCDevice *device, uint8_t channelNum);

/*!
* Initialize a GPIO pin
* @param[in] pin The pointer to the GPIOPin struct defining the pin
* @return The current reading from the ADC
*/
int32_t ADCReadCurrentChannel(sADCDevice *device);


#endif /* ADC_H_ */