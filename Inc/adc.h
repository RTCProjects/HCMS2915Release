#ifndef _ADC_H
#define _ADC_H

#include "stm32l0xx_hal.h"

#define ADC_RESOLUTION	1024
#define ADC_REF_VOLTAGE	3.3f
#define ADC_DIVIDER			1.65f

#define ADC_ARRAY_SIZE	16

typedef struct
{
	uint16_t	adcvalue;
	float			adcvoltage;
	float			resultvoltage;
}tADCResult;

typedef struct
{
	float 	voltage;
	uint8_t	percent;
}tADCBattery;

void				ADC_Init(void);
float 			ADC_Voltage(void);
tADCBattery	ADC_Battery(void);
#endif
