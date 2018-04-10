#ifndef _GRAPHIC_H
#define _GRPAHIC_H

#include "stm32l0xx_hal.h"


void 	Graphic_Init(void);
void 	Graphic_Process(void);
void	Graphic_ResetSettingsScroll(void);
void 	Graphic_ScrollChangeCallback(void);

#endif
