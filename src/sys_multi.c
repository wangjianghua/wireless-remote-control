#include "includes.h"


COM_PARAM g_com_param;

// -------------------------------------------------------------------

void _variable_init(void)
{
#if (VARIABLES_MEMSET_EN > 0U)
    memset(&g_rf_param, 0, sizeof(RF_PARAM));
#else
    g_rf_param.tx.timeout = 0;
#endif
    g_rf_param.idle_state = RF_STATE_READY;
    g_rf_param.state_monitor = 0;
    g_rf_param.rst_dp = 0;

#if (VARIABLES_MEMSET_EN > 0U)
    memset(&g_uart_param, 0, sizeof(UART_PARAM));
#else
    g_uart_param.rx.bytes = 0;
    g_uart_param.rx.timeout = 0;
    g_uart_param.rx.recv = FALSE;
    g_uart_param.shorted = FALSE;
#endif
    g_uart_param.rx.write_index = g_uart_param.rx.buf;
    g_uart_param.rx.read_index = g_uart_param.rx.buf;
    g_uart_param.tx.check_time = CHECK_TIME_255B_9600;

#if (VARIABLES_MEMSET_EN > 0U)
    memset(&g_com_param, 0, sizeof(COM_PARAM));
#else
    g_com_param.len = 0;
#endif
    g_com_param.ext.rf.data = g_com_param.buf;

#if (VARIABLES_MEMSET_EN > 0U)
    memset(&g_key_param, 0, sizeof(KEY_PARAM));
#else
    g_key_param.press = FALSE;
#endif

    g_app_tick = 0;
}

// -------------------------------------------------------------------

void _device_init(void)
{
    DI();
    init_clock();
    init_timer();
    init_wdt();
    EI();

#if (RF_AUTO_SEND_EN > 0U)
    DI();
    init_int0();
    init_lvi();
    init_spi();
    
    VDD_CTRL_PORT_INIT();
    VDD_ON(); //����Ƶ��Դ
    
    init_rf();
    EI();
#elif (LOW_POWER_TEST_EN == 0U)
    chk_sys_sta(); //���ϵͳ״̬
#endif
}

// -------------------------------------------------------------------

void power_off(void)
{
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
    
    VDD_CTRL_PORT_INIT();
    VDD_OFF(); //�ر���Ƶ��Դ

    VCC_CTRL_PORT_INIT();
    VCC_OFF(); //�ر�ϵͳ��Դ
}

void config_port(void)
{
    pd1_0 = 1; //vcc_ctrl, output
    pd1_1 = 1; //led_ctrl, output
    pd1_2 = 0; //ki2, input
    pd1_3 = 0; //ki3, input
    pd1_4 = 1; //txd, output
    pd1_5 = 1; //rxd, output
    pd1_6 = 1; //short2, output
    pd1_7 = 1; //vdd_ctrl & exten, output

    pd3_3 = 1; //miso, output
    pd3_4 = 1; //short1 & cs, output
    pd3_5 = 1; //sclk, output
    pd3_7 = 1; //mosi, output

    pd4_5 = 1; //rf_intp, output

    p1_0 = 0; //vcc off
    p1_1 = 0; //led off
    p1_4 = 1;
    p1_5 = 1;
    p1_6 = 1;
    p1_7 = 0; //vdd off, rf off

    p3_3 = 0; //rf off
    p3_4 = 0; //rf off
    p3_5 = 0; //rf off
    p3_7 = 0; //rf off

    p4_5 = 0; //rf off
}

void config_intp(void)
{
    asm("FCLR I");
    
    prc3 = 1;
    vca2 &= 0x3f; //��ѹ����·��Ч
    prc3 = 0;

    ilvl_txic = 0; //��ʱ��X�жϽ���
    txs = 0; //��ʱ��Xֹͣ����

    ilvl_tzic = 0; //��ʱ��Z�жϽ���
    tzs = 0; //��ʱ��Zֹͣ����

    ilvl_int0ic = 0; //INT0�жϽ���
    int0en = 0; //INT0��ֹ

    ilvl_s0tic = 0; //uart0�����жϽ���
    te_u0c1 = 0; //��ֹ����
    ilvl_s0ric = 0; //uart0�����жϽ���
    re_u0c1 = 0; //��ֹ����

    /* ����Ҫ�����жϣ�����stopģʽ�������ͷţ���Դ���Զ��ж� */
    //init_ki(); //�򿪰����ж�
    
    asm("FSET I");
}

void stop_mode(void)
{
    asm("FCLR I");

    fmr01 = 0; //CPU��дģʽ��Ч
    
    prc0 = 1;
    ocd &= 0x0c; //��ֹͣ�����Ч
    asm("FSET I");
    cm10 = 1; //ȫʱ��ֹͣ(ֹͣģʽ)
    asm("jmp.b _dummy_jump");
    asm("_dummy_jump:");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("FCLR I");
    for(u16 i=0; i<1000; i++) {CLR_WDT();} //�ȴ����ȶ�
    asm("FSET I");
    prc0 = 0;
}

void wait_mode(void)
{
    asm("FCLR I");

    fmr01 = 0; //CPU��дģʽ��Ч
    
    prc0 = 1;
    cm02 = 1; //ֹͣ��Χʱ�� 
    //ocd &= 0x0c; //��ֹͣ�����Ч������ʹ��
    cm05 = 1; //ֹͣ��ʱ��
    prc0 = 0;
    
    asm("FSET I"); //���ж�
    
    asm("wait"); //�ȴ�ģʽ
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("FCLR I");
    for(u16 i=0; i<1000; i++) {CLR_WDT();} //�ȴ����ȶ�
    asm("FSET I");
}

void soft_reset(void)
{
    prc1 = 1;
    pm03 = 1;
    prc1 = 0;
}

// -------------------------------------------------------------------

void shutdown(void)
{
    power_off(); //��һ�ιص�Դ����ʹ�ز���ϵͳ��Դ��Ҳ�ܹر���Ƶ��Դ

    EI();
    g_key_param.start_time = get_tick(); //�ȴ������ͷŵĿ�ʼʱ��
    
    while(1) 
    {
        CLR_WDT();
        
        if((KEY_LOCK && KEY_UNLOCK) || //�ȴ������ͷţ�ͬʱ���ƾ���
            check_timeout(g_key_param.start_time, MAX_KEY_RELEASED_CHECK_TIME)) //һ��ʱ���Ժ��˳��ȴ�
        {

            if(KEY_LOCK && KEY_UNLOCK)
            {
                g_key_param.press = FALSE;
            }
            
            KEY_LED_OFF();
            
            break;
        }
    }

#if 0
    for(u8 i=0; i<3; i++) //�������ιرյ�Դ�����ʧ�ܣ���������ģʽ
    {
        power_off();
    }
#endif

    config_port();
    config_intp();
    stop_mode();

    while(1)
    {
        CLR_WDT();

        power_off();
    }

#if 0
    config_port();

    tick start_time;


    start_time = get_tick();
    
    while(1)
    {
        CLR_WDT();
        power_off();

        if(check_timeout(start_time, POWER_CTRL_CHECK_TIME)) //����ʱ����������ߵ�Դ���Ƶ�·���ϣ��澯
        {
            KEY_LED_ON();
        }
    }
#endif    
}

void chk_sys_sta(void)
{
    bool sys_sta_err = FALSE;
    

    SHORT1_CTRL_PORT_INIT();
    SHORT2_CTRL_PORT_INIT(0); //p1_6����
    SHORT1_HIGH();

    for(u8 i=0; i<3; i++)
    {
        if(!(KEY_LOCK && KEY_UNLOCK))
        {
            g_key_param.press = TRUE;
        }

        mdelay(1);
    }

    if(SHORT2()) //���̽�
    {
        SHORT1_LOW();
        mdelay(1);
        
        if(!SHORT2())
        {           
            g_uart_param.shorted = TRUE; //�ж̽�
            
            if(FALSE == g_key_param.press) //û�а���
            {
                power_off(); //�رյ�ع���
                
                init_uart();
                g_uart_param.rx.start_time = get_tick(); //���ÿ�ʼ���յ�ʱ��
            }
            else //�ȶ̽ӣ����а�������Ƶ���͹̶����ȵĲ�������
            {
                KEY_LED_ON();

                for(u8 i=0; i<MAX_TEST_DATA_LEN; i++)
                {
                    g_com_param.buf[i] = i; 
                }
                
                g_com_param.len = MAX_TEST_DATA_LEN;

                g_com_param.ext.rf.data = g_com_param.buf;
                g_com_param.ext.rf.len = g_com_param.len;
                g_com_param.len = 0;
                
                sys_evt_add(SYS_EVT_RF_SEND);      
                
                DI();
                init_int0();
                init_lvi();
                init_spi();
                
                VDD_CTRL_PORT_INIT();
                VDD_ON(); //����Ƶ��Դ
                
                init_rf();
                EI();
            }
        }
        else
        {
            g_uart_param.shorted = FALSE; //û�ж̽�
        }
    }
    else
    {
        g_uart_param.shorted = FALSE; //û�ж̽�
    }

    if(FALSE == g_uart_param.shorted)
    {
        SHORT2_CTRL_PORT_INIT(1); //p1_6���
    }

    if((FALSE == g_uart_param.shorted) && (TRUE == g_key_param.press)) //û�ж̽ӣ��а���
    {
        if(!KEY_LOCK && KEY_UNLOCK) //����LOCK��
        {
            mdelay(5); //����ȥ��

            if(!KEY_LOCK && KEY_UNLOCK)
            {
                KEY_LED_ON();
                g_key_param.msg = KEY_MSG_LOCK;
                sys_evt_add(SYS_EVT_KEY);
            }
            else //��������
            {
                sys_sta_err = TRUE; //�Ƿ�״̬
            }
        }
        else if(KEY_LOCK && !KEY_UNLOCK) //����UNLOCK��
        {
            mdelay(5); //����ȥ��

            if(KEY_LOCK && !KEY_UNLOCK)
            {
                KEY_LED_ON();
                g_key_param.msg = KEY_MSG_UNLOCK;
                sys_evt_add(SYS_EVT_KEY);
            }
            else //��������
            {
                sys_sta_err = TRUE; //�Ƿ�״̬
            }
        }
        else //ͬʱ������
        {
            sys_sta_err = TRUE; //�Ƿ�״̬  
        }
    }
    else if((FALSE == g_uart_param.shorted) && (FALSE == g_key_param.press)) //û�ж̽ӣ�û�а���
    {
        sys_sta_err = TRUE; //�Ƿ�״̬
    }

    if(TRUE == sys_sta_err)
    {
        SHUTDOWN(); //ϵͳ���ҹر�
    }
}

// -------------------------------------------------------------------
