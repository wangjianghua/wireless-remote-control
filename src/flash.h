#ifndef __FLASH_H__
#define __FLASH_H__

#ifdef __cplusplus
extern "C" {
#endif


#define FLASH_ERASE_CMD1          0x20 //块擦除命令1
#define FLASH_ERASE_CMD2          0xd0 //块擦除命令2
#define FLASH_WRITE_CMD           0x40 //写命令
#define FLASH_READ_CMD            0xff //读命令
#define FLASH_CLEAR_STATUS_CMD    0x50 //清状态命令
#define FLASH_READ_STATUS_CMD     0x70 //读状态命令

#define FLASH_BLOCK_A                0 //块A
#define FLASH_BLOCK_B                1 //块B

#define FLASH_BLOCK_A_ADDR         ((u8 *)0x2400) //块A起始地址
#define FLASH_BLOCK_A_END_ADDR     ((u8 *)0x27ff) //块A结束地址
#define FLASH_BLOCK_B_ADDR         ((u8 *)0x2800) //块B起始地址
#define FLASH_BLOCK_B_END_ADDR     ((u8 *)0x2bff) //块B结束地址

#define NORMAL                    0x00 //正常
#define CMD_SEQ_ERROR             0x01 //命令时序出错
#define ERASE_ERROR               0x02 //擦除出错
#define PROGRAM_ERROR             0x03 //编程出错
#define BLOCK_SELECT_ERROR        0x04 //块选择出错
#define OUT_OF_RANGE              0x05 //超出范围

u8 flash_erase_block(u8 block); //块擦除，块号，1块1KB
u8 flash_write_data(u8 block, u16 offset, u8 *buf, u16 len); //写数据，块号、偏移量、数据缓冲区、长度
u8 flash_read_data(u8 block, u16 offset, u8 *buf, u16 len); //读数据，块号、偏移量、数据缓冲区、长度
void flash_write_init(void); //写初始化


#ifdef __cplusplus
}
#endif

#endif
