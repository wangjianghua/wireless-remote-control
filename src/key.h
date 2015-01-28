#ifndef __KEY_H__
#define __KEY_H__

#ifdef __cplusplus
extern "C" {
#endif


#define KEY_DEBUG_EN                0U //遥控调试使能

#define CHIP_ID_ADDR               (0x2400) //芯片ID的存储地址
#define CHIP_ID_BLOCK               FLASH_BLOCK_A //芯片ID所在flash数据块
#define CHIP_ID_OFFSET              0 //芯片ID所在flash数据块的偏移
#define CHIP_ID_LEN                 4 //芯片ID长度

#define PWD_ADDR                   (0x2404) //用户密码的存储地址
#define PWD_BLOCK                   FLASH_BLOCK_A //用户密码所在flash数据块
#define PWD_OFFSET                  4 //用户密码所在flash数据块的偏移
#define PWD_LEN                     4 //用户密码长度

#define HOPE_CODE_ADDR             (0x2800) //跳码的存储地址    
#define HOPE_CODE_BLOCK             FLASH_BLOCK_B //跳码所在flash数据块
#define HOPE_CODE_LEN               4 //跳码长度

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

#define KEY_MSG_LOCK              0x55  //加锁
#define KEY_MSG_UNLOCK            0xaa  //解锁
#define KEY_MSG_UNKNOW            0x5a  //未知按键

/* 帧结构定义 */
#define KEY_FRAME_LEN             16
#define LEAD_CODE                 g_key_param.buf[0]  //前导码
#define CHIP_ID_0                 g_key_param.buf[1]  //芯片ID
#define CHIP_ID_1                 g_key_param.buf[2]
#define CHIP_ID_2                 g_key_param.buf[3]
#define CHIP_ID_3                 g_key_param.buf[4]  
#define LVD                       g_key_param.buf[5]  //低电压指示
#define KEY_CODE                  g_key_param.buf[6]  //按键码
#define SYNC_0                    g_key_param.buf[7]  //同步字
#define SYNC_1                    g_key_param.buf[8]
#define SYNC_2                    g_key_param.buf[9]  
#define SYNC_3                    g_key_param.buf[10] 
#define RESERVE_0                 g_key_param.buf[11] //保留
#define RESERVE_1                 g_key_param.buf[12]
#define RESERVE_2                 g_key_param.buf[13]
#define RESERVE_3                 g_key_param.buf[14] 
#define KEY_CRC                   g_key_param.buf[15] //按键CRC

typedef struct _KEY_PARAM {
    u8 msg;                         //按键信息
    u8 buf[KEY_FRAME_LEN];          //遥控缓冲区
    u8 lvd;                         //低电压指示
    tick start_time;                //等待按键释放的开始时间
    bool press;                     //按下按键标志

#if (KEY_DEBUG_EN > 0U)
    u16 press_key_times;            //按键次数
#endif
} KEY_PARAM;

extern KEY_PARAM g_key_param; //滚码遥控参数

u8 set_hope_code(u8 *buf); //设置跳码 
u32 get_hop_code(void); //获取跳码
u8 set_chip_id(u8 *buf); //设置芯片ID
void get_chip_id(u8 *buf); //获取芯片ID
u8 set_pwd(u8 *buf); //设置用户密码
void get_pwd(u8 *buf); //获取用户密码


#ifdef __cplusplus
}
#endif

#endif
