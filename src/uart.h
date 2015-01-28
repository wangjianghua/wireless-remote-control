#ifndef __UART_H__
#define __UART_H__

#ifdef __cplusplus
extern "C" {
#endif


#define UART_DEBUG_EN               0U //���ڵ���ʹ��
#define DEBUG_INFO_PRINT_EN         1U //���Դ�ӡʹ��

#if (DEBUG_INFO_PRINT_EN > 0U)
#define DEBUG_PRINT(s)        printf s
#else
#define DEBUG_PRINT(s)             
#endif

#define WARN(s)

#define MAX_UART_RECV_BUF_LEN       (24) //���ڽ��ջ���������
#define UART_RX_AF_THRESHOLD      0xfa //���ڽ�������
#define UART_TRANSMIT_TIMEOUT       (40 / SYS_TICK_PERIOD) //���ڷ��͡�����һ�ֽڵĳ�ʱʱ��
#define MAX_RECV_PKT_CHECK_TIME     (1000UL * 60) //60s

typedef struct _uart_param {
    bool shorted;                   //�̽ӱ�־λ

    struct __tx {                   //�������ݽṹ
        u8 *addr;                   //���ͻ�������ַ
        u16 bytes;                  //�����ֽ���
        bool completed;             //������ɱ�־
        tick check_time;            //255�ֽڷ��ͳ�ʱʱ��
    } tx;
    
    struct __rx {
        u8 buf[MAX_UART_RECV_BUF_LEN]; //���ջ�����
        u8 * volatile write_index;  //д����
        u8 * volatile read_index;   //������
        volatile u16 bytes;         //�������ִ��ֽ���
        u8 timeout;                 //�����ֽڵĳ�ʱʱ��
        tick start_time;            //��ʼ����ʱ��
        bool recv;                  //�������ݱ�־
    } rx;
    
#if (UART_DEBUG_EN > 0U)
    bool recv_pkt_overflow;         //���հ������־
    u8 err_type;                    //���ڴ����ж�����
    u16 err_intp_times;             //���ڴ����жϴ���
    u16 send_timeout_times;         //���ͳ�ʱ����
#endif
} UART_PARAM;

extern UART_PARAM g_uart_param; //���ڲ���

void init_uart(void); //��ʼ������
u8 uart_send_data(u8 *tx_buf, u16 tx_num); //���ڷ�������
u16 get_bytes(void); //��ȡ���ڽ��ջ����������ֽ�
u8 get_char(void); //�Ӵ��ڽ��ջ�������ȡ�����ֽڣ��ٽ紦��
void uart_send_end_callback(void); //���ڷ��ͽ����ص�����


#ifdef __cplusplus
}
#endif

#endif
