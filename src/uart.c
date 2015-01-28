#include "includes.h"


UART_PARAM g_uart_param;

void init_uart(void)
{
    /* uarti����/����ģʽ�Ĵ��� */        
    ckdir_u0mr = 0; //ʱ��Դѡ��(0�ڲ�ʱ��Դ��1�ⲿʱ��Դ)
    smd0_u0mr = 1; //uartģʽ�������ݳ�8λ
    smd1_u0mr = 0;
    smd2_u0mr = 1;
    stps_u0mr = 0; //1λֹͣλ
    prye_u0mr = 0; //��У��λ    

    /* uarti����/���տ��ƼĴ���0 */
    uform_u0c0 = 0; //LSB��
    clk0_u0c0 = 0; //�ڲ�ʱ��Դ��Ƶ��f1
    clk1_u0c0 = 0;
    
    /* uibrg = f / (λ���� * 16) - 1 */
    //u0brg = 0x40; //f = 10MHz��baud = 9600bps
    u0brg = 0x33; //f = 8MHz��baud = 9600bps

    /* �жϿ��ƼĴ��� */
    ir_s0tic = 0; //�巢���жϱ�־λ
    ilvl_s0tic = 2; //uart0�����ж����ȼ�
    ir_s0ric = 0; //������жϱ�־λ
    ilvl_s0ric = 2; //uart0�����ж����ȼ�

    /* uarti����/���տ��ƼĴ���2 */
    u0irs = 0; //���ͽ���(TXEPT=1)
    u0rrm = 0; 
    
    pd1_4 = 1; //IO������Ϊ�����TXD0
    pd1_5 = 0; //IO������Ϊ���룬RXD0
    //cntrsel = 0; //p1_5/RXD0

    /* uarti����/���տ��ƼĴ���1 */
    ti_u0c1 = 1; //����������
    te_u0c1 = 1; //������
    ri_u0c1 = 0; //����������
    re_u0c1 = 1; //�������
}

u8 uart_send_data(u8 *tx_buf, u16 tx_num)
{
	u8 status = 0;
    tick start_time;
    
    
    start_time = get_tick();
    
    while(g_uart_param.tx.bytes)
    {
        CLR_WDT();

        if(check_timeout(start_time, g_uart_param.tx.check_time)) //��⴮�ڷ��ͳ�ʱ
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

