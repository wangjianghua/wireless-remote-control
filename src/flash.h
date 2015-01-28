#ifndef __FLASH_H__
#define __FLASH_H__

#ifdef __cplusplus
extern "C" {
#endif


#define FLASH_ERASE_CMD1          0x20 //���������1
#define FLASH_ERASE_CMD2          0xd0 //���������2
#define FLASH_WRITE_CMD           0x40 //д����
#define FLASH_READ_CMD            0xff //������
#define FLASH_CLEAR_STATUS_CMD    0x50 //��״̬����
#define FLASH_READ_STATUS_CMD     0x70 //��״̬����

#define FLASH_BLOCK_A                0 //��A
#define FLASH_BLOCK_B                1 //��B

#define FLASH_BLOCK_A_ADDR         ((u8 *)0x2400) //��A��ʼ��ַ
#define FLASH_BLOCK_A_END_ADDR     ((u8 *)0x27ff) //��A������ַ
#define FLASH_BLOCK_B_ADDR         ((u8 *)0x2800) //��B��ʼ��ַ
#define FLASH_BLOCK_B_END_ADDR     ((u8 *)0x2bff) //��B������ַ

#define NORMAL                    0x00 //����
#define CMD_SEQ_ERROR             0x01 //����ʱ�����
#define ERASE_ERROR               0x02 //��������
#define PROGRAM_ERROR             0x03 //��̳���
#define BLOCK_SELECT_ERROR        0x04 //��ѡ�����
#define OUT_OF_RANGE              0x05 //������Χ

u8 flash_erase_block(u8 block); //���������ţ�1��1KB
u8 flash_write_data(u8 block, u16 offset, u8 *buf, u16 len); //д���ݣ���š�ƫ���������ݻ�����������
u8 flash_read_data(u8 block, u16 offset, u8 *buf, u16 len); //�����ݣ���š�ƫ���������ݻ�����������
void flash_write_init(void); //д��ʼ��


#ifdef __cplusplus
}
#endif

#endif
