#include "system.h"
//#include "graphic.h"
//#include "settings.h"
#include "hcms2915.h"
#include "cmsis_os.h"

RTC_HandleTypeDef hrtc;

static RTC_TimeTypeDef	rtcTime;
static RTC_DateTypeDef	rtcDate;

static eSystemState			eCurrentSysState;
static eSettingsState		eCurrentSettingsState;

extern ADC_HandleTypeDef hadc;

uint16_t		btnPressCnt;
uint16_t		btnPressState;
uint32_t		sleepCounter;

uint8_t			uBtnPressFlag = 0;
uint8_t			uChrgStatWait = 0;

void	System_GetDateTimeFromRTC(void);

__IO static uint32_t uwADCxConvertedValue;
	__IO float fVoltage = 0;
void System_Init()
{	
	hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
	
	eCurrentSysState = SYS_TIME;
	eCurrentSettingsState = SELECT_DEFAULT;
	
	sleepCounter = 0;
	btnPressCnt = 0;
	btnPressState = 0;
	
	uwADCxConvertedValue = 0;
}

eChargeState System_GetChargeState()
{
	GPIO_PinState pinState = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8);
	
	if(pinState == GPIO_PIN_SET)return CHARGE_OFF;
	else return CHARGE_ON;
}

void System_Process()
{
	GPIO_PinState pinState = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
	
	if(pinState == GPIO_PIN_RESET)
	{
		uBtnPressFlag = 1;
	}
	
	if(pinState == GPIO_PIN_SET)//?????? ??????
	{
		if(!uBtnPressFlag)return;
		
			switch(eCurrentSysState)
			{
				
				
				case SYS_TIME:
				{
					if(btnPressCnt > HI_PRESS_TIME){
						System_SetState(SYS_SETTINGS);
						System_SetSettingsState(SELECT_BRIGHT);
						HCMS_Effect(EFFECT_OFF);
						
						btnPressCnt = 0;
						uBtnPressFlag = 0;
					}					
				}break;
				
				case SYS_DATE:
				{
					
				}break;
				
				case SYS_SETTINGS:
				{			
					switch(eCurrentSettingsState)
					{
						case SELECT_TIME:
						{
							if(btnPressCnt > HI_PRESS_TIME){
								HCMS_Effect(BLINK_R1);
								btnPressCnt = 0;
								System_SetState(SYS_SET_HOUR);	
							}		
						}break;
						
						case SELECT_DATE:
						{
							if(btnPressCnt > HI_PRESS_TIME){
								HCMS_Effect(BLINK_R1);
								btnPressCnt = 0;
								System_SetState(SYS_SET_DAY);	
							}	
						}break;
						
						case SELECT_EXIT:
						{
							if(btnPressCnt > HI_PRESS_TIME){
								btnPressCnt = 0;
								System_SetState(SYS_TIME);	
							}	
						}break;
					}
				}break;
				
				case SYS_SET_HOUR:
				{
					if(btnPressCnt > HI_PRESS_TIME){
						HCMS_Effect(BLINK_R2);
						
						System_SetState(SYS_SET_MIN);	
						btnPressCnt = 0;
					}			
				}break;
				
				case SYS_SET_MIN:
				{
					if(btnPressCnt > HI_PRESS_TIME){
						HCMS_Effect(BLINK_R3);
						
						System_SetState(SYS_SET_WEEK);	
						btnPressCnt = 0;
					}			
				}break;
				
				case SYS_SET_WEEK:
				{
					if(btnPressCnt > HI_PRESS_TIME){
						HCMS_Effect(EFFECT_OFF);
						
						System_SetState(SYS_TIME);
						btnPressCnt = 0;
					}			
				}break;
				
				case SYS_SET_DAY:
				{
					if(btnPressCnt > HI_PRESS_TIME){
						HCMS_Effect(BLINK_R2);
						
						System_SetState(SYS_SET_MONTH);
						btnPressCnt = 0;
					}			
				}break;
				
				case SYS_SET_MONTH:
				{
					if(btnPressCnt > HI_PRESS_TIME){
						HCMS_Effect(BLINK_R3);
						
						System_SetState(SYS_SET_YEAR);
						btnPressCnt = 0;
					}			
				}break;
				
				case SYS_SET_YEAR:
				{
					if(btnPressCnt > HI_PRESS_TIME){						
						System_SetState(SYS_TIME);
						btnPressCnt = 0;
					}			
				}break;
			}			
		btnPressCnt++;	

		
	}
	else
	{
		switch(eCurrentSysState)
		{
			case SYS_SETTINGS:
			{
				if(btnPressCnt > LO_PRESS_TIME & btnPressCnt < HI_PRESS_TIME){
						eCurrentSettingsState++;
						if(eCurrentSettingsState > LAST_SETTING)eCurrentSettingsState = FIRST_SETTING;
					}	
			}break;
			
			case SYS_SET_HOUR:
			{
				if(btnPressCnt > LO_PRESS_TIME & btnPressCnt < HI_PRESS_TIME){
					rtcTime.Hours++;
					if(rtcTime.Hours>23)rtcTime.Hours = 0;
					HAL_RTC_SetTime(&hrtc,&rtcTime,RTC_FORMAT_BIN);
				}
			}break;
			
			case SYS_SET_MIN:
			{
				if(btnPressCnt > LO_PRESS_TIME & btnPressCnt < HI_PRESS_TIME){
					rtcTime.Minutes++;
					rtcTime.Seconds = 0;
					if(rtcTime.Minutes>59)rtcTime.Minutes = 0;
					HAL_RTC_SetTime(&hrtc,&rtcTime,RTC_FORMAT_BIN);
				}
			}break;
			
			case SYS_SET_WEEK:
			{
				if(btnPressCnt > LO_PRESS_TIME & btnPressCnt < HI_PRESS_TIME){
					rtcDate.WeekDay++;
					if(rtcDate.WeekDay>7)rtcDate.WeekDay = 1;
					HAL_RTC_SetDate(&hrtc,&rtcDate,RTC_FORMAT_BIN);
				}
			}break;
			
			case SYS_SET_DAY:
			{
				if(btnPressCnt > LO_PRESS_TIME & btnPressCnt < HI_PRESS_TIME){
					rtcDate.Date++;
					if(rtcDate.Date>31)rtcDate.Date = 1;
					HAL_RTC_SetDate(&hrtc,&rtcDate,RTC_FORMAT_BIN);
				}
			}break;
			
			case SYS_SET_MONTH:
			{
				if(btnPressCnt > LO_PRESS_TIME & btnPressCnt < HI_PRESS_TIME){
					rtcDate.Month++;
					if(rtcDate.Month>12)rtcDate.Month = 1;
					HAL_RTC_SetDate(&hrtc,&rtcDate,RTC_FORMAT_BIN);
				}
			}break;
			
			case SYS_SET_YEAR:
			{
				if(btnPressCnt > LO_PRESS_TIME & btnPressCnt < HI_PRESS_TIME){
					rtcDate.Year++;
					if(rtcDate.Year>99)rtcDate.Year = 0;
					HAL_RTC_SetDate(&hrtc,&rtcDate,RTC_FORMAT_BIN);
				}
			}break;
		}
		
		btnPressCnt = 0;

	}
		
	System_ADCVoltage();
	
	/*sleepCounter++;
	if(sleepCounter > 1000 && sleepCounter < 1500)
		HCMS_Effect(SCROLL_R3);
	else if(sleepCounter > 1500 && sleepCounter < 3000)
		HCMS_Effect(EFFECT_OFF);
	else if(sleepCounter >= 3000)
		sleepCounter = 0;
		*/
	
	 /* sleepCounter++;
		if(sleepCounter > SLEEP_VALUE)
			System_EnterStandBy();  
*/
}

void System_ADCVoltage()
{

	
	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc, 10);
  if ((HAL_ADC_GetState(&hadc) & HAL_ADC_STATE_REG_EOC) == HAL_ADC_STATE_REG_EOC)
  {
  uwADCxConvertedValue = HAL_ADC_GetValue(&hadc);
		
		fVoltage = uwADCxConvertedValue * 0.001304347826087;
  }
}

void System_EnterStandBy()
{
	HCMS_On(1);
	osDelay(500);	
	HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);	
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
	HAL_PWR_EnterSTANDBYMode();
}

 void	System_GetDateTimeFromRTC()
{
	HAL_RTC_GetTime(&hrtc,&rtcTime,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc,&rtcDate, RTC_FORMAT_BIN);
	
	
}

RTC_TimeTypeDef *System_GetRTCTime()
{
	System_GetDateTimeFromRTC();
	
	return &rtcTime;
}

RTC_DateTypeDef *System_GetRTCDate()
{
	System_GetDateTimeFromRTC();
	
	return &rtcDate;	
}

void System_SetState(eSystemState State)
{
	eCurrentSysState = State;
	if(State == SYS_TIME)HCMS_Effect(SCROLL_R3);
}
eSystemState	System_GetState(void){return eCurrentSysState;}

void System_SetSettingsState(eSettingsState State)
{
	eCurrentSettingsState = State;
}
eSettingsState	System_GetSettingsState(void){return eCurrentSettingsState;}

