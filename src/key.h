#ifndef __KEY_H__
#define __KEY_H__

#ifdef __cplusplus
extern "C" {
#endif


#define KEY_DEBUG_EN                0U //ң�ص���ʹ��

#define CHIP_ID_ADDR               (0x2400) //оƬID�Ĵ洢��ַ
#define CHIP_ID_BLOCK               FLASH_BLOCK_A //оƬID����flash���ݿ�
#define CHIP_ID_OFFSET              0 //оƬID����flash���ݿ��ƫ��
#define CHIP_ID_LEN                 4 //оƬID����

#define PWD_ADDR                   (0x2404) //�û�����Ĵ洢��ַ
#define PWD_BLOCK                   FLASH_BLOCK_A //�û���������flash���ݿ�
#define PWD_OFFSET                  4 //�û���������flash���ݿ��ƫ��
#define PWD_LEN                     4 //�û����볤��

#define HOPE_CODE_ADDR             (0x2800) //����Ĵ洢��ַ    
#define HOPE_CODE_BLOCK             FLASH_BLOCK_B //��������flash���ݿ�
#define HOPE_CODE_LEN               4 //���볤��

#define MAX_KEY_RELEASED_CHECK_TIME (300UL) //300ms

#define KEY_LED_SWAP_PERIOD         (300UL) //300ms

#define KEY_LED_PORT_INIT() (pd1_1 = 1)

#define KEY_LED_ON()         (p1_1 = 1)
#define KEY_LED_OFF()        (p1_1 = 0) 
#define KEY_LED_SWAP()       (p1_1 ^= 1)

#define KEY_LOCK_CTRL_PORT_INIT()   (pd1_2 = 0)
#define KEY_LOCK                    (p1_2)
#define KEY_UNLOCK_CTRL_PORT_INIT() (pd1_3 = 0)
#define KEY_UNLOCK                  (p1_3)

#define KEY_MSG_LOCK              0x55  //����
#define KEY_MSG_UNLOCK            0xaa  //����
#define KEY_MSG_UNKNOW            0x5a  //δ֪����

/* ֡�ṹ���� */
#define KEY_FRAME_LEN             16
#define LEAD_CODE                 g_key_param.buf[0]  //ǰ����
#define CHIP_ID_0                 g_key_param.buf[1]  //оƬID
#define CHIP_ID_1                 g_key_param.buf[2]
#define CHIP_ID_2                 g_key_param.buf[3]
#define CHIP_ID_3                 g_key_param.buf[4]  
#define LVD                       g_key_param.buf[5]  //�͵�ѹָʾ
#define KEY_CODE                  g_key_param.buf[6]  //������
#define SYNC_0                    g_key_param.buf[7]  //ͬ����
#define SYNC_1                    g_key_param.buf[8]
#define SYNC_2                    g_key_param.buf[9]  
#define SYNC_3                    g_key_param.buf[10] 
#define RESERVE_0                 g_key_param.buf[11] //����
#define RESERVE_1                 g_key_param.buf[12]
#define RESERVE_2                 g_key_param.buf[13]
#define RESERVE_3                 g_key_param.buf[14] 
#define KEY_CRC                   g_key_param.buf[15] //����CRC

typedef struct _KEY_PARAM {
    u8 msg;                         //������Ϣ
    u8 buf[KEY_FRAME_LEN];          //ң�ػ�����
    u8 lvd;                         //�͵�ѹָʾ
    tick start_time;                //�ȴ������ͷŵĿ�ʼʱ��
    bool press;                     //���°�����־

#if (KEY_DEBUG_EN > 0U)
    u16 press_key_times;            //��������
#endif
} KEY_PARAM;

extern KEY_PARAM g_key_param; //����ң�ز���

u8 set_hope_code(u8 *buf); //�������� 
u32 get_hop_code(void); //��ȡ����
u8 set_chip_id(u8 *buf); //����оƬID
void get_chip_id(u8 *buf); //��ȡоƬID
u8 set_pwd(u8 *buf); //�����û�����
void get_pwd(u8 *buf); //��ȡ�û�����


#ifdef __cplusplus
}
#endif

#endif
