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

#define LOW_POWER_TEST_EN           0U //低功耗测试使能
#define VARIABLES_MEMSET_EN         0U //变量清零使能

#define CLR_WDT()           clr_wdt() //喂狗

#define VCC_CTRL_PORT_INIT()       (pd1_0 = 1) //系统电源控制管脚初始化
#define VCC_ON()                   (p1_0 = 1 ) //打开系统电源
#define VCC_OFF()                  (p1_0 = 0 ) //关闭系统电源

#define VDD_CTRL_PORT_INIT()       (pd1_7 = 1) //射频电源控制管脚初始化
#define VDD_ON()                   (p1_7 = 1 ) //打开射频电源
#define VDD_OFF()                  (p1_7 = 0 ) //关闭射频电源

#define SHORT1_CTRL_PORT_INIT()    (pd3_4 = 1) //短接点1控制管脚初始化
#define SHORT1_HIGH()              (p3_4 = 1 ) //短接点1输出高
#define SHORT1_LOW()               (p3_4 = 0 ) //短接点2输出低
#define SHORT2_CTRL_PORT_INIT(val) (pd1_6 = val) //短接点2控制管脚初始化，0输入，1输出
#define SHORT2()                   (p1_6     ) //短接点2输入状态

#define SHUTDOWN()                   shutdown() //系统自我关闭

#define POWER_CTRL_CHECK_TIME      (1000UL * 5) //5s

#define DEFAULT_BUF_SIZE            64 //默认缓冲区大小

#define MAX_TEST_DATA_LEN           58 //发数测试包的数据长度

#define SYS_TICK_PERIOD             (1000UL / SYS_TICKS_PER_SEC) //系统节拍周期，ms

#define CHECK_TIME_255B_2400     900UL //2400bps，255字节传输超时时间，单位ms
#define CHECK_TIME_255B_4800     500UL //4800bps，255字节传输超时时间，单位ms
#define CHECK_TIME_255B_9600     300UL //9600bps，255字节传输超时时间，单位ms
#define CHECK_TIME_255B_19200    200UL //19200bps，255字节传输超时时间，单位ms
#define CHECK_TIME_255B_312500    10UL //312500bps，255字节传输超时时间，单位ms
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

typedef union __EXT {               //射频、串口发送数据以及用户配置的联合体
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

#define MAX_COM_BUF_LEN              DEFAULT_BUF_SIZE //公共缓冲区最大长度

typedef struct _COM_PARAM {
    volatile u8 len;                //公共缓冲区数据字节
    u8 buf[MAX_COM_BUF_LEN];        //公共缓冲区

    _EXT ext;                       //射频、串口发送数据以及用户配置的联合体
} COM_PARAM;

/******************************************************************************
*                                    USER                                     *
******************************************************************************/

#define HEAD                      g_com_param.ext.user.buf[0] //头
#define CMD                       g_com_param.ext.user.buf[1] //命令
#define PARAM_0                   g_com_param.ext.user.buf[2] //参数
#define PARAM_1                   g_com_param.ext.user.buf[3]
#define PARAM_2                   g_com_param.ext.user.buf[4]
#define PARAM_3                   g_com_param.ext.user.buf[5]  
#define PWD_0                     g_com_param.ext.user.buf[6]
#define PWD_1                     g_com_param.ext.user.buf[7]
#define PWD_2                     g_com_param.ext.user.buf[8] //普通用户密码
#define PWD_3                     g_com_param.ext.user.buf[9] //超级用户密码

/******************************************************************************
*                                   GLOBAL                                    *
******************************************************************************/

extern COM_PARAM g_com_param; //公共参数

/******************************************************************************
*                                  PROTOTYPE                                  *
******************************************************************************/

void _variable_init(void); //初始化全局变量
void _device_init(void); //外设初始化
void power_off(void); //关闭电源
void config_port(void); //待机前配置管脚
void config_intp(void); //待机前配置中断
void stop_mode(void); //停止模式
void wait_mode(void); //等待模式
void soft_reset(void); //软复位
void shutdown(void); //系统自我关闭
void chk_sys_sta(void); //检测系统状态

/******************************************************************************
*                                     END                                     *
******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif
