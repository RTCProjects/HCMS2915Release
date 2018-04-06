#include "graphic.h"
#include "system.h"
#include "hcms2915.h"
 #include "cmsis_os.h"

static RTC_TimeTypeDef	*currentTime;
static RTC_DateTypeDef	*currentDate;

const char	*strSettingsText = "Настройки";
const char	*strBrightText	 = "Ярк:";
const char  *strTimeText		 = "УстВремя";
const char  *strDateText		 = "УстДата";
const char	*strExitText		 = "Выход";

const char	*strDays[] = {"Пн","Вт","Ср","Чт","Пт","Сб","Вс"};

uint32_t	uSettingScrollCnt;

osThreadId graphicsTaskHandle;

void Graphic_Init()
{
	uSettingScrollCnt = 0;
}

void Graphic_Process()
{
	
	//while(1)
	{

	eSystemState	currensSysState = System_GetState();
	char strOutBuf[8];
	
	HCMS_Clear();
	
	//switch(System_GetState())
	{
		if (currensSysState == SYS_TIME || currensSysState == SYS_SET_HOUR || currensSysState == SYS_SET_MIN || currensSysState == SYS_SET_SEC)
		{
			currentTime = System_GetRTCTime();
			if(currentTime->SubSeconds < 0x7F)
				sprintf(strOutBuf, "%02d:%02d:%02d", currentTime->Hours,currentTime->Minutes,currentTime->Seconds);
			else
				sprintf(strOutBuf, "%02d %02d %02d", currentTime->Hours,currentTime->Minutes,currentTime->Seconds);
			HCMS_PutStr(strOutBuf);
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
		/*
		case SYS_SET_HOUR:
		{		
				sprintf(strOutBuf, "%02d:%02d:%02d", currentTime.Hours,currentTime.Minutes,currentTime.Seconds);
				HCMS_PutStr((char*)"хуй");
		}break;
		
		case SYS_SET_MIN:
		{				
				sprintf(strOutBuf, "%02d:%02d:%02d", currentTime.Hours,currentTime.Minutes,currentTime.Seconds);
				HCMS_PutStr((char*)"суй");
		}break;
		
		case SYS_SET_SEC:
		{			
				sprintf(strOutBuf, "%02d:%02d:%02d", currentTime.Hours,currentTime.Minutes,currentTime.Seconds);
				HCMS_PutStr((char*)"туда");
		}break;*/
		
		

	}
	
	}
}
void	Graphic_ResetSettingsScroll()
{
	uSettingScrollCnt = 0;
}
