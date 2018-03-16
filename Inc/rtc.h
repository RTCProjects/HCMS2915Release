#ifndef _RTC_H
#define _RTC_H

#include "stm32l0xx_hal.h"

void RTC_Init(void);
RTC_TimeTypeDef *RTC_GetRTCTime(void);
RTC_DateTypeDef *RTC_GetRTCDate(void);

void	RTC_SetTime(RTC_TimeTypeDef	*rtcTime);
void	RTC_SetDate(RTC_DateTypeDef	*rtcDate);
#endif
