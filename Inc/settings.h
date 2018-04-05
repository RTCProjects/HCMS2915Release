#ifndef _SETTINGS_H
#define _SETTINGS_H

#include "stm32l0xx_hal.h"

typedef struct
{
	uint8_t	uPWMBrightness;
}tSettings;

void	Settings_Init(void);

extern tSettings Settigns;
#endif
