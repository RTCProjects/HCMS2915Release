#include "flash.h"
#include <string.h>

static FLASH_EraseInitTypeDef EraseInitStruct;
static tFlashSettings flashSettigns;

void	Flash_Init()
{
	memcpy(&flashSettigns,(uint32_t*)SETTINGS_ADDRESS,sizeof(tFlashSettings));
	
	if(flashSettigns.magicword != MAGIC_WORD){
		flashSettigns.brightness = 12;
		flashSettigns.magicword = MAGIC_WORD;
		Flash_Save();
	}
}

void	Flash_Save()
{
	Flash_ErasePage(SETTINGS_ADDRESS);
	Flash_WriteBlock(SETTINGS_ADDRESS,(uint32_t*)&flashSettigns,sizeof(tFlashSettings));
}

HAL_StatusTypeDef	Flash_ErasePage(uint32_t pageaddress)
{
	HAL_StatusTypeDef status;
	
	uint32_t	sectorerror;
	
	HAL_FLASH_Unlock();
			
			EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
			EraseInitStruct.PageAddress = pageaddress;
			EraseInitStruct.NbPages = 1;
			
			status = HAL_FLASHEx_Erase(&EraseInitStruct, &sectorerror);

	HAL_FLASH_Lock();
	
	return status;
}

HAL_StatusTypeDef Flash_WriteBlock(uint32_t	blockaddress,uint32_t	*blockdata,uint32_t blocksize)
{
	HAL_StatusTypeDef status;
	
	uint32_t	startaddress = blockaddress;
	
	HAL_FLASH_Unlock();
		
	while(blockaddress < startaddress + blocksize)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, blockaddress,*(blockdata++));
		
		blockaddress += 4;
	}
	
	HAL_FLASH_Lock();	
	
	return status;
}
tFlashSettings *Flash_Settings()
{
	return &flashSettigns;
}
