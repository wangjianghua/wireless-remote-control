/** 
 * @file     flash.c
 * @brief    flash自编程
 * @details  瑞萨单片机(nec mcu)，R8C/1B - R5F211B4，flash自编程实现
 * @author   华兄
 * @email    591881218@qq.com
 * @date     2013
 * @version  vX.XX
 * @par Copyright (c):  
 *           深圳市合尔凯科技有限公司
 * @par History:          
 *   version: author, date, desc\n 
 */ 

#include "includes.h"


u8 full_status_check(u8 *check_addr)
{
    u8 fmr_06, fmr_07;
    

    fmr_06 = fmr06;
    fmr_07 = fmr07;

    if((1 == fmr_06) && (1 == fmr_07))
    {
        *check_addr = FLASH_CLEAR_STATUS_CMD; //执行清除状态寄存器软件命令
        return (CMD_SEQ_ERROR);
    }

    if(1 == fmr_06)
    {
        *check_addr = FLASH_CLEAR_STATUS_CMD;
        return (PROGRAM_ERROR);
    }

    if(1 == fmr_07)
    {
        *check_addr = FLASH_CLEAR_STATUS_CMD;
        return (ERASE_ERROR);
    }

    return (NORMAL);
}

u8 flash_erase_block(u8 block)
{
    u8 result;
    u8 *erase_addr = NULL;
    

    switch(block)
    {
    case FLASH_BLOCK_A:
        erase_addr = FLASH_BLOCK_A_ADDR;
        break;
        
    case FLASH_BLOCK_B:
        erase_addr = FLASH_BLOCK_B_ADDR;
        break;
        
    default:
        return (BLOCK_SELECT_ERROR);
    }

    asm("FCLR I"); //关中断
    
    fmr01 = 0; //进入CPU改写模式
    asm(" ");
    fmr01 = 1;
    
    fmr11 = 0; //设置为EW1模式
    asm(" ");
    fmr11 = 1;
    
    fmr40 = 0; //允许中断擦除挂起 
    asm(" ");
    fmr40 = 1;

    fmr47 = 0; //禁止低消费电流读模式
    
    *erase_addr = FLASH_ERASE_CMD1; //执行块擦除软件命令
    asm("FSET I"); //开中断
    *erase_addr = FLASH_ERASE_CMD2;
    
    while(1) //判断擦除是否结束
    { 
        CLR_WDT();
        
        fmr41 = 0; //中断返回后继续擦除

        if(1 == fmr00)
        {
            break;
        }
    }

    result = full_status_check(erase_addr); //全状态检测

    fmr40 = 0; //禁止挂起功能
    fmr01 = 0; //退出CPU改写模式

    return (result);
}

u8 flash_write_data(u8 block, u16 offset, u8 *buf, u16 len)
{
    u8 result;
    u8 *write_addr = NULL, *block_end_addr = NULL;
    

    switch(block)
    {
    case FLASH_BLOCK_A:
        write_addr = FLASH_BLOCK_A_ADDR + offset;
        block_end_addr = FLASH_BLOCK_A_END_ADDR;
        break;
        
    case FLASH_BLOCK_B:
        write_addr = FLASH_BLOCK_B_ADDR + offset;
        block_end_addr = FLASH_BLOCK_B_END_ADDR;
        break;
        
    default:
        return (BLOCK_SELECT_ERROR);
    }

    if((write_addr + len) > block_end_addr)
    {
        return (OUT_OF_RANGE);
    }
    
    asm("FCLR I"); //关中断
    
    fmr01 = 0; //进入CPU改写模式
    asm(" ");
    fmr01 = 1;
    
    fmr11 = 0; //设置为EW1模式
    asm(" ");
    fmr11 = 1;
    
    fmr40 = 0; //允许中断擦除挂起 
    asm(" ");
    fmr40 = 1;

    fmr47 = 0; //禁止低消费电流读模式
    
    asm("FSET I"); //开中断

    for(u16 i=0; i<len; i++)
    {
        write_addr[i] = FLASH_WRITE_CMD; //执行编程软件命令
        asm(" ");
        write_addr[i] = buf[i];    

        while(1) //判断编程是否结束
        {
            CLR_WDT();
            
            fmr42 = 0; //中断返回后继续编程

            if(0 == fmr44)
            {
                break;
            }
        }

        result = full_status_check(write_addr); //全状态检测

        if(NORMAL != result)
        {
            break;
        }          
    }

    fmr40 = 0; //禁止挂起功能
    fmr01 = 0; //退出CPU改写模式
    
    return (result);
}

u8 flash_read_data(u8 block, u16 offset, u8 *buf, u16 len)
{
    u8 *read_addr = NULL, *block_end_addr = NULL;
    

    switch(block)
    {
    case FLASH_BLOCK_A:
        read_addr = FLASH_BLOCK_A_ADDR + offset;
        block_end_addr = FLASH_BLOCK_A_END_ADDR;
        break;
        
    case FLASH_BLOCK_B:
        read_addr = FLASH_BLOCK_B_ADDR + offset;
        block_end_addr = FLASH_BLOCK_B_END_ADDR;
        break;
        
    default:
        return (BLOCK_SELECT_ERROR);
    }

    if((read_addr + len) > block_end_addr)
    {
        return (OUT_OF_RANGE);
    }

    asm("FCLR I"); //关中断
    
    fmr01 = 0; //进入CPU改写模式
    asm(" ");
    fmr01 = 1;
    
    fmr11 = 0; //设置为EW1模式
    asm(" ");
    fmr11 = 1;
    
    fmr40 = 0; //允许中断擦除挂起 
    asm(" ");
    fmr40 = 1;

    fmr47 = 0; //禁止低消费电流读模式
    
    asm("FSET I"); //开中断

    *read_addr = FLASH_READ_CMD; //执行读阵列软件命令

    for(u16 i=0; i<len; i++)
    {
        CLR_WDT();
        
        buf[i] = read_addr[i];        
    }

    fmr40 = 0; //禁止挂起功能
    fmr01 = 0; //退出CPU改写模式

    return (NORMAL);
}

void flash_write_init(void)
{
    /** 
     * 来源：瑞萨单片机论坛
     *
     * R8C\Tiny 单片机的DataFlash处理起来比较繁琐，处理模式分为EW0模式和EW1模式，通常我们采用EW1模式。　　
     * 在此模式中，DataFlash的一个块擦除时间比较长，经常达到100ms以上，这个时间远远超出了WDT的时间，所以在擦除DataFlash时，如果处理不当，会导致单片机的死机。
　　 * 在擦除DataFlash时，主程序已经停止，在等待擦除DataFlash时结束，此时不可能在主程序中喂狗；在R8C\Tiny 单片机，当WDT开启之后，程序将不能关闭WDT。
　　 * 综合以上的特点，要处理擦除DataFlash时的WDT溢出问题，处理方法如下：
　　 * 1、开启擦除挂起
　　 * 2、WDT设置成中断模式，普通模式
　　 * 3、执行擦除命令
　　 * 4、擦除中，WDT溢出中断，可以忽略或者喂狗，判断状态软件复位
     */
    flash_erase_block(FLASH_BLOCK_A);
    flash_erase_block(FLASH_BLOCK_B);
}

