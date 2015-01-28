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
    VDD_ON(); //打开射频电源
    
    init_rf();
    EI();
#elif (LOW_POWER_TEST_EN == 0U)
    chk_sys_sta(); //检测系统状态
#endif
}

// -------------------------------------------------------------------

void power_off(void)
{
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
    
    VDD_CTRL_PORT_INIT();
    VDD_OFF(); //关闭射频电源

    VCC_CTRL_PORT_INIT();
    VCC_OFF(); //关闭系统电源
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
    vca2 &= 0x3f; //电压检测电路无效
    prc3 = 0;

    ilvl_txic = 0; //定时器X中断禁能
    txs = 0; //定时器X停止计数

    ilvl_tzic = 0; //定时器Z中断禁能
    tzs = 0; //定时器Z停止计数

    ilvl_int0ic = 0; //INT0中断禁能
    int0en = 0; //INT0禁止

    ilvl_s0tic = 0; //uart0发送中断禁能
    te_u0c1 = 0; //禁止发送
    ilvl_s0ric = 0; //uart0接收中断禁能
    re_u0c1 = 0; //禁止接收

    /* 不需要按键中断，进入stop模式，按键释放，电源会自动切断 */
    //init_ki(); //打开按键中断
    
    asm("FSET I");
}

void stop_mode(void)
{
    asm("FCLR I");

    fmr01 = 0; //CPU改写模式无效
    
    prc0 = 1;
    ocd &= 0x0c; //振荡停止检测无效
    asm("FSET I");
    cm10 = 1; //全时钟停止(停止模式)
    asm("jmp.b _dummy_jump");
    asm("_dummy_jump:");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("FCLR I");
    for(u16 i=0; i<1000; i++) {CLR_WDT();} //等待振荡稳定
    asm("FSET I");
    prc0 = 0;
}

void wait_mode(void)
{
    asm("FCLR I");

    fmr01 = 0; //CPU改写模式无效
    
    prc0 = 1;
    cm02 = 1; //停止外围时钟 
    //ocd &= 0x0c; //振荡停止检测无效，不能使用
    cm05 = 1; //停止主时钟
    prc0 = 0;
    
    asm("FSET I"); //开中断
    
    asm("wait"); //等待模式
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("FCLR I");
    for(u16 i=0; i<1000; i++) {CLR_WDT();} //等待振荡稳定
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
    power_off(); //第一次关电源，即使关不断系统电源，也能关闭射频电源

    EI();
    g_key_param.start_time = get_tick(); //等待按键释放的开始时间
    
    while(1) 
    {
        CLR_WDT();
        
        if((KEY_LOCK && KEY_UNLOCK) || //等待按键释放，同时亮灯警告
            check_timeout(g_key_param.start_time, MAX_KEY_RELEASED_CHECK_TIME)) //一段时间以后，退出等待
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
    for(u8 i=0; i<3; i++) //尝试三次关闭电源，如果失败，则进入待机模式
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

        if(check_timeout(start_time, POWER_CTRL_CHECK_TIME)) //按键时间过长，或者电源控制电路故障，告警
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
    SHORT2_CTRL_PORT_INIT(0); //p1_6输入
    SHORT1_HIGH();

    for(u8 i=0; i<3; i++)
    {
        if(!(KEY_LOCK && KEY_UNLOCK))
        {
            g_key_param.press = TRUE;
        }

        mdelay(1);
    }

    if(SHORT2()) //检测短接
    {
        SHORT1_LOW();
        mdelay(1);
        
        if(!SHORT2())
        {           
            g_uart_param.shorted = TRUE; //有短接
            
            if(FALSE == g_key_param.press) //没有按键
            {
                power_off(); //关闭电池供电
                
                init_uart();
                g_uart_param.rx.start_time = get_tick(); //设置开始接收的时间
            }
            else //既短接，又有按键，射频发送固定长度的测试数据
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
                VDD_ON(); //打开射频电源
                
                init_rf();
                EI();
            }
        }
        else
        {
            g_uart_param.shorted = FALSE; //没有短接
        }
    }
    else
    {
        g_uart_param.shorted = FALSE; //没有短接
    }

    if(FALSE == g_uart_param.shorted)
    {
        SHORT2_CTRL_PORT_INIT(1); //p1_6输出
    }

    if((FALSE == g_uart_param.shorted) && (TRUE == g_key_param.press)) //没有短接，有按键
    {
        if(!KEY_LOCK && KEY_UNLOCK) //按下LOCK键
        {
            mdelay(5); //按键去抖

            if(!KEY_LOCK && KEY_UNLOCK)
            {
                KEY_LED_ON();
                g_key_param.msg = KEY_MSG_LOCK;
                sys_evt_add(SYS_EVT_KEY);
            }
            else //按键抖动
            {
                sys_sta_err = TRUE; //非法状态
            }
        }
        else if(KEY_LOCK && !KEY_UNLOCK) //按下UNLOCK键
        {
            mdelay(5); //按键去抖

            if(KEY_LOCK && !KEY_UNLOCK)
            {
                KEY_LED_ON();
                g_key_param.msg = KEY_MSG_UNLOCK;
                sys_evt_add(SYS_EVT_KEY);
            }
            else //按键抖动
            {
                sys_sta_err = TRUE; //非法状态
            }
        }
        else //同时按两键
        {
            sys_sta_err = TRUE; //非法状态  
        }
    }
    else if((FALSE == g_uart_param.shorted) && (FALSE == g_key_param.press)) //没有短接，没有按键
    {
        sys_sta_err = TRUE; //非法状态
    }

    if(TRUE == sys_sta_err)
    {
        SHUTDOWN(); //系统自我关闭
    }
}

// -------------------------------------------------------------------
