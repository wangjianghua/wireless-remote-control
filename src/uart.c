#include "includes.h"


UART_PARAM g_uart_param;

void init_uart(void)
{
    /* uarti发送/接收模式寄存器 */        
    ckdir_u0mr = 0; //时钟源选择(0内部时钟源，1外部时钟源)
    smd0_u0mr = 1; //uart模式传送数据长8位
    smd1_u0mr = 0;
    smd2_u0mr = 1;
    stps_u0mr = 0; //1位停止位
    prye_u0mr = 0; //无校验位    

    /* uarti发送/接收控制寄存器0 */
    uform_u0c0 = 0; //LSB先
    clk0_u0c0 = 0; //内部时钟源分频，f1
    clk1_u0c0 = 0;
    
    /* uibrg = f / (位速率 * 16) - 1 */
    //u0brg = 0x40; //f = 10MHz，baud = 9600bps
    u0brg = 0x33; //f = 8MHz，baud = 9600bps

    /* 中断控制寄存器 */
    ir_s0tic = 0; //清发送中断标志位
    ilvl_s0tic = 2; //uart0发送中断优先级
    ir_s0ric = 0; //清接收中断标志位
    ilvl_s0ric = 2; //uart0接收中断优先级

    /* uarti发送/接收控制寄存器2 */
    u0irs = 0; //发送结束(TXEPT=1)
    u0rrm = 0; 
    
    pd1_4 = 1; //IO口设置为输出，TXD0
    pd1_5 = 0; //IO口设置为输入，RXD0
    //cntrsel = 0; //p1_5/RXD0

    /* uarti发送/接收控制寄存器1 */
    ti_u0c1 = 1; //发送无数据
    te_u0c1 = 1; //允许发送
    ri_u0c1 = 0; //接收无数据
    re_u0c1 = 1; //允许接收
}

u8 uart_send_data(u8 *tx_buf, u16 tx_num)
{
	u8 status = 0;
    tick start_time;
    
    
    start_time = get_tick();
    
    while(g_uart_param.tx.bytes)
    {
        CLR_WDT();

        if(check_timeout(start_time, g_uart_param.tx.check_time)) //检测串口发送超时
        {
#if (UART_DEBUG_EN > 0U)
            g_uart_param.send_timeout_times++;
#endif
            
            init_uart();
            
            break;
        }
    }

	if (tx_num < 1)
	{
		status = 1;
	}
	else
	{
	    g_uart_param.tx.completed = FALSE;
		g_uart_param.tx.addr = tx_buf;
		g_uart_param.tx.bytes = tx_num;
        ilvl_s0tic = 0; //disable uart0 tx interrupt
		u0tbl = *g_uart_param.tx.addr;
		g_uart_param.tx.addr++;
		g_uart_param.tx.bytes--;
        ilvl_s0tic = 2; //enable uart0 tx interrupt
	}
  
	return (status);
}

u16 get_bytes(void)
{
    return (g_uart_param.rx.bytes);
}

u8 get_char(void)
{
    u8 ch;


    ch = *g_uart_param.rx.read_index;
    g_uart_param.rx.read_index++;

    if(g_uart_param.rx.read_index >= (g_uart_param.rx.buf + MAX_UART_RECV_BUF_LEN))
    {
        g_uart_param.rx.read_index = g_uart_param.rx.buf;
    }

    DI();
    g_uart_param.rx.bytes--;
    EI();

    return (ch);
}

void uart_send_end_callback(void)
{
    g_uart_param.tx.completed = TRUE;
}

#if (DEBUG_INFO_PRINT_EN > 0U)
int putchar(int ch)
{
    u8 temp;
    tick start_time;


    temp = (u8)ch;
    uart_send_data(&temp, 1);

    start_time = get_tick();
        
    while(FALSE == g_uart_param.tx.completed)
    {
        CLR_WDT();
    
        if(check_timeout(start_time, g_uart_param.tx.check_time))
        {
            init_uart();

            break;
        }
    }

    return (ch);
}
#endif

