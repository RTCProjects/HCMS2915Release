#include "flash.h"

FLASH_EraseInitTypeDef EraseInitStruct;

uint32_t SectorError = 0;

uint8_t 	flash_read_block(uint32_t		address,uint8_t *pData,uint16_t size)
{
	uint32_t 	chk_sum = *(uint32_t*)address;
	uint32_t	flash_chksum = 0;
	
	for(int i = 4;i<size + 4;i++)
		flash_chksum += *(uint8_t*)(address + i);
	
	if(chk_sum == flash_chksum)
	{
		for(int i = 4;i<size + 4;i++)
			*(pData + (i - 4)) = *(uint8_t*)(address + i);
	}
	else
		return 0;
	
	return 1;
}

uint8_t		flash_write_block(uint32_t	address,uint8_t *pData,uint16_t	size)
{
	
	uint32_t	addr_counter = 0;
	uint32_t 	chk_sum = 0;
	
	for(int i = 0;i<size;i++)
		chk_sum += (*(pData + i));
	
	
	if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, chk_sum) == HAL_OK)
	{
		addr_counter = addr_counter + 4;
		
			while(addr_counter < (size + 4))
			{
				if( HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE,address + addr_counter , *(pData + (addr_counter - 4))) == HAL_OK)
					addr_counter++;
				else
					return 0;
			}
	}
	else
		return 0;

	return 1;
	
	/*
	while(address < settings_size)
	{
		if( flash_program(ADDR_FLASH_SECTOR_14 + address,*(pData + address)) == HAL_OK)
		{
			address++;
		}
	}
	*/
}

HAL_StatusTypeDef	flash_erase(uint32_t sector)
{
	HAL_StatusTypeDef status;
	
		HAL_FLASH_Unlock();
			
			EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;

			EraseInitStruct.Sector = sector;
			EraseInitStruct.NbSectors = 1;
			
			status = HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError);
	
		//__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
	//__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);

		//FLASH_Erase_Sector(sector,VOLTAGE_RANGE_3);
	
	  __HAL_FLASH_DATA_CACHE_DISABLE();
  __HAL_FLASH_INSTRUCTION_CACHE_DISABLE();

  __HAL_FLASH_DATA_CACHE_RESET();
  __HAL_FLASH_INSTRUCTION_CACHE_RESET();

  __HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
  __HAL_FLASH_DATA_CACHE_ENABLE();
	
		//HAL_FLASH_Lock();
	
	return status;
}

HAL_StatusTypeDef flash_program(uint32_t address, uint8_t data)
{
	//HAL_FLASH_Unlock();
	
		HAL_StatusTypeDef status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, address, data);
	
	//HAL_FLASH_Lock();
	
	return status;
}

HAL_StatusTypeDef flash_program_hword(uint32_t address, uint16_t data)
{
//	HAL_FLASH_Unlock();
	
		HAL_StatusTypeDef status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, address, data);
	
//	HAL_FLASH_Lock();
	
	return status;
}



uint32_t flash_get_sector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_SECTOR_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_SECTOR_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_SECTOR_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_SECTOR_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_SECTOR_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_SECTOR_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_SECTOR_6;  
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_SECTOR_7;  
  }
  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  {
    sector = FLASH_SECTOR_8;  
  }
  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  {
    sector = FLASH_SECTOR_9;  
  }
  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  {
    sector = FLASH_SECTOR_10;  
  }
  else if((Address < ADDR_FLASH_SECTOR_12) && (Address >= ADDR_FLASH_SECTOR_11))
  {
    sector = FLASH_SECTOR_11;  
  }
  else if((Address < ADDR_FLASH_SECTOR_13) && (Address >= ADDR_FLASH_SECTOR_12))
  {
    sector = FLASH_SECTOR_12;  
  }
  else if((Address < ADDR_FLASH_SECTOR_14) && (Address >= ADDR_FLASH_SECTOR_13))
  {
    sector = FLASH_SECTOR_13;  
  }
  else if((Address < ADDR_FLASH_SECTOR_15) && (Address >= ADDR_FLASH_SECTOR_14))
  {
    sector = FLASH_SECTOR_14;  
  }
  else if((Address < ADDR_FLASH_SECTOR_16) && (Address >= ADDR_FLASH_SECTOR_15))
  {
    sector = FLASH_SECTOR_15;  
  }
  else if((Address < ADDR_FLASH_SECTOR_17) && (Address >= ADDR_FLASH_SECTOR_16))
  {
    sector = FLASH_SECTOR_16;  
  }
  else if((Address < ADDR_FLASH_SECTOR_18) && (Address >= ADDR_FLASH_SECTOR_17))
  {
    sector = FLASH_SECTOR_17;  
  }
  else if((Address < ADDR_FLASH_SECTOR_19) && (Address >= ADDR_FLASH_SECTOR_18))
  {
    sector = FLASH_SECTOR_18;  
  }
  else if((Address < ADDR_FLASH_SECTOR_20) && (Address >= ADDR_FLASH_SECTOR_19))
  {
    sector = FLASH_SECTOR_19;  
  }
  else if((Address < ADDR_FLASH_SECTOR_21) && (Address >= ADDR_FLASH_SECTOR_20))
  {
    sector = FLASH_SECTOR_20;  
  } 
  else if((Address < ADDR_FLASH_SECTOR_22) && (Address >= ADDR_FLASH_SECTOR_21))
  {
    sector = FLASH_SECTOR_21;  
  }
  else if((Address < ADDR_FLASH_SECTOR_23) && (Address >= ADDR_FLASH_SECTOR_22))
  {
    sector = FLASH_SECTOR_22;  
  }
  else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_23))*/
  {
    sector = FLASH_SECTOR_23;  
  }

  return sector;
}
