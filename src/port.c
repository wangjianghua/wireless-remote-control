#include "includes.h"


void init_port(void)
{
    VCC_CTRL_PORT_INIT();
    VCC_ON(); //打开系统电源

    VDD_CTRL_PORT_INIT();
    VDD_OFF(); //关闭射频电源

    KEY_LED_PORT_INIT();
    KEY_LED_OFF(); //关闭指示灯

    KEY_LOCK_CTRL_PORT_INIT(); 
    KEY_UNLOCK_CTRL_PORT_INIT(); //按键管脚初始化

    /* 防止射频漏电 */
    pd3_3 = 1; //miso, output
    pd3_4 = 1; //short1 & cs, output
    pd3_5 = 1; //sclk, output
    pd3_7 = 1; //mosi, output

    pd4_5 = 1; //rf_intp, output

    p3_3 = 0; //rf off
    p3_4 = 0; //rf off
    p3_5 = 0; //rf off
    p3_7 = 0; //rf off

    p4_5 = 0; //rf off
}

