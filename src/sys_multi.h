#ifndef __SYS_MULTI_H__
#define __SYS_MULTI_H__

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
*                                    MACRO                                    *
******************************************************************************/

#define TRUE                         1
#define FALSE                        0

#define NOP()          __no_operation()
#define EI()        _enable_interrupt()
#define DI()       _disable_interrupt()
#define NO_INIT              __no_init
#define ROOT                    __root
#define INTERRUPT          __interrupt

#define LOW_POWER_TEST_EN           0U //�͹��Ĳ���ʹ��
#define VARIABLES_MEMSET_EN         0U //��������ʹ��

#define CLR_WDT()           clr_wdt() //ι��

#define VCC_CTRL_PORT_INIT()       (pd1_0 = 1) //ϵͳ��Դ���ƹܽų�ʼ��
#define VCC_ON()                   (p1_0 = 1 ) //��ϵͳ��Դ
#define VCC_OFF()                  (p1_0 = 0 ) //�ر�ϵͳ��Դ

#define VDD_CTRL_PORT_INIT()       (pd1_7 = 1) //��Ƶ��Դ���ƹܽų�ʼ��
#define VDD_ON()                   (p1_7 = 1 ) //����Ƶ��Դ
#define VDD_OFF()                  (p1_7 = 0 ) //�ر���Ƶ��Դ

#define SHORT1_CTRL_PORT_INIT()    (pd3_4 = 1) //�̽ӵ�1���ƹܽų�ʼ��
#define SHORT1_HIGH()              (p3_4 = 1 ) //�̽ӵ�1�����
#define SHORT1_LOW()               (p3_4 = 0 ) //�̽ӵ�2�����
#define SHORT2_CTRL_PORT_INIT(val) (pd1_6 = val) //�̽ӵ�2���ƹܽų�ʼ����0���룬1���
#define SHORT2()                   (p1_6     ) //�̽ӵ�2����״̬

#define SHUTDOWN()                   shutdown() //ϵͳ���ҹر�

#define POWER_CTRL_CHECK_TIME      (1000UL * 5) //5s

#define DEFAULT_BUF_SIZE            64 //Ĭ�ϻ�������С

#define MAX_TEST_DATA_LEN           58 //�������԰������ݳ���

#define SYS_TICK_PERIOD             (1000UL / SYS_TICKS_PER_SEC) //ϵͳ�������ڣ�ms

#define CHECK_TIME_255B_2400     900UL //2400bps��255�ֽڴ��䳬ʱʱ�䣬��λms
#define CHECK_TIME_255B_4800     500UL //4800bps��255�ֽڴ��䳬ʱʱ�䣬��λms
#define CHECK_TIME_255B_9600     300UL //9600bps��255�ֽڴ��䳬ʱʱ�䣬��λms
#define CHECK_TIME_255B_19200    200UL //19200bps��255�ֽڴ��䳬ʱʱ�䣬��λms
#define CHECK_TIME_255B_312500    10UL //312500bps��255�ֽڴ��䳬ʱʱ�䣬��λms
#define MAX_SEND_PKT_CHECK_TIME      CHECK_TIME_255B_4800

#define VERSION                      "wireless-remote-control_2013.10.23"

/******************************************************************************
*                                  DATA TYPE                                  *
******************************************************************************/

typedef unsigned char bool;
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed long s32;
typedef unsigned long u32;
typedef unsigned long tick;

/******************************************************************************
*                                    COM                                      *
******************************************************************************/

typedef union __EXT {               //��Ƶ�����ڷ��������Լ��û����õ�������
    struct _rf {
        u8 *data;
        u8 len;
    } rf;

    struct _uart {
        u8 *data;
        u8 len;
    } uart;

    struct _user {
        u8 *buf;
        u8 len;    
    } user;
} _EXT;

#define MAX_COM_BUF_LEN              DEFAULT_BUF_SIZE //������������󳤶�

typedef struct _COM_PARAM {
    volatile u8 len;                //���������������ֽ�
    u8 buf[MAX_COM_BUF_LEN];        //����������

    _EXT ext;                       //��Ƶ�����ڷ��������Լ��û����õ�������
} COM_PARAM;

/******************************************************************************
*                                    USER                                     *
******************************************************************************/

#define HEAD                      g_com_param.ext.user.buf[0] //ͷ
#define CMD                       g_com_param.ext.user.buf[1] //����
#define PARAM_0                   g_com_param.ext.user.buf[2] //����
#define PARAM_1                   g_com_param.ext.user.buf[3]
#define PARAM_2                   g_com_param.ext.user.buf[4]
#define PARAM_3                   g_com_param.ext.user.buf[5]  
#define PWD_0                     g_com_param.ext.user.buf[6]
#define PWD_1                     g_com_param.ext.user.buf[7]
#define PWD_2                     g_com_param.ext.user.buf[8] //��ͨ�û�����
#define PWD_3                     g_com_param.ext.user.buf[9] //�����û�����

/******************************************************************************
*                                   GLOBAL                                    *
******************************************************************************/

extern COM_PARAM g_com_param; //��������

/******************************************************************************
*                                  PROTOTYPE                                  *
******************************************************************************/

void _variable_init(void); //��ʼ��ȫ�ֱ���
void _device_init(void); //�����ʼ��
void power_off(void); //�رյ�Դ
void config_port(void); //����ǰ���ùܽ�
void config_intp(void); //����ǰ�����ж�
void stop_mode(void); //ֹͣģʽ
void wait_mode(void); //�ȴ�ģʽ
void soft_reset(void); //��λ
void shutdown(void); //ϵͳ���ҹر�
void chk_sys_sta(void); //���ϵͳ״̬

/******************************************************************************
*                                     END                                     *
******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif
