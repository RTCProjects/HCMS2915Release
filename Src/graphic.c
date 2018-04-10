#include "graphic.h"
#include "system.h"
#include "hcms2915.h"
 #include "cmsis_os.h"

static RTC_TimeTypeDef	*currentTime;
static RTC_DateTypeDef	*currentDate;

const char	*strSettingsText = "Íàñòðîéêè";
const char	*strBrightText	 = "ßðê:";
const char  *strTimeText		 = "ÓñòÂðåìÿ";
const char  *strDateText		 = "ÓñòÄàòà";
const char	*strExitText		 = "Âûõîä";

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
	
	//while(1)
	{

	eSystemState	currensSysState = System_GetState();
	char strOutBuf[8];
	
	HCMS_Clear();

		
		if (currensSysState == SYS_TIME || currensSysState == SYS_SET_HOUR || currensSysState == SYS_SET_MIN || currensSysState == SYS_SET_WEEK)
		{
			if(System_GetChargeState() == CHARGE_ON)
			{
				HCMS_PutStr("Charging");
			}
			else
			{
				char strDay[2];
				
				currentTime = System_GetRTCTime();
				currentDate = System_GetRTCDate();
				
				if(uChangeFlag)sprintf(strDay,"%s",strDays[currentDate->WeekDay]);
				else sprintf(strDay,"%d",currentDate->Date);
				
				if(currentTime->SubSeconds < 0x7F)
					sprintf(strOutBuf, "%02d:%02d %s", currentTime->Hours,currentTime->Minutes,strDay);
				else
					sprintf(strOutBuf, "%02d %02d %s", currentTime->Hours,currentTime->Minutes,strDay);
				HCMS_PutStr(strOutBuf);
				
			}
		}
		
		if (currensSysState == SYS_DATE || currensSysState == SYS_SET_DAY || currensSysState == SYS_SET_MONTH || currensSysState == SYS_SET_YEAR)
		{
			currentDate = System_GetRTCDate();
			
			sprintf(strOutBuf, "%02d/%02d/%02d", currentDate->Date,currentDate->Month,currentDate->Year);
			HCMS_PutStr(strOutBuf);
		}
		
		if (currensSysState == SYS_SETTINGS )
		{	
			uSettingScrollCnt++;
			
			switch(System_GetSettingsState())
			{
				
				case SELECT_DEFAULT: 	HCMS_PutStr((char*)strSettingsText); break;
				case SELECT_BRIGHT: 	
				{
					sprintf(strOutBuf,"%s%3d%%",strBrightText,/*Settigns.uPWMBrightness*/10);
					HCMS_PutStr((char*)strOutBuf);
				}break;
				case SELECT_TIME: 		HCMS_PutStr((char*)strTimeText); 		 break;
				case SELECT_DATE: 		HCMS_PutStr((char*)strDateText); 		 break;
				case SELECT_EXIT:			HCMS_PutStr((char*)strExitText);  		break;
			}

		}

	
	}
}
void	Graphic_ResetSettingsScroll()
{
	uSettingScrollCnt = 0;
}

void 	Graphic_ScrollChangeCallback()
{
	if(uChangeFlag){uChangeFlag = 0;return;}
	if(!uChangeFlag){uChangeFlag = 1;return;}
}
