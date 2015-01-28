#include "includes.h"


void init_wdt(void)
{
    cspro = 0;
    cspro = 1; //计数源保护模式有效，当低速内部振荡器时钟为125KHz时，周期约为32.8ms

    prc1 = 1; //处理器模式寄存器1写保护
    pm12 = 1; //看门狗溢出复位
    prc1 = 0; //处理器模式寄存器1写保护

    wdtr = 0x00; //初始化看门狗计数器
    wdtr = 0xff;

    wdts = 1; //启动看门狗
}

void restart_wdt(void)
{
    wdtr = 0x00;
    wdtr = 0xff;
}

