#ifndef _FLASH_H
#define _FLASH_H

#include "stm32l0xx_hal.h"

#define SETTINGS_ADDRESS	0x08007000
#define MAGIC_WORD				0xAA55AA55

typedef struct
{
	uint32_t	magicword;
	uint8_t		brightness;	
}tFlashSettings;

void	Flash_Init(void);

HAL_StatusTypeDef	Flash_ErasePage(uint32_t pageaddress);
HAL_StatusTypeDef Flash_WriteBlock(uint32_t	blockaddress,uint32_t	*blockdata,uint32_t blocksize);

tFlashSettings *Flash_Settings(void);
void	Flash_Save(void);
#endif
