#include "includes.h"


void init_port(void)
{
    VCC_CTRL_PORT_INIT();
    VCC_ON(); //��ϵͳ��Դ

    VDD_CTRL_PORT_INIT();
    VDD_OFF(); //�ر���Ƶ��Դ

    KEY_LED_PORT_INIT();
    KEY_LED_OFF(); //�ر�ָʾ��

    KEY_LOCK_CTRL_PORT_INIT(); 
    KEY_UNLOCK_CTRL_PORT_INIT(); //�����ܽų�ʼ��

    /* ��ֹ��Ƶ©�� */
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

