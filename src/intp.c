#include "includes.h"


void init_int0(void)
{
    int0f |= 0x02; //f8数字滤波
   
    int0pl = 1; //两边沿
    pol_int0ic = 0; //下降沿
    
    ir_int0ic = 0; //清除中断标志位
    ilvl_int0ic = 2; //中断优先级

    pd4_5 = 0; //IO口设置为输入

    int0en = 1; //允许
}

void init_ki2(void)
{
    ki2pl = 0; //下降沿

    ir_kupic = 0; //清除中断标志位
    ilvl_kupic = 2; //中断优先级

    pd1_2 = 0; //IO口设置为输入
    
    ki2en = 1; //允许
}

void init_ki3(void)
{
    ki3pl = 0; //下降沿

    ir_kupic = 0; //清除中断标志位
    ilvl_kupic = 2; //中断优先级

    pd1_3 = 0; //IO口设置为输入
    
    ki3en = 1; //允许
}

void init_ki(void)
{
    init_ki2();
    init_ki3();
}

