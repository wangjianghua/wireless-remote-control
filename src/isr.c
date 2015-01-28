#include "includes.h"


#pragma vector = TMRX
INTERRUPT void timerX_isr(void)
{
    g_app_tick++; //1ms	
}

#pragma vector = TMRZ
INTERRUPT void timerZ_isr(void)
{
    SYS_EVT_ADD(SYS_EVT_TICK); //20ms	
}

#pragma vector = INT0
INTERRUPT void int0_isr(void)
{
    sys_evt_add(SYS_EVT_RF_INTP);

#if (RF_DEBUG_EN > 0U)
    g_rf_param.intp_times++;
#endif		
} 

#pragma vector = KEY
INTERRUPT void key_isr(void)
{
    for(u8 i=0; i<3; i++) //系统从待机模式唤醒以后，尝试三次软复位
    {
        soft_reset();

        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
    }

    SHUTDOWN(); //软复位失败，系统自我关闭
}

#pragma vector = UART0_TX
INTERRUPT void uart0_tx_isr(void)
{
    if(g_uart_param.tx.bytes > 0)
    {
    	u0tbl = *g_uart_param.tx.addr;
    	g_uart_param.tx.addr++;
    	g_uart_param.tx.bytes--;
    }
    else
    {
        uart_send_end_callback();
    }			
}

#pragma vector = UART0_RX
INTERRUPT void uart0_rx_isr(void)
{
    u8 rx_data;


    rx_data = u0rb;

    if(MAX_UART_RECV_BUF_LEN == g_uart_param.rx.bytes) //缓冲区已满，使缓冲区保持最新数据
    {
#if (UART_DEBUG_EN > 0U)
        if(!g_uart_param.recv_pkt_overflow)
        {
            g_uart_param.recv_pkt_overflow = 1; //数据溢出，置缓冲区溢出标志
        }
#endif

        *g_uart_param.rx.write_index = rx_data;
        g_uart_param.rx.write_index++;
        
        g_uart_param.rx.read_index++;

        if(g_uart_param.rx.write_index >= (g_uart_param.rx.buf + MAX_UART_RECV_BUF_LEN))
        {
            g_uart_param.rx.write_index = g_uart_param.rx.buf;
        }

        if(g_uart_param.rx.read_index >= (g_uart_param.rx.buf + MAX_UART_RECV_BUF_LEN))
        {
            g_uart_param.rx.read_index = g_uart_param.rx.buf;
        }
    }
    else //缓冲区未满
    {
        *g_uart_param.rx.write_index = rx_data;
        g_uart_param.rx.write_index++;

        if(g_uart_param.rx.write_index >= (g_uart_param.rx.buf + MAX_UART_RECV_BUF_LEN))
        {
            g_uart_param.rx.write_index = g_uart_param.rx.buf;
        }

        g_uart_param.rx.bytes++;
    }		
}

INTERRUPT void __watchdog_timer_handler(void)
{
    if(vw2c2) //电压监视2
    {
        if(vca13 == 1) //VCC>=Vdet2或电压检测电路2无效
        {
            g_key_param.lvd = 0;
        }
        else //VCC<Vdet2
        {
            g_key_param.lvd = 1;
        }
    }

    if(vw2c3) //监视定时器
    {

    }

    if(ocd3) //振荡停止检测
    {
        
    }
}

