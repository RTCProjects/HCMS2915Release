#include "graphic.h"
#include "system.h"
#include "hcms2915.h"
#include "cmsis_os.h"
#include "adc.h"
#include "flash.h"

static RTC_TimeTypeDef	*currentTime;
static RTC_DateTypeDef	*currentDate;
	
const char	*strSettingsText = "Íàñòðîéêè";
const char	*strBrightText	 = "Óñòßð-òü";
const char  *strTimeText		 = "ÓñòÂðåìÿ";
const char  *strDateText		 = "ÓñòÄàòó";
const char	*strExitText		 = "-=Exit=-";

const char	*strDays[] = {"","Ïí","Âò","Ñð","×ò","Ïò","Ñá","Âñ"};

uint32_t	uSettingScrollCnt;

uint8_t	uChangeFlag;

osThreadId graphicsTaskHandle;

void Graphic_Init()
{
	uSettingScrollCnt = 0;
	uChangeFlag = 0;
}

void Graphic_Process()
{
	eSystemState	currensSysState = System_GetState();
	tADCBattery		adcBattery = ADC_Battery();
	

	
	currentTime = System_GetRTCTime();
	currentDate = System_GetRTCDate();
	
	char strOutBuf[8];
	
	HCMS_Clear();

		
		if (currensSysState == SYS_TIME || currensSysState == SYS_SET_HOUR || currensSysState == SYS_SET_MIN || currensSysState == SYS_SET_WEEK)
		{
				char strDay[2];
				
				
				
				if(currensSysState == SYS_TIME){
					if(uChangeFlag)sprintf(strDay,"%s",strDays[currentDate->WeekDay]);
					else sprintf(strDay,"%02d",currentDate->Date);
				}
				else{
					sprintf(strDay,"%s",strDays[currentDate->WeekDay]);
				}
				
				if(currentTime->SubSeconds < 0x7F)
					sprintf(strOutBuf, "%02d:%02d %s", currentTime->Hours,currentTime->Minutes,strDay);
				else
					sprintf(strOutBuf, "%02d %02d %s", currentTime->Hours,currentTime->Minutes,strDay);			
				HCMS_PutStr(strOutBuf);
		}
		
		if (currensSysState == SYS_DATE || currensSysState == SYS_SET_DAY || currensSysState == SYS_SET_MONTH || currensSysState == SYS_SET_YEAR)
		{
			sprintf(strOutBuf, "%02d/%02d/%02d", currentDate->Date,currentDate->Month,currentDate->Year);
			HCMS_PutStr(strOutBuf);
		}
		
		if(currensSysState == SYS_BAT)
		{
			sprintf(strOutBuf,"Áàò:%.3d%%",adcBattery.percent);
			HCMS_PutStr(strOutBuf);
		}
		
		if(currensSysState == SYS_SET_BRIGHT)
		{
			sprintf(strOutBuf,"ßðê:%.2d%óð",Flash_Settings()->brightness);
			HCMS_PutStr(strOutBuf);
		}
		
		if (currensSysState == SYS_SETTINGS )
		{	
		
			
			switch(System_GetSettingsState())
			{
				
				case SELECT_DEFAULT: 	HCMS_PutStr((char*)strSettingsText); break;
				case SELECT_TIME: 		HCMS_PutStr((char*)strTimeText); 		 break;
				case SELECT_DATE: 		HCMS_PutStr((char*)strDateText); 		 break;
				case SELECT_BAT:
				{
					sprintf(strOutBuf,"%.1fv%.3d%%",adcBattery.voltage,adcBattery.percent);
					HCMS_PutStr(strOutBuf);
				}break;
				case SELECT_BRIGHT:HCMS_PutStr((char*)strBrightText);break;
				case SELECT_EXIT:			HCMS_PutStr((char*)strExitText);  		break;
			}

		}
}
void	Graphic_ResetSettingsScroll()
{
	
}

void 	Graphic_ScrollChangeCallback()
{
	if(uChangeFlag){uChangeFlag = 0;return;}
	if(!uChangeFlag){uChangeFlag = 1;return;}
}
