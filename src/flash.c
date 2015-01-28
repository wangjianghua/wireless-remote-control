/** 
 * @file     flash.c
 * @brief    flash�Ա��
 * @details  ������Ƭ��(nec mcu)��R8C/1B - R5F211B4��flash�Ա��ʵ��
 * @author   ����
 * @email    591881218@qq.com
 * @date     2013
 * @version  vX.XX
 * @par Copyright (c):  
 *           �����к϶����Ƽ����޹�˾
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
        *check_addr = FLASH_CLEAR_STATUS_CMD; //ִ�����״̬�Ĵ����������
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

    asm("FCLR I"); //���ж�
    
    fmr01 = 0; //����CPU��дģʽ
    asm(" ");
    fmr01 = 1;
    
    fmr11 = 0; //����ΪEW1ģʽ
    asm(" ");
    fmr11 = 1;
    
    fmr40 = 0; //�����жϲ������� 
    asm(" ");
    fmr40 = 1;

    fmr47 = 0; //��ֹ�����ѵ�����ģʽ
    
    *erase_addr = FLASH_ERASE_CMD1; //ִ�п�����������
    asm("FSET I"); //���ж�
    *erase_addr = FLASH_ERASE_CMD2;
    
    while(1) //�жϲ����Ƿ����
    { 
        CLR_WDT();
        
        fmr41 = 0; //�жϷ��غ��������

        if(1 == fmr00)
        {
            break;
        }
    }

    result = full_status_check(erase_addr); //ȫ״̬���

    fmr40 = 0; //��ֹ������
    fmr01 = 0; //�˳�CPU��дģʽ

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
    
    asm("FCLR I"); //���ж�
    
    fmr01 = 0; //����CPU��дģʽ
    asm(" ");
    fmr01 = 1;
    
    fmr11 = 0; //����ΪEW1ģʽ
    asm(" ");
    fmr11 = 1;
    
    fmr40 = 0; //�����жϲ������� 
    asm(" ");
    fmr40 = 1;

    fmr47 = 0; //��ֹ�����ѵ�����ģʽ
    
    asm("FSET I"); //���ж�

    for(u16 i=0; i<len; i++)
    {
        write_addr[i] = FLASH_WRITE_CMD; //ִ�б���������
        asm(" ");
        write_addr[i] = buf[i];    

        while(1) //�жϱ���Ƿ����
        {
            CLR_WDT();
            
            fmr42 = 0; //�жϷ��غ�������

            if(0 == fmr44)
            {
                break;
            }
        }

        result = full_status_check(write_addr); //ȫ״̬���

        if(NORMAL != result)
        {
            break;
        }          
    }

    fmr40 = 0; //��ֹ������
    fmr01 = 0; //�˳�CPU��дģʽ
    
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

    asm("FCLR I"); //���ж�
    
    fmr01 = 0; //����CPU��дģʽ
    asm(" ");
    fmr01 = 1;
    
    fmr11 = 0; //����ΪEW1ģʽ
    asm(" ");
    fmr11 = 1;
    
    fmr40 = 0; //�����жϲ������� 
    asm(" ");
    fmr40 = 1;

    fmr47 = 0; //��ֹ�����ѵ�����ģʽ
    
    asm("FSET I"); //���ж�

    *read_addr = FLASH_READ_CMD; //ִ�ж������������

    for(u16 i=0; i<len; i++)
    {
        CLR_WDT();
        
        buf[i] = read_addr[i];        
    }

    fmr40 = 0; //��ֹ������
    fmr01 = 0; //�˳�CPU��дģʽ

    return (NORMAL);
}

void flash_write_init(void)
{
    /** 
     * ��Դ��������Ƭ����̳
     *
     * R8C\Tiny ��Ƭ����DataFlash���������ȽϷ���������ģʽ��ΪEW0ģʽ��EW1ģʽ��ͨ�����ǲ���EW1ģʽ������
     * �ڴ�ģʽ�У�DataFlash��һ�������ʱ��Ƚϳ��������ﵽ100ms���ϣ����ʱ��ԶԶ������WDT��ʱ�䣬�����ڲ���DataFlashʱ��������������ᵼ�µ�Ƭ����������
���� * �ڲ���DataFlashʱ���������Ѿ�ֹͣ���ڵȴ�����DataFlashʱ��������ʱ����������������ι������R8C\Tiny ��Ƭ������WDT����֮�󣬳��򽫲��ܹر�WDT��
���� * �ۺ����ϵ��ص㣬Ҫ�������DataFlashʱ��WDT������⣬���������£�
���� * 1��������������
���� * 2��WDT���ó��ж�ģʽ����ͨģʽ
���� * 3��ִ�в�������
���� * 4�������У�WDT����жϣ����Ժ��Ի���ι�����ж�״̬�����λ
     */
    flash_erase_block(FLASH_BLOCK_A);
    flash_erase_block(FLASH_BLOCK_B);
}

