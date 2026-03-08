#ifndef __FLASH_H
#define __FLASH_H

#include "stm32f10x.h"

#define FLASH_START_ADDR  0x08000000
#define FLASH_USER_START_ADDR  0x0800FC00 

#define FLASH_CONFIG_ADDR  0x0800FC00 
#define CONFIG_FLAG        0x55AA    

void Flash_Write_Data(uint32_t WriteAddr, uint16_t *pBuffer, uint16_t NumToWrite);
void Flash_Read_Data(uint32_t ReadAddr, uint16_t *pBuffer, uint16_t NumToRead);
void Flash_Save(void) ;
void Flash_Load(void) ;


#endif
