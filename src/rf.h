#ifndef __RF_H__
#define __RF_H__

#ifdef __cplusplus
extern "C" {
#endif


/* REGISTER ADDRESS */
#define SYSTEM1_ADDR                0x00
#define SYSTEM2_ADDR                0x01
#define SYS_STA_ADDR                0x02
#define INTP_STA1_ADDR              0x03
#define INTP_STA2_ADDR              0x04
#define INTP_CTRL1_ADDR             0x05
#define INTP_CTRL2_ADDR             0x06
#define GPIO_CFG1_ADDR              0x07
#define GPIO_CFG2_ADDR              0x08
#define GPIO_CFG3_ADDR              0x09
#define IO_CTRL_ADDR                0x0a
#define IO_STA_ADDR                 0x0b
#define WUT_CTRL_ADDR               0x0c
#define WUT_PERIOD1_ADDR            0x0d
#define WUT_PERIOD2_ADDR            0x0e
#define DATA_RATE1_ADDR             0x0f
#define DATA_RATE2_ADDR             0x10
#define FDEV_ADDR                   0x11
#define MOD_CTRL_ADDR               0x12
#define MOD_TYPE_ADDR               0x13
#define PREA_LEN_ADDR               0x14
#define SYNC_LEN_ADDR               0x15
#define TXPKT_LEN_ADDR              0x16
#define RXPKT_LEN_ADDR              0x17
#define SYNC_WORD3_ADDR             0x18
#define SYNC_WORD2_ADDR             0x19
#define SYNC_WORD1_ADDR             0x1a
#define SYNC_WORD0_ADDR             0x1b
#define TX_HDR3_ADDR                0x1c
#define TX_HDR2_ADDR                0x1d
#define TX_HDR1_ADDR                0x1e
#define TX_HDR0_ADDR                0x1f
#define AGC_CFG1_ADDR               0x21
#define AGC_CFG2_ADDR               0x22
#define AGC_FILT1_ADDR              0x23
#define AGC_FILT2_ADDR              0x24
#define DAGC_HEAD_ADDR              0x25
#define DAGC_CFG_ADDR               0x26
#define DAGC_FILT_ADDR              0x27
#define RSSI_CNST_ADDR              0x28
#define RSSI_REFH_ADDR              0x29
#define RSSI_REFL_ADDR              0x2a
#define RSSI_FILT_ADDR              0x2b
#define AGC_STA_ADDR                0x2c
#define DAGC_STA_ADDR               0x2d
#define RSSI_VAL_ADDR               0x2e
#define RSSI_CUR_ADDR               0x2f
#define	AFC_VAL_ADDR  	            0x30
#define	BER0_ADDR                   0x31
#define	PREA_TH_ADDR	            0x34
#define CHDR_CTRL_ADDR              0x35
#define CIC_CTRL_ADDR               0x42
#define CFILT_CTRL_ADDR             0x43
#define DEMOD_ADDR                  0x44
#define RX_OSR_ADDR                 0x45
#define AFC_MODE_ADDR               0x4a
#define AFC_SYM_ADDR                0x4b
#define AFC_DR_ADDR                 0x4c
#define AFC_LIMIT1_ADDR             0x4f
#define AFC_LIMIT2_ADDR             0x50
#define FDEV_ANA1_ADDR              0x59
#define FDEV_ANA2_ADDR              0x5a
#define LPF_ANA1_ADDR               0x5b
#define LPF_ANA2_ADDR               0x5c
#define PLL1_ADDR                   0x5d
#define PLL2_ADDR                   0x5e
#define PLL_DEN1_ADDR               0x5f
#define PLL_DEN2_ADDR               0x60
#define PLL_NUMRX1_ADDR             0x61
#define PLL_NUMRX2_ADDR             0x62
#define PLL_NUMTX1_ADDR             0x63
#define PLL_NUMTX2_ADDR             0x64
#define PLL_N_RX_ADDR               0x65
#define PLL_N_TX_ADDR               0x66
#define PLL_N_ADDR                  0x67
#define ANA_TEST_ADDR               0x68
#define PA_CTRL_ADDR                0x69
#define VCO_BAND_ADDR               0x6a
#define OSC32_CTRL_ADDR             0x6b
#define OSC32_STA_ADDR              0x6c
#define MBIST_ADDR                  0x77
#define TEST_DIG_ADDR               0x78
#define TEST_SEL_ADDR               0x79
#define CID1_ADDR                   0x7a
#define CID2_ADDR                   0x7b
#define FIFO_CTRL1_ADDR             0x7c
#define FIFO_CTRL2_ADDR             0x7d
#define FIFO_CTRL3_ADDR             0x7e
#define FIFO_DATA_ADDR              0x7f

/* REGISTER SUBITEM */
#define SRST_REG                    0x80
#define SRST_DP                     0x40
#define FSM_STA_MASK	            0x07
#define FSM_STA_TUNE	            0x04
#define FSM_STA_RX                  0x06
#define FSM_STA_READY	            0x03  
#define RX_MPKT                     0x80
#define AUTO_TX                     0x40
#define XTAL_T1MSHH                 0x30
#define XTAL_T1MSH                  0x20
#define XTAL_T1MSL                  0x10
#define XTAL_T1MSLL                 0x00
#define XTAL_REQ                    0x08
#define PLL_REQ                     0x04
#define RX_REQ                      0x02
#define TX_REQ                      0x01
#define PREA_DET                    0x01
#define SYNC_DET                    0x02
#define RXPKT_DONE                  0x20
#define TXPKT_DONE                  0x10
#define RXFF_AF                     0x08
#define TXFF_AE                     0x04
#define TXFF_AF                     0x02
#define FF_ERROR                    0x01
#define CRC_ERROR                   0x40
#define EXT_IEN                     0x80
#define CRCERR_IEN                  0x40
#define RXPKT_IEN                   0x20
#define TXPKT_IEN                   0x10
#define RXFFAF_IEN                  0x08
#define TXFFAE_IEN                  0x04
#define TXFFAF_IEN                  0x02
#define FFERR_IEN                   0x01
#define RXFIFO_CLR                  0x20
#define TXFIFO_CLR                  0x10
#define RSSI_DET                    0x04

#define RF_AUTO_SEND_EN             0U //射频自动发送使能
#define RF_DEBUG_EN                 0U //射频调试使能
#define RF_WORK_MONITOR_EN          0U //射频监控使能，主要是AFC以及射频状态监控

#define RF_DIS_INTP()                ilvl_int0ic = 0 //射频禁能中断
#define RF_EN_INTP()                 ilvl_int0ic = 2 //射频使能中断

#if (RF_AUTO_SEND_EN > 0U)
#define TX_LED_ON()          (p1_1 = 1)
#define TX_LED_OFF()         (p1_1 = 0)          
#define RX_LED_ON()          (p1_1 = 1)
#define RX_LED_OFF()         (p1_1 = 0)
#else
#define TX_LED_ON() 
#define TX_LED_OFF()           
#define RX_LED_ON()
#define RX_LED_OFF() 
#endif

/* CMD */
#define RF_WRITE_FIFO_CMD         0xff //写fifo命令
#define RF_READ_FIFO_CMD          0x7f //读fifo命令

/* MASK */
#define RF_WRITE_REG_MASK         0x80 //写寄存器掩码
#define RF_READ_REG_MASK          0x7f //读寄存器掩码

/* SYNC WORD */
#define RF_SYNC_WORD3_VALUE       0xd3 //同步字3
#define RF_SYNC_WORD2_VALUE       0x91 //同步字2
#define RF_SYNC_WORD1_VALUE       0x00 //同步字1
#define RF_SYNC_WORD0_VALUE       0x00 //同步字0

/* THRESHOLD */
#define RF_TX_AF_THRESHOLD	      0x37 //射频发送将满门限
#define RF_TX_AE_THRESHOLD	      0x08 //射频发送将空门限
#define RF_RX_AF_THRESHOLD	      0x2f //射频接收将满门限

/* AIR BAUD RATE */
#define RF_AIR_BAUD_RATE        9600UL //射频空中波特率

/* TX POWER */
#define RF_TX_POWER_VALUE         0x0f //射频发射功率

/* RF BUF LEN */
#define MAX_RF_SEND_BUF_LEN          DEFAULT_BUF_SIZE //射频发送缓冲区长度
#define MAX_RF_RECV_BUF_LEN          1 //射频接收缓冲区长度

/* RF EXT ENABLE */
#define RF_EXT_EN(val)                 //射频全局使能、禁能，0关闭，1打开

/* OTHERS */
#if (19200UL == RF_AIR_BAUD_RATE)
#define RF_TRANSMIT_TIMEOUT         (CHECK_TIME_255B_19200 / SYS_TICK_PERIOD) //19200bps的空中波特率发送、接收255字节的超时时间
#elif (4800UL == RF_AIR_BAUD_RATE)
#define RF_TRANSMIT_TIMEOUT          (CHECK_TIME_255B_4800 / SYS_TICK_PERIOD) //4800bps的空中波特率发送、接收255字节的超时时间
#else
#define RF_TRANSMIT_TIMEOUT          (CHECK_TIME_255B_9600 / SYS_TICK_PERIOD) //9600bps的空中波特率发送、接收255字节的超时时间
#endif

#define MAX_RF_LISTEN_OPERATE_CHECK_TIME  (1000UL * 30) //30s

enum _rf_state {
	RF_STATE_SHUTDOWN = 0,          //射频掉电态
	RF_STATE_SLEEP,                 //射频睡眠态
	RF_STATE_READY,		            //射频预备态
	RF_STATE_TUNE,                  //射频调谐态
	RF_STATE_LISTEN,	            //射频监听态
	RF_STATE_RX,		            //射频接收态
	RF_STATE_TX                     //射频发送态
};

typedef struct _rf_param {
    u8 state;                       //射频的状态，接收态、发送态、监听态、空闲态
    volatile tick last_listen_operate_time; //上一次监听操作时间
    int rssi;                       //接收信号强度，单位dB
    int cfo;                        //频偏，单位Hz
    bool state_monitor;             //射频工作状态监控标志
    bool rst_dp;                    //复位数据通路标志
    bool fdev_monitor;              //频偏监测标志
    bool rssi_monitor;              //接收信号强度监测
    u8 idle_state;                  //idle状态
    u16 work_monitor_period;        //射频运行状态监控周期

    struct _tx {                    //发送数据结构
        u8 buf[MAX_RF_SEND_BUF_LEN];//发送缓冲区
        u8 * volatile read_index;   //发送数据读索引
        u8 len;                     //缓冲区数据长度
        u8 timeout;                 //发送超时时间
        bool complete;              //发送完成标志
        bool busy;                  //发送忙标志
    } tx;
    
	struct _rx {				    //接收数据结构	
		u8 buf[MAX_RF_RECV_BUF_LEN];//接收缓冲区
		u8 pkt_len;                 //接收数据包长度
		volatile u8 index;          //接收缓冲区索引
		u8 timeout;                 //接收超时时间
	} rx;
    
#if (RF_DEBUG_EN > 0U)
    u16 ff_error_times;             //fifo操作出错次数
    u16 crc_error_times;            //crc校验不通过次数
    u16 recv_pkt_lost_times;        //接收数据不完整次数
    u16 send_timeout_times;         //发送超时次数
    u16 recv_timeout_times;         //接收超时次数
    u16 send_pkt_num;               //发送包总数
    u16 recv_pkt_num;               //接收包总数
    u32 send_bytes;                 //发送总字节数
    u32 recv_bytes;                 //接收总字节数
    u16 send_busy_times;            //射频发送忙次数
    u16 send_nothing_times;         //射频空发送次数
    u16 send_pkt_overlength_times;  //发送包长度过长的包数
    u16 recv_pkt_overlength_times;  //接收包长度过长的包数
    u16 intp_times;                 //射频中断次数
    u16 reset_chip_times;           //射频软复位芯片次数
    u16 reset_dp_times;             //射频软复位数据通路次数
    u8 reg_val;                     //用于记录射频寄存器值
    u16 error_state_times;          //状态错误次数
    u16 error_reg_assign_times;     //寄存器赋值失败次数
#endif
} RF_PARAM;

extern RF_PARAM g_rf_param; //射频参数

u8 rf_read_reg(u8 addr); //读射频寄存器
void rf_write_reg(u8 addr, u8 val); //写射频寄存器
void init_rf(void); //射频初始化
void rf_reset_chip(void); //射频软复位
void rf_rssi_monitor(void); //射频接收信号强度监测
void rf_fdev_monitor(void); //射频接收频偏监测
void rf_read_fifo(u8 len); //射频读fifo
void rf_write_fifo(void); //射频写fifo
void rf_state_shutdown(void); //射频转掉电模式
void rf_state_sleep(void); //射频转睡眠模式
void rf_wake_up(void); //射频唤醒，唤醒以后进入监听状态
void rf_state_idle(void); //射频转空闲模式
void rf_state_tx(void); //射频转发送模式
void rf_state_listen(void); //射频转监听模式
void rf_send_data(u8 *buf, u8 len); //射频发送数据
void rf_afc(void); //射频自动频率校准
void rf_work_monitor(void); //射频运行状态监控


#ifdef __cplusplus
}
#endif

#endif
