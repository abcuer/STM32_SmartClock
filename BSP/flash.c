#include "flash.h"
extern uint16_t RTC_Time[];

/**
  * @brief  向指定地址写入数据
  * @param  WriteAddr: 起始地址 (必须是2的倍数)
  * @param  pBuffer: 指向待写入数据的指针
  * @param  NumToWrite: 写入数据的半字(16位)数量
  */
void Flash_Write_Data(uint32_t WriteAddr, uint16_t *pBuffer, uint16_t NumToWrite) {
    uint16_t i;
    
    FLASH_Unlock();               // 1. 解锁
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR); // 清除标志位

    // 2. 擦除页 (注意：写入前必须擦除，否则无法将0写为1)
    // 这里简单演示直接擦除目标地址所在的页
    FLASH_ErasePage(WriteAddr);

    // 3. 循环写入半字
    for (i = 0; i < NumToWrite; i++) {
        FLASH_ProgramHalfWord(WriteAddr + (i * 2), pBuffer[i]);
    }

    FLASH_Lock();                 // 4. 上锁
}

/**
  * @brief  从指定地址读取数据
  */
void Flash_Read_Data(uint32_t ReadAddr, uint16_t *pBuffer, uint16_t NumToRead) {
    uint16_t i;
    for (i = 0; i < NumToRead; i++) {
        pBuffer[i] = *(__IO uint16_t*)ReadAddr; // 直接寻址读取
        ReadAddr += 2;
    }
}

void Flash_Save(void) 
{
	uint16_t data_to_save[7];
	data_to_save[0] = CONFIG_FLAG; // 写入标志位
	for(int i = 0; i < 6; i++) {
		data_to_save[i+1] = RTC_Time[i];
	}
	Flash_Write_Data(FLASH_CONFIG_ADDR, data_to_save, 7);
}

void Flash_Load(void) 
{
    uint16_t read_data[7];
    Flash_Read_Data(FLASH_CONFIG_ADDR, read_data, 7);
    
    // 检查标志位，如果 Flash 里是空的（0xFFFF），就不覆盖默认值
    if (read_data[0] == CONFIG_FLAG) {
        for(int i = 0; i < 6; i++) {
            RTC_Time[i] = read_data[i+1];
        }
    }
}

