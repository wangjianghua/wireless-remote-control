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

    if((TRUE == g_uart_param.shorted) && (FALSE == g_key_param.press)) //����ж̽ӣ���û�а���
    {
        if(get_bytes())
        {
            g_uart_param.rx.recv = TRUE;
            sys_evt_add(SYS_EVT_UART_RECV);
        }
        else if((FALSE == g_uart_param.rx.recv) && check_timeout(g_uart_param.rx.start_time, MAX_RECV_PKT_CHECK_TIME))
        {
            SHUTDOWN(); //60���ڴ��ڽ��������ݣ�ϵͳ���ҹر�
        }
    }

    if(g_uart_param.rx.timeout) //�����ڽ��ճ�ʱ
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
    
    if(g_rf_param.tx.timeout) //������Ƶ���ͳ�ʱ
    {
        g_rf_param.tx.timeout--;
        
        if(!g_rf_param.tx.timeout)
        {
#if (RF_AUTO_SEND_EN > 0U)
            g_rf_param.tx.complete = TRUE; //���ͳ�ʱ��ǿ�ƽ���
            
            TX_LED_OFF();

            rf_reset_chip();
            
#if (RF_DEBUG_EN > 0U)
            g_rf_param.send_timeout_times++; //���ͳ�ʱ����
            g_rf_param.send_pkt_num++; //���Ͱ���Ŀ
            g_rf_param.reset_chip_times++;
#endif

#else
            SHUTDOWN(); //�쳣��ϵͳ���ҹر�
#endif
        }
    }

#if 0
    if(g_rf_param.rx.timeout) //������Ƶ���ճ�ʱ
    {
        g_rf_param.rx.timeout--;
        
        if(!g_rf_param.rx.timeout)
        {
            RX_LED_OFF();

            rf_reset_chip();
            
#if (RF_DEBUG_EN > 0U)
            g_rf_param.recv_timeout_times++; //���ճ�ʱ����
            g_rf_param.recv_pkt_num++; //���հ���Ŀ
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


    rf_intp_sta1 = rf_read_reg(INTP_STA1_ADDR); //��ȡ��Ƶ�ж�״̬
    rf_read_reg(INTP_STA2_ADDR); //��intp_sta2�ж�
    
	if(FF_ERROR == (rf_intp_sta1 & FF_ERROR)) 
	{//FIFO��������
#if (RF_AUTO_SEND_EN > 0U)
        rf_reset_chip();
        
#if (RF_DEBUG_EN > 0U)
	    g_rf_param.ff_error_times++;
        g_rf_param.reset_chip_times++;
#endif

#else
        SHUTDOWN(); //�쳣��ϵͳ���ҹر�
#endif
	}    
    
#if 0
    else if(RXFF_AF == (rf_intp_sta1 & RXFF_AF)) 
    {//����FIFO�����ж�
        if(RF_STATE_RX != g_rf_param.state)
        {
            RX_LED_ON();
            g_rf_param.state = RF_STATE_RX; //��Ƶת������̬
            g_rf_param.rx.pkt_len = rf_read_reg(RXPKT_LEN_ADDR); //��ȡ���ݰ�����

            rf_rssi_monitor(); //�����ź�ǿ��
            rf_fdev_monitor(); //���ڽ����жϺ��ʣ����ڽ��չ�����

            if(g_rf_param.rx.pkt_len >= MAX_RF_RECV_BUF_LEN) //���ƽ��հ��������������˰�
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
            
            g_rf_param.rx.index = 0; //��λ������������
            g_rf_param.rx.timeout = RF_TRANSMIT_TIMEOUT; //���ý��ճ�ʱʱ��
        }
               
        rf_read_fifo(RF_RX_AF_THRESHOLD); 
    }
    else if(RXPKT_DONE == (rf_intp_sta1 & RXPKT_DONE))
    {//����FIFO����ж�
        if(RF_STATE_RX != g_rf_param.state)
        {
            RX_LED_ON();
            g_rf_param.state = RF_STATE_RX; //��Ƶת������̬
            g_rf_param.rx.pkt_len = rf_read_reg(RXPKT_LEN_ADDR); //��ȡ���ݰ�����

            rf_rssi_monitor(); //�����ź�ǿ��

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
            
            g_rf_param.rx.index = 0; //��λ������������
            g_rf_param.rx.timeout = RF_TRANSMIT_TIMEOUT; //���ý��ճ�ʱʱ��
        }

        u8 remain_bytes;
        
        
        remain_bytes = g_rf_param.rx.pkt_len - g_rf_param.rx.index; //��Ƶʣ���ֽ���
        
        rf_read_fifo(remain_bytes);
        
#if (RF_DEBUG_EN > 0U)
        if(g_rf_param.rx.index != g_rf_param.rx.pkt_len) //���յ����ݲ�����
        {
            g_rf_param.recv_pkt_lost_times++;  
            
            DEBUG_PRINT(("<rf_recv_lost>"));
        }

        if(CRC_ERROR == (rf_intp_sta1 & CRC_ERROR)) //Ӳ��crcУ�����
        {
            g_rf_param.crc_error_times++;

            DEBUG_PRINT(("<rf_crc_error>"));
        }

        g_rf_param.recv_pkt_num++; //���հ���Ŀ
#endif 

        g_com_param.ext.uart.data = g_rf_param.rx.buf;
        g_com_param.ext.uart.len = g_rf_param.rx.index;

        sys_evt_add(SYS_EVT_UART_SEND);
 
        g_rf_param.rx.timeout = 0; //�峬ʱ������

		rf_state_idle();
		rf_state_listen();
        
        RX_LED_OFF();
    }
#endif

    else if(TXFF_AF == (rf_intp_sta1 & TXFF_AF))
    {//����FIFO�����ж�
        rf_state_tx(); //fifo������������Ƶ��������
    }
    else if(TXPKT_DONE == (rf_intp_sta1 & TXPKT_DONE))
    {//����FIFO����ж�
#if (RF_AUTO_SEND_EN > 0U)

#if (RF_DEBUG_EN > 0U)
        g_rf_param.send_pkt_num++; //���Ͱ���Ŀ
#endif

        g_rf_param.tx.timeout = 0; //�峬ʱ������
        g_rf_param.tx.complete = TRUE; //�������

        rf_state_idle();
        rf_state_listen();
        
        TX_LED_OFF();
#else
        SHUTDOWN(); //������ɣ�ϵͳ���ҹر�
#endif
    }
    else if(TXFF_AE == (rf_intp_sta1 & TXFF_AE))
    {//����FIFO�����ж�
        rf_write_fifo();
    }
}

void on_uart_recv(void)
{
    u8 ch;


    while(get_bytes()) //����һ�߽������ݣ��¼�һ�ߴ�����յ����ݣ���Ƶ���в�����Ϊ19200
    {
        CLR_WDT();
        
        ch = get_char();
        g_uart_param.rx.timeout = UART_TRANSMIT_TIMEOUT;
        
        g_com_param.buf[g_com_param.len++] = ch;

        if(g_com_param.len >= UART_RX_AF_THRESHOLD) //�ﵽ���ڽ������ޣ�ת����������
        {
            g_uart_param.rx.timeout = 0; //������������������

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


    hop_code  = get_hop_code(); //��ȡ����
    get_chip_id(buf); //��ȡоƬID

    LEAD_CODE = 'K'; //ǰ����

    CHIP_ID_0 = buf[0]; //оƬID
    CHIP_ID_1 = buf[1];
    CHIP_ID_2 = buf[2];
    CHIP_ID_3 = buf[3];

    LVD       = g_key_param.lvd; //�͵�ѹָʾ

    KEY_CODE  = g_key_param.msg; //����
    g_key_param.msg = KEY_MSG_UNKNOW; //���������Ϣ

    SYNC_0    = hop_code >> 0; //����
    SYNC_1    = hop_code >> 8;
    SYNC_2    = hop_code >> 16;
    SYNC_3    = hop_code >> 24;

    RESERVE_0 = 0x00; //������
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
    VDD_ON(); //����Ƶ��Դ

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


        get_pwd(pwd); //��ȡ�û�����
        
        if(pwd[0] == PWD_0 &&
           pwd[1] == PWD_1 &&
           pwd[2] == PWD_2) //��ͨ�û�����
        {
            switch(CMD)
            {
                case 'D': //доƬID
                {
                    u8 buf_id[CHIP_ID_LEN];
                    u8 ret1, ret2;
                    

                    /* �鿴оƬID�Ƿ��Ѿ�д�� */
                    get_chip_id(buf_id);

                    if('H' != buf_id[0] ||
                       'R' != buf_id[1] ||
                       'K' != buf_id[2] ||
                       ' ' != buf_id[3])
                    {
                        DEBUG_PRINT(("Chip ID has been set!\n"));
                        break;
                    } 

                    /* ��һ��доƬID */
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

                case 'd': //��оƬID
                {
                    u8 buf[CHIP_ID_LEN];
                    
                    
                    get_chip_id(buf);
                    DEBUG_PRINT(("Chip ID: %02x %02x %02x %02x\n", buf[0], buf[1], buf[2], buf[3]));
                    break;
                }

                case 'P': //�����û�����
                {
                    if(pwd[3] != PWD_3) //�����û�����
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

                case 'v': //������汾
                {
                    DEBUG_PRINT(("Version: %s\n", VERSION));
                    break;
                }

                case 't': //��ϵͳ����ʱ��
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
                'K' == PWD_3) //�ָ���������
        {
            u8 buf_id[CHIP_ID_LEN] = {'H', 'R', 'K', ' '};
            u8 buf_pwd[PWD_LEN] = {'1', '2', '3', 's'};
            u8 buf_hope_code[HOPE_CODE_LEN] = {0, 0, 0, 0};
            u8 ret1, ret2, ret3;


            DEBUG_PRINT(("Erasing data flash ...\n"));
            flash_write_init(); //������������

            ret1 = set_chip_id(buf_id); //�ָ�Ĭ��оƬID
            ret2 = set_pwd(buf_pwd); //�ָ�Ĭ������
            ret3 = set_hope_code(buf_hope_code); //��ʼ������

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

