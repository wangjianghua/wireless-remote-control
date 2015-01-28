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

#define RF_AUTO_SEND_EN             0U //��Ƶ�Զ�����ʹ��
#define RF_DEBUG_EN                 0U //��Ƶ����ʹ��
#define RF_WORK_MONITOR_EN          0U //��Ƶ���ʹ�ܣ���Ҫ��AFC�Լ���Ƶ״̬���

#define RF_DIS_INTP()                ilvl_int0ic = 0 //��Ƶ�����ж�
#define RF_EN_INTP()                 ilvl_int0ic = 2 //��Ƶʹ���ж�

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
#define RF_WRITE_FIFO_CMD         0xff //дfifo����
#define RF_READ_FIFO_CMD          0x7f //��fifo����

/* MASK */
#define RF_WRITE_REG_MASK         0x80 //д�Ĵ�������
#define RF_READ_REG_MASK          0x7f //���Ĵ�������

/* SYNC WORD */
#define RF_SYNC_WORD3_VALUE       0xd3 //ͬ����3
#define RF_SYNC_WORD2_VALUE       0x91 //ͬ����2
#define RF_SYNC_WORD1_VALUE       0x00 //ͬ����1
#define RF_SYNC_WORD0_VALUE       0x00 //ͬ����0

/* THRESHOLD */
#define RF_TX_AF_THRESHOLD	      0x37 //��Ƶ���ͽ�������
#define RF_TX_AE_THRESHOLD	      0x08 //��Ƶ���ͽ�������
#define RF_RX_AF_THRESHOLD	      0x2f //��Ƶ���ս�������

/* AIR BAUD RATE */
#define RF_AIR_BAUD_RATE        9600UL //��Ƶ���в�����

/* TX POWER */
#define RF_TX_POWER_VALUE         0x0f //��Ƶ���书��

/* RF BUF LEN */
#define MAX_RF_SEND_BUF_LEN          DEFAULT_BUF_SIZE //��Ƶ���ͻ���������
#define MAX_RF_RECV_BUF_LEN          1 //��Ƶ���ջ���������

/* RF EXT ENABLE */
#define RF_EXT_EN(val)                 //��Ƶȫ��ʹ�ܡ����ܣ�0�رգ�1��

/* OTHERS */
#if (19200UL == RF_AIR_BAUD_RATE)
#define RF_TRANSMIT_TIMEOUT         (CHECK_TIME_255B_19200 / SYS_TICK_PERIOD) //19200bps�Ŀ��в����ʷ��͡�����255�ֽڵĳ�ʱʱ��
#elif (4800UL == RF_AIR_BAUD_RATE)
#define RF_TRANSMIT_TIMEOUT          (CHECK_TIME_255B_4800 / SYS_TICK_PERIOD) //4800bps�Ŀ��в����ʷ��͡�����255�ֽڵĳ�ʱʱ��
#else
#define RF_TRANSMIT_TIMEOUT          (CHECK_TIME_255B_9600 / SYS_TICK_PERIOD) //9600bps�Ŀ��в����ʷ��͡�����255�ֽڵĳ�ʱʱ��
#endif

#define MAX_RF_LISTEN_OPERATE_CHECK_TIME  (1000UL * 30) //30s

enum _rf_state {
	RF_STATE_SHUTDOWN = 0,          //��Ƶ����̬
	RF_STATE_SLEEP,                 //��Ƶ˯��̬
	RF_STATE_READY,		            //��ƵԤ��̬
	RF_STATE_TUNE,                  //��Ƶ��г̬
	RF_STATE_LISTEN,	            //��Ƶ����̬
	RF_STATE_RX,		            //��Ƶ����̬
	RF_STATE_TX                     //��Ƶ����̬
};

typedef struct _rf_param {
    u8 state;                       //��Ƶ��״̬������̬������̬������̬������̬
    volatile tick last_listen_operate_time; //��һ�μ�������ʱ��
    int rssi;                       //�����ź�ǿ�ȣ���λdB
    int cfo;                        //Ƶƫ����λHz
    bool state_monitor;             //��Ƶ����״̬��ر�־
    bool rst_dp;                    //��λ����ͨ·��־
    bool fdev_monitor;              //Ƶƫ����־
    bool rssi_monitor;              //�����ź�ǿ�ȼ��
    u8 idle_state;                  //idle״̬
    u16 work_monitor_period;        //��Ƶ����״̬�������

    struct _tx {                    //�������ݽṹ
        u8 buf[MAX_RF_SEND_BUF_LEN];//���ͻ�����
        u8 * volatile read_index;   //�������ݶ�����
        u8 len;                     //���������ݳ���
        u8 timeout;                 //���ͳ�ʱʱ��
        bool complete;              //������ɱ�־
        bool busy;                  //����æ��־
    } tx;
    
	struct _rx {				    //�������ݽṹ	
		u8 buf[MAX_RF_RECV_BUF_LEN];//���ջ�����
		u8 pkt_len;                 //�������ݰ�����
		volatile u8 index;          //���ջ���������
		u8 timeout;                 //���ճ�ʱʱ��
	} rx;
    
#if (RF_DEBUG_EN > 0U)
    u16 ff_error_times;             //fifo�����������
    u16 crc_error_times;            //crcУ�鲻ͨ������
    u16 recv_pkt_lost_times;        //�������ݲ���������
    u16 send_timeout_times;         //���ͳ�ʱ����
    u16 recv_timeout_times;         //���ճ�ʱ����
    u16 send_pkt_num;               //���Ͱ�����
    u16 recv_pkt_num;               //���հ�����
    u32 send_bytes;                 //�������ֽ���
    u32 recv_bytes;                 //�������ֽ���
    u16 send_busy_times;            //��Ƶ����æ����
    u16 send_nothing_times;         //��Ƶ�շ��ʹ���
    u16 send_pkt_overlength_times;  //���Ͱ����ȹ����İ���
    u16 recv_pkt_overlength_times;  //���հ����ȹ����İ���
    u16 intp_times;                 //��Ƶ�жϴ���
    u16 reset_chip_times;           //��Ƶ��λоƬ����
    u16 reset_dp_times;             //��Ƶ��λ����ͨ·����
    u8 reg_val;                     //���ڼ�¼��Ƶ�Ĵ���ֵ
    u16 error_state_times;          //״̬�������
    u16 error_reg_assign_times;     //�Ĵ�����ֵʧ�ܴ���
#endif
} RF_PARAM;

extern RF_PARAM g_rf_param; //��Ƶ����

u8 rf_read_reg(u8 addr); //����Ƶ�Ĵ���
void rf_write_reg(u8 addr, u8 val); //д��Ƶ�Ĵ���
void init_rf(void); //��Ƶ��ʼ��
void rf_reset_chip(void); //��Ƶ��λ
void rf_rssi_monitor(void); //��Ƶ�����ź�ǿ�ȼ��
void rf_fdev_monitor(void); //��Ƶ����Ƶƫ���
void rf_read_fifo(u8 len); //��Ƶ��fifo
void rf_write_fifo(void); //��Ƶдfifo
void rf_state_shutdown(void); //��Ƶת����ģʽ
void rf_state_sleep(void); //��Ƶת˯��ģʽ
void rf_wake_up(void); //��Ƶ���ѣ������Ժ�������״̬
void rf_state_idle(void); //��Ƶת����ģʽ
void rf_state_tx(void); //��Ƶת����ģʽ
void rf_state_listen(void); //��Ƶת����ģʽ
void rf_send_data(u8 *buf, u8 len); //��Ƶ��������
void rf_afc(void); //��Ƶ�Զ�Ƶ��У׼
void rf_work_monitor(void); //��Ƶ����״̬���


#ifdef __cplusplus
}
#endif

#endif
