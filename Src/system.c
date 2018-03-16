#include "system.h"
//#include "graphic.h"
//#include "settings.h"
#include "hcms2915.h"
#include "cmsis_os.h"
#include "rtc.h"


static eSystemState			eCurrentSysState;
static eSettingsState		eCurrentSettingsState;


uint8_t		btnPressCnt;
uint8_t		btnPressState;
uint32_t	sleepCounter;

void	System_GetDateTimeFromRTC(void);

osThreadId systemTaskHandle;

void System_Init()
{	
	eCurrentSysState = SYS_TIME;
	eCurrentSettingsState = SELECT_DEFAULT;
	
	sleepCounter = 0;
	btnPressCnt = 0;
	btnPressState = 0;
	
	osThreadDef(systemTask, System_Process, osPriorityNormal, 0, configMINIMAL_STACK_SIZE + 0x100);
  systemTaskHandle = osThreadCreate(osThread(systemTask), NULL);
}

void System_Process(void const * argument)
{
	GPIO_PinState pinState = HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13);
	
	if(pinState == GPIO_PIN_RESET)//нажата кнопка
	{

			switch(eCurrentSysState)
			{
				case SYS_TIME:
				{
					if(btnPressCnt > HI_PRESS_TIME){
						System_SetState(SYS_SETTINGS);
						System_SetSettingsState(SELECT_BRIGHT);
						btnPressCnt = 0;
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
						
						System_SetState(SYS_SET_SEC);	
						btnPressCnt = 0;
					}			
				}break;
				
				case SYS_SET_SEC:
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
						HCMS_Effect(EFFECT_OFF);
						
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
					RTC_TimeTypeDef	*rtcTime = RTC_GetRTCTime();
					rtcTime->Hours++;
					RTC_SetTime(rtcTime);
				}
			}break;
			
			case SYS_SET_MIN:
			{
				if(btnPressCnt > LO_PRESS_TIME & btnPressCnt < HI_PRESS_TIME){
					RTC_TimeTypeDef	*rtcTime = RTC_GetRTCTime();
					rtcTime->Minutes++;
					RTC_SetTime(rtcTime);
				}
			}break;
			
			case SYS_SET_SEC:
			{
				if(btnPressCnt > LO_PRESS_TIME & btnPressCnt < HI_PRESS_TIME){
					RTC_TimeTypeDef	*rtcTime = RTC_GetRTCTime();
					rtcTime->Seconds++;
					RTC_SetTime(rtcTime);
				}
			}break;
			
			case SYS_SET_DAY:
			{
				if(btnPressCnt > LO_PRESS_TIME & btnPressCnt < HI_PRESS_TIME){
					RTC_DateTypeDef	*rtcDate = RTC_GetRTCDate();
					rtcDate->Date++;
					RTC_SetDate(rtcDate);
				}
			}break;
			
			case SYS_SET_MONTH:
			{
				if(btnPressCnt > LO_PRESS_TIME & btnPressCnt < HI_PRESS_TIME){
					RTC_DateTypeDef	*rtcDate = RTC_GetRTCDate();
					rtcDate->Month++;
					RTC_SetDate(rtcDate);
				}
			}break;
			
			case SYS_SET_YEAR:
			{
				if(btnPressCnt > LO_PRESS_TIME & btnPressCnt < HI_PRESS_TIME){
					RTC_DateTypeDef	*rtcDate = RTC_GetRTCDate();
					rtcDate->Year++;
					RTC_SetDate(rtcDate);
				}
			}break;
		}
		
		btnPressCnt = 0;
		btnPressState = 0;
	}
		

	
	  //sleepCounter++;
		//if(sleepCounter > SLEEP_VALUE)
			//HAL_PWR_EnterSTANDBYMode();
			//HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
}



void System_SetState(eSystemState State)
{
	eCurrentSysState = State;
}
eSystemState	System_GetState(void){return eCurrentSysState;}

void System_SetSettingsState(eSettingsState State)
{
	eCurrentSettingsState = State;
}
eSettingsState	System_GetSettingsState(void){return eCurrentSettingsState;}
