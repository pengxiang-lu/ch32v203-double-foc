#include "driver_config.h"
//  Attention: flash擦写次数为10万次，故不可在死循环中反复调用flash函数  //
/**
  * @brief    擦除指定FLASH地址页内的内容
  * @param    add 32位FLASH地址
  * @retval   无
  */
void Flash_Erase(uint32_t add)
{
    FLASH_Unlock(); //解锁FLASH编程擦除控制器
    FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_OPTERR|FLASH_FLAG_WRPRTERR);//清除标志位
    FLASH_ErasePage(add);    //擦除指定地址页
    FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_OPTERR|FLASH_FLAG_WRPRTERR);//清除标志位
    FLASH_Lock();    //锁定FLASH编程擦除控制器
}
/**
  * @brief   flash写入数据
  * @param   add 32位flash地址
  * @param   dat 16位数据
  * @retval  无
  */
void Flash_WriteHalfWord(uint32_t add,uint16_t data)
{
     FLASH_Unlock(); //解锁FLASH编程擦除控制器
     FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_OPTERR|FLASH_FLAG_WRPRTERR);//清除标志位
     //FLASH_ErasePage(add);    //擦除指定地址页
     FLASH_ProgramHalfWord(add,data); //从指定页的addr地址开始写
     FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_OPTERR|FLASH_FLAG_WRPRTERR);//清除标志位
     FLASH_Lock();    //锁定FLASH编程擦除控制器
}

/**
  * @brief    FLASH读出数据
  * @param    add 32位读出FLASH地址
  * @retval   16位数据
  */
uint16_t FLASH_Read(uint32_t add)
{
    u16 a;
    a = *(__IO uint16_t*)add;
    return a;
}


