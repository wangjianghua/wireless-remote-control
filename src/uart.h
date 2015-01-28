#ifndef __UART_H__
#define __UART_H__

#ifdef __cplusplus
extern "C" {
#endif


#define UART_DEBUG_EN               0U //串口调试使能
#define DEBUG_INFO_PRINT_EN         1U //调试打印使能

#if (DEBUG_INFO_PRINT_EN > 0U)
#define DEBUG_PRINT(s)        printf s
#else
#define DEBUG_PRINT(s)             
#endif

#define WARN(s)

#define MAX_UART_RECV_BUF_LEN       (24) //串口接收缓冲区长度
#define UART_RX_AF_THRESHOLD      0xfa //串口接收门限
#define UART_TRANSMIT_TIMEOUT       (40 / SYS_TICK_PERIOD) //串口发送、接收一字节的超时时间
#define MAX_RECV_PKT_CHECK_TIME     (1000UL * 60) //60s

typedef struct _uart_param {
    bool shorted;                   //短接标志位

    struct __tx {                   //发送数据结构
        u8 *addr;                   //发送缓冲区地址
        u16 bytes;                  //发送字节数
        bool completed;             //发送完成标志
        tick check_time;            //255字节发送超时时间
    } tx;
    
    struct __rx {
        u8 buf[MAX_UART_RECV_BUF_LEN]; //接收缓冲区
        u8 * volatile write_index;  //写索引
        u8 * volatile read_index;   //读索引
        volatile u16 bytes;         //缓冲区现存字节数
        u8 timeout;                 //数据字节的超时时间
        tick start_time;            //开始接收时间
        bool recv;                  //接收数据标志
    } rx;
    
#if (UART_DEBUG_EN > 0U)
    bool recv_pkt_overflow;         //接收包溢出标志
    u8 err_type;                    //串口错误中断类型
    u16 err_intp_times;             //串口错误中断次数
    u16 send_timeout_times;         //发送超时次数
#endif
} UART_PARAM;

extern UART_PARAM g_uart_param; //串口参数

void init_uart(void); //初始化串口
u8 uart_send_data(u8 *tx_buf, u16 tx_num); //串口发送数据
u16 get_bytes(void); //获取串口接收缓冲区数据字节
u8 get_char(void); //从串口接收缓冲区获取数据字节，临界处理
void uart_send_end_callback(void); //串口发送结束回调函数


#ifdef __cplusplus
}
#endif

#endif
