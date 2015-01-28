#include "includes.h"


_sys_tick_t _sys_tick = NO_TICK;
_sys_sec_t _sys_sec = NO_SEC;
_sys_event_t _sys_event = NO_EVT;

void sys_evt_add( _sys_event_t evt )
{
    _disable_interrupt();
    _sys_event |= evt;
    _enable_interrupt();
}

void sys_evt_del( _sys_event_t evt )
{
    _disable_interrupt();
    _sys_event &= ~( evt | SYS_EVT_RESERVE );
    _enable_interrupt();
}

void on_tick(void)
{
    _sys_tick++;

    if(!(_sys_tick % SYS_TICKS_PER_SEC)) //One second
    {
        sys_evt_add(SYS_EVT_SECOND);
    }

    if((TRUE == g_uart_param.shorted) && (FALSE == g_key_param.press)) //如果有短接，且没有按键
    {
        if(get_bytes())
        {
            g_uart_param.rx.recv = TRUE;
            sys_evt_add(SYS_EVT_UART_RECV);
        }
        else if((FALSE == g_uart_param.rx.recv) && check_timeout(g_uart_param.rx.start_time, MAX_RECV_PKT_CHECK_TIME))
        {
            SHUTDOWN(); //60秒内串口接收无数据，系统自我关闭
        }
    }

    if(g_uart_param.rx.timeout) //处理串口接收超时
    {
        g_uart_param.rx.timeout--;

        if(!g_uart_param.rx.timeout)
        {
            g_com_param.ext.rf.data = g_com_param.buf;
            g_com_param.ext.rf.len = g_com_param.len;
            g_com_param.len = 0;

#if (UART_DEBUG_EN > 0U)
            if(g_uart_param.recv_pkt_overflow)
            {
                g_uart_param.recv_pkt_overflow = 0;
                DEBUG_PRINT(("<uart_recv_overflow> %s(%d) - %s\n", __FILE__, __LINE__, __FUNCTION__));
            }
#endif

            sys_evt_add(SYS_EVT_USER);
        }
    }
    
    if(g_rf_param.tx.timeout) //处理射频发送超时
    {
        g_rf_param.tx.timeout--;
        
        if(!g_rf_param.tx.timeout)
        {
#if (RF_AUTO_SEND_EN > 0U)
            g_rf_param.tx.complete = TRUE; //发送超时，强制结束
            
            TX_LED_OFF();

            rf_reset_chip();
            
#if (RF_DEBUG_EN > 0U)
            g_rf_param.send_timeout_times++; //发送超时次数
            g_rf_param.send_pkt_num++; //发送包数目
            g_rf_param.reset_chip_times++;
#endif

#else
            SHUTDOWN(); //异常，系统自我关闭
#endif
        }
    }

#if 0
    if(g_rf_param.rx.timeout) //处理射频接收超时
    {
        g_rf_param.rx.timeout--;
        
        if(!g_rf_param.rx.timeout)
        {
            RX_LED_OFF();

            rf_reset_chip();
            
#if (RF_DEBUG_EN > 0U)
            g_rf_param.recv_timeout_times++; //接收超时次数
            g_rf_param.recv_pkt_num++; //接收包数目
            g_rf_param.reset_chip_times++;
#endif
        }
    }
#endif
}

void on_second(void)
{
    _sys_sec++;
    
#if (RF_AUTO_SEND_EN > 0U)
    g_com_param.len = strlen("hello world!\n");

    memcpy(g_com_param.buf,
           "hello world!\n",
           g_com_param.len);

    g_com_param.ext.rf.data = g_com_param.buf;
    g_com_param.ext.rf.len = g_com_param.len;
    g_com_param.len = 0;

    sys_evt_add(SYS_EVT_RF_SEND);
#endif
}

void on_rf_intp(void)
{   
    u8 rf_intp_sta1;


    rf_intp_sta1 = rf_read_reg(INTP_STA1_ADDR); //获取射频中断状态
    rf_read_reg(INTP_STA2_ADDR); //清intp_sta2中断
    
	if(FF_ERROR == (rf_intp_sta1 & FF_ERROR)) 
	{//FIFO操作出错
#if (RF_AUTO_SEND_EN > 0U)
        rf_reset_chip();
        
#if (RF_DEBUG_EN > 0U)
	    g_rf_param.ff_error_times++;
        g_rf_param.reset_chip_times++;
#endif

#else
        SHUTDOWN(); //异常，系统自我关闭
#endif
	}    
    
#if 0
    else if(RXFF_AF == (rf_intp_sta1 & RXFF_AF)) 
    {//接收FIFO将满中断
        if(RF_STATE_RX != g_rf_param.state)
        {
            RX_LED_ON();
            g_rf_param.state = RF_STATE_RX; //射频转到接收态
            g_rf_param.rx.pkt_len = rf_read_reg(RXPKT_LEN_ADDR); //读取数据包长度

            rf_rssi_monitor(); //接收信号强度
            rf_fdev_monitor(); //放在将满中断合适，处在接收过程中

            if(g_rf_param.rx.pkt_len >= MAX_RF_RECV_BUF_LEN) //限制接收包长，过长丢弃此包
            {
#if (RF_DEBUG_EN > 0U)
                g_rf_param.recv_pkt_overlength_times++;
#endif

                DEBUG_PRINT(("<rf_recv_overlength>"));

        		rf_state_idle();
        		rf_state_listen();
                
                RX_LED_OFF();
                return;
            }
            
            g_rf_param.rx.index = 0; //复位接收数据索引
            g_rf_param.rx.timeout = RF_TRANSMIT_TIMEOUT; //设置接收超时时间
        }
               
        rf_read_fifo(RF_RX_AF_THRESHOLD); 
    }
    else if(RXPKT_DONE == (rf_intp_sta1 & RXPKT_DONE))
    {//接收FIFO完成中断
        if(RF_STATE_RX != g_rf_param.state)
        {
            RX_LED_ON();
            g_rf_param.state = RF_STATE_RX; //射频转到接收态
            g_rf_param.rx.pkt_len = rf_read_reg(RXPKT_LEN_ADDR); //读取数据包长度

            rf_rssi_monitor(); //接收信号强度

            if(g_rf_param.rx.pkt_len >= MAX_RF_RECV_BUF_LEN)
            {
#if (RF_DEBUG_EN > 0U)
                g_rf_param.recv_pkt_overlength_times++;
#endif

                DEBUG_PRINT(("<rf_recv_overlength>"));

        		rf_state_idle();
        		rf_state_listen();
                
                RX_LED_OFF();
                return;
            }
            
            g_rf_param.rx.index = 0; //复位接收数据索引
            g_rf_param.rx.timeout = RF_TRANSMIT_TIMEOUT; //设置接收超时时间
        }

        u8 remain_bytes;
        
        
        remain_bytes = g_rf_param.rx.pkt_len - g_rf_param.rx.index; //射频剩余字节数
        
        rf_read_fifo(remain_bytes);
        
#if (RF_DEBUG_EN > 0U)
        if(g_rf_param.rx.index != g_rf_param.rx.pkt_len) //接收的数据不完整
        {
            g_rf_param.recv_pkt_lost_times++;  
            
            DEBUG_PRINT(("<rf_recv_lost>"));
        }

        if(CRC_ERROR == (rf_intp_sta1 & CRC_ERROR)) //硬件crc校验错误
        {
            g_rf_param.crc_error_times++;

            DEBUG_PRINT(("<rf_crc_error>"));
        }

        g_rf_param.recv_pkt_num++; //接收包数目
#endif 

        g_com_param.ext.uart.data = g_rf_param.rx.buf;
        g_com_param.ext.uart.len = g_rf_param.rx.index;

        sys_evt_add(SYS_EVT_UART_SEND);
 
        g_rf_param.rx.timeout = 0; //清超时计数器

		rf_state_idle();
		rf_state_listen();
        
        RX_LED_OFF();
    }
#endif

    else if(TXFF_AF == (rf_intp_sta1 & TXFF_AF))
    {//发送FIFO将满中断
        rf_state_tx(); //fifo将满，启动射频发送数据
    }
    else if(TXPKT_DONE == (rf_intp_sta1 & TXPKT_DONE))
    {//发送FIFO完成中断
#if (RF_AUTO_SEND_EN > 0U)

#if (RF_DEBUG_EN > 0U)
        g_rf_param.send_pkt_num++; //发送包数目
#endif

        g_rf_param.tx.timeout = 0; //清超时计数器
        g_rf_param.tx.complete = TRUE; //发送完成

        rf_state_idle();
        rf_state_listen();
        
        TX_LED_OFF();
#else
        SHUTDOWN(); //发射完成，系统自我关闭
#endif
    }
    else if(TXFF_AE == (rf_intp_sta1 & TXFF_AE))
    {//发送FIFO将空中断
        rf_write_fifo();
    }
}

void on_uart_recv(void)
{
    u8 ch;


    while(get_bytes()) //串口一边接收数据，事件一边处理接收的数据，射频空中波特率为19200
    {
        CLR_WDT();
        
        ch = get_char();
        g_uart_param.rx.timeout = UART_TRANSMIT_TIMEOUT;
        
        g_com_param.buf[g_com_param.len++] = ch;

        if(g_com_param.len >= UART_RX_AF_THRESHOLD) //达到串口接收门限，转发串口数据
        {
            g_uart_param.rx.timeout = 0; //处理门限整数倍数据

#if (UART_DEBUG_EN > 0U)
            if(g_uart_param.recv_pkt_overflow)
            {
                g_uart_param.recv_pkt_overflow = 0;
                DEBUG_PRINT(("<uart_recv_overflow> %s(%d) - %s\n", __FILE__, __LINE__, __FUNCTION__));
            }
#endif
            
            g_com_param.ext.rf.data = g_com_param.buf;
            g_com_param.ext.rf.len = g_com_param.len;
            g_com_param.len = 0;            

            break;
        }
    }
}

void on_rf_send(void)
{  
    rf_send_data(g_com_param.ext.rf.data, g_com_param.ext.rf.len);
}

void on_key(void)
{
    u32 hop_code;
    u8 buf[CHIP_ID_LEN];


    hop_code  = get_hop_code(); //获取跳码
    get_chip_id(buf); //获取芯片ID

    LEAD_CODE = 'K'; //前导码

    CHIP_ID_0 = buf[0]; //芯片ID
    CHIP_ID_1 = buf[1];
    CHIP_ID_2 = buf[2];
    CHIP_ID_3 = buf[3];

    LVD       = g_key_param.lvd; //低电压指示

    KEY_CODE  = g_key_param.msg; //键码
    g_key_param.msg = KEY_MSG_UNKNOW; //清除按键信息

    SYNC_0    = hop_code >> 0; //跳码
    SYNC_1    = hop_code >> 8;
    SYNC_2    = hop_code >> 16;
    SYNC_3    = hop_code >> 24;

    RESERVE_0 = 0x00; //保留项
    RESERVE_1 = 0x00;
    RESERVE_2 = 0x00;
    RESERVE_3 = 0x00;
    
    KEY_CRC   = 0x00;
    for(u8 i=0; i<(KEY_FRAME_LEN - 1); i++) 
    {
        KEY_CRC += g_key_param.buf[i];
    }

    AES_Encrypt(g_key_param.buf, (u8 *)g_AesExpKey, g_com_param.buf);

    g_com_param.len = KEY_FRAME_LEN;

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

#if (KEY_DEBUG_EN > 0U)
    g_key_param.press_key_times++;
#endif
}

void on_user(void)
{       
    if('h' == HEAD)
    {
        u8 pwd[PWD_LEN];


        get_pwd(pwd); //获取用户密码
        
        if(pwd[0] == PWD_0 &&
           pwd[1] == PWD_1 &&
           pwd[2] == PWD_2) //普通用户密码
        {
            switch(CMD)
            {
                case 'D': //写芯片ID
                {
                    u8 buf_id[CHIP_ID_LEN];
                    u8 ret1, ret2;
                    

                    /* 查看芯片ID是否已经写过 */
                    get_chip_id(buf_id);

                    if('H' != buf_id[0] ||
                       'R' != buf_id[1] ||
                       'K' != buf_id[2] ||
                       ' ' != buf_id[3])
                    {
                        DEBUG_PRINT(("Chip ID has been set!\n"));
                        break;
                    } 

                    /* 第一次写芯片ID */
                    buf_id[0] = PARAM_0;
                    buf_id[1] = PARAM_1;
                    buf_id[2] = PARAM_2;
                    buf_id[3] = PARAM_3;

                    flash_erase_block(FLASH_BLOCK_A);

                    ret1 = set_pwd(pwd);
                    ret2 = set_chip_id(buf_id);
                    
                    if((NORMAL == ret1) && (NORMAL == ret2))
                    {
                        DEBUG_PRINT(("Chip ID set successfully!\n"));
                    }
                    else
                    {
                        DEBUG_PRINT(("Failed to set the chip ID!\n"));
                    }
                    break;
                }

                case 'd': //读芯片ID
                {
                    u8 buf[CHIP_ID_LEN];
                    
                    
                    get_chip_id(buf);
                    DEBUG_PRINT(("Chip ID: %02x %02x %02x %02x\n", buf[0], buf[1], buf[2], buf[3]));
                    break;
                }

                case 'P': //设置用户密码
                {
                    if(pwd[3] != PWD_3) //超级用户密码
                    {
                        DEBUG_PRINT(("Wrong password!\n"));
                        break;
                    }

                    u8 buf_id[CHIP_ID_LEN];
                    u8 buf_pwd[PWD_LEN];
                    u8 ret1, ret2;


                    buf_pwd[0] = PARAM_0;
                    buf_pwd[1] = PARAM_1;
                    buf_pwd[2] = PARAM_2;
                    buf_pwd[3] = PARAM_3;

                    get_chip_id(buf_id);
                    
                    flash_erase_block(FLASH_BLOCK_A);
                    
                    ret1 = set_pwd(buf_pwd);
                    ret2 = set_chip_id(buf_id);

                    if((NORMAL == ret1) && (NORMAL == ret2))
                    {
                        DEBUG_PRINT(("User password set successfully!\n"));
                    }
                    else
                    {
                        DEBUG_PRINT(("Failed to set the user password!\n"));
                    }
                    break;
                }

                case 'v': //读软件版本
                {
                    DEBUG_PRINT(("Version: %s\n", VERSION));
                    break;
                }

                case 't': //读系统工作时间
                {
                    DEBUG_PRINT(("System operating time: %lu (s)\n", _sys_sec));
                    break;
                }

                default:
                {
                    DEBUG_PRINT(("Bad command!\n")); 
                    break;
                }
            } //end of switch
        } 
        else if(' ' == PWD_0 &&
                'H' == PWD_1 &&
                'R' == PWD_2 &&
                'K' == PWD_3) //恢复出厂设置
        {
            u8 buf_id[CHIP_ID_LEN] = {'H', 'R', 'K', ' '};
            u8 buf_pwd[PWD_LEN] = {'1', '2', '3', 's'};
            u8 buf_hope_code[HOPE_CODE_LEN] = {0, 0, 0, 0};
            u8 ret1, ret2, ret3;


            DEBUG_PRINT(("Erasing data flash ...\n"));
            flash_write_init(); //擦除数据闪存

            ret1 = set_chip_id(buf_id); //恢复默认芯片ID
            ret2 = set_pwd(buf_pwd); //恢复默认密码
            ret3 = set_hope_code(buf_hope_code); //初始化跳码

            if((NORMAL == ret1) && (NORMAL == ret2) && (NORMAL == ret3))
            {
                DEBUG_PRINT(("Restore factory defaults!\n"));
            }
            else
            {
                DEBUG_PRINT(("Failed to restore factory defaults!\n"));
            }        
        }
        else
        {
            DEBUG_PRINT(("Wrong password!\n"));
        } //end of if(PWD)
    } 
    else
    {
        DEBUG_PRINT(("Wrong SD!\n"));
    } //end of if(HEAD)
}

