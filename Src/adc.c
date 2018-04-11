#include "adc.h"
#include <float.h>
#include <string.h>

static ADC_HandleTypeDef	hadc;
static tADCResult					adcResult;		
static tADCBattery				adcBattery;
static float 							adcMinVoltage;

static float							adcResultArray[ADC_ARRAY_SIZE];
static uint8_t						adcArrayWriteInd;



void	ADC_Init()
{
	ADC_ChannelConfTypeDef sConfig;

  hadc.Instance = ADC1;
  hadc.Init.OversamplingMode = DISABLE;
  hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc.Init.Resolution = ADC_RESOLUTION_10B;
  hadc.Init.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerFrequencyMode = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
	
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
	
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
	
	if (HAL_ADC_Start(&hadc) != HAL_OK)
  {
    Error_Handler();
  }
	
	adcMinVoltage = 5.0f;
	adcArrayWriteInd = 0;
	memset((uint8_t*)adcResultArray,0,sizeof(float) * ADC_ARRAY_SIZE);
	
}

float ADC_Voltage()
{	
	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc, 10);
	
  if ((HAL_ADC_GetState(&hadc) & HAL_ADC_STATE_REG_EOC) == HAL_ADC_STATE_REG_EOC)
  {
		adcResult.adcvalue = HAL_ADC_GetValue(&hadc);
		adcResult.adcvoltage = adcResult.adcvalue * (ADC_REF_VOLTAGE / ADC_RESOLUTION);
		adcResult.resultvoltage = adcResult.adcvoltage * ADC_DIVIDER;
			
		adcResultArray[(adcArrayWriteInd++)%ADC_ARRAY_SIZE] = adcResult.resultvoltage;
		
		if(adcArrayWriteInd == ADC_ARRAY_SIZE - 1)
		{
				float adcAverage = 0;
				for(int i = 0;i<ADC_ARRAY_SIZE;i++){
					adcAverage+=adcResultArray[i];
				}
				adcAverage = adcAverage / ADC_ARRAY_SIZE;
				
				//if(adcAverage < adcMinVoltage)adcMinVoltage = adcAverage;
				adcMinVoltage = adcAverage;
		}
		/*
		*/
		return adcMinVoltage;
  }
	else
		return 0;
}

tADCBattery	ADC_Battery()
{
	float fVoltage = ADC_Voltage();
	
	adcBattery.voltage = fVoltage;
	
	if(fVoltage < 2.8f)adcBattery.percent = 0;
	else if(fVoltage >= 2.8f && fVoltage < 2.92f)	adcBattery.percent = 10;
	else if(fVoltage >= 2.92f && fVoltage < 3.04f)adcBattery.percent = 20;
	else if(fVoltage >= 3.04f && fVoltage < 3.16f)adcBattery.percent = 30;
	else if(fVoltage >= 3.16f && fVoltage < 3.28f)adcBattery.percent = 40;
	else if(fVoltage >= 3.28f && fVoltage < 3.4f)	adcBattery.percent = 50;
	else if(fVoltage >= 3.4f && fVoltage < 3.52f)	adcBattery.percent = 60;
	else if(fVoltage >= 3.52f && fVoltage < 3.64f)adcBattery.percent = 70;
	else if(fVoltage >= 3.64f && fVoltage < 3.76f)adcBattery.percent = 80;
	else if(fVoltage >= 3.76f && fVoltage < 3.88f)adcBattery.percent = 90;
	else adcBattery.percent = 100;
	
	return adcBattery;
}
