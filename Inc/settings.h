#ifndef _SETTINGS_H
#define _SETTINGS_H

#include "stm32f4xx_hal.h"

typedef struct
{
	uint8_t	uPWMBrightness;
}tSettings;

void	Settings_Init(void);

extern tSettings Settigns;
#endif
