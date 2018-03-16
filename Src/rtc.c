#include "rtc.h"

static RTC_HandleTypeDef hrtc;
static RTC_TimeTypeDef	rtcTime;
static RTC_DateTypeDef	rtcDate;

static void	RTC_GetDateTimeFromRTC(void);

void RTC_Init()
{
	hrtc.Instance = RTC;
	
	if(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0) != 0x32F2)
	{
		hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
		hrtc.Init.AsynchPrediv = 127;
		hrtc.Init.SynchPrediv = 255;
		hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
		hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
		hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
		hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
		if (HAL_RTC_Init(&hrtc) != HAL_OK)
		{
			_Error_Handler(__FILE__, __LINE__);
		}
    HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR0,0x32F2);
  }
}

static void	RTC_GetDateTimeFromRTC()
{
	HAL_RTC_GetTime(&hrtc,&rtcTime,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc,&rtcDate, RTC_FORMAT_BIN);
}

RTC_TimeTypeDef *RTC_GetRTCTime()
{
	RTC_GetDateTimeFromRTC();
	
	return &rtcTime;
}

RTC_DateTypeDef *RTC_GetRTCDate()
{
	RTC_GetDateTimeFromRTC();
	
	return &rtcDate;	
}

void	RTC_SetTime(RTC_TimeTypeDef	*rtcTime)
{
	if(rtcTime->Hours > 23)rtcTime->Hours = 0;
	if(rtcTime->Minutes > 59)rtcTime->Minutes = 0;
	if(rtcTime->Seconds > 59)rtcTime->Seconds = 0;
	
	
	HAL_RTC_SetTime(&hrtc,rtcTime,RTC_FORMAT_BIN);
}

void	RTC_SetDate(RTC_DateTypeDef	*rtcDate)
{
	if(rtcDate->Date > 31)rtcDate->Date = 1;
	if(rtcDate->Month > 12)rtcDate->Month = 1;
	if(rtcDate->Year > 99)rtcDate->Year = 0;
	
	HAL_RTC_SetDate(&hrtc,rtcDate,RTC_FORMAT_BIN);
}
