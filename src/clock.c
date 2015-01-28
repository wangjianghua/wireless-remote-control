#include "includes.h"


void init_clock(void)
{
#if 0
    u16 stab_time;
    
    
    prc0 = 1; //系统时钟控制寄存器写保护

    cm13 = 1; //xin-xout管脚
    cm05 = 0; //主时钟xin-xout振荡

    ocd0 = 1; //外部主时钟振荡停止检测有效
    ocd1 = 1;

    stab_time = 1000;
    while(stab_time--)
    {
        if(!ocd3)
            break;
    }
    
    ocd2 = 0; //选择外部主时钟作为系统时钟
    
    cm16 = 0; //fclk，无分频
    cm17 = 0; 
    cm06 = 0; //cm17、cm16有效

    hra00 = 0; //高速内部振荡器停止

    cm14 = 0; //低速内部振荡器振荡

    hra01 = 0; //选择低速内部振荡器作系统时钟
    
    prc0 = 0; //系统时钟控制寄存器写保护 
#endif

    prc0 = 1; //系统时钟控制寄存器写保护
    
    hra00 = 1; //高速内部振荡器振荡
    
    hra01 = 1; //选择高速内部振荡器作系统时钟
    
    ocd2 = 1; //选择内部时钟作为系统时钟

    cm16 = 0; //fclk，无分频
    cm17 = 0; 
    cm06 = 0; //cm17、cm16有效

    cm14 = 0; //低速内部振荡器振荡

    prc0 = 0; //系统时钟控制寄存器写保护 
}

