#include "includes.h"


KEY_PARAM g_key_param;

ROOT const u8 default_chip_id[] @ CHIP_ID_ADDR = {'H', 'R', 'K', ' '};
ROOT const u8 default_user_password[] @ PWD_ADDR = {'1', '2', '3', 's'};
ROOT const u32 default_hope_code @ HOPE_CODE_ADDR = 0x00000000;

u8 set_hope_code(u8 *buf)
{
    u8 ret;

    
    if(NULL == buf)
    {
        return (!NORMAL);
    }
    
    ret = flash_write_data(HOPE_CODE_BLOCK, 0, buf, HOPE_CODE_LEN);
    return (ret);
}

u32 get_hop_code(void)
{
    u8 ret;
    u8 buf[4];
    u32 *ptr = (u32 *)HOPE_CODE_ADDR;
    u32 temp;


    for(u8 i=0; i<0xff; i++)
    {
        if(0xfe == i)
        {
            ret = flash_erase_block(HOPE_CODE_BLOCK); //Ω´¬˙£¨≤¡≥˝Ã¯¬ÎÀ˘‘⁄øÈ

            if(NORMAL != ret)
            {
                ret = flash_erase_block(HOPE_CODE_BLOCK); //≤¡≥˝Ã¯¬ÎÀ˘‘⁄øÈ ß∞‹£¨≥¢ ‘‘Ÿ¥Œ≤¡≥˝

                if(NORMAL != ret)
                {
                    WARN(("<flash_erase_error> %s(%d) - %s\n", __FILE__, __LINE__, __FUNCTION__));
                }
            }

            temp++; //Ã¯¬Î◊‘º”
            
            buf[0] = temp >> 0;
            buf[1] = temp >> 8;
            buf[2] = temp >> 16;
            buf[3] = temp >> 24;

            ret = flash_write_data(HOPE_CODE_BLOCK, 0, buf, HOPE_CODE_LEN); //¥Ê¥¢Ã¯¬Î

            if(NORMAL != ret)
            {
                WARN(("<flash_write_error> %s(%d) - %s\n", __FILE__, __LINE__, __FUNCTION__));
            }

            flash_read_data(HOPE_CODE_BLOCK, 0, buf, HOPE_CODE_LEN); //∂¡»°Ã¯¬Î

            temp = ((u32)buf[3] << 24) | ((u32)buf[2] << 16) | ((u32)buf[1] << 8) | ((u32)buf[0] << 0);

            break;
        }
        
        if(0xffffffff != *ptr)
        {
            temp = *ptr;
            ptr++;
        }
        else
        {
            temp++; //Ã¯¬Î◊‘º”
            
            buf[0] = temp >> 0;
            buf[1] = temp >> 8;
            buf[2] = temp >> 16;
            buf[3] = temp >> 24;
                
            ret = flash_write_data(HOPE_CODE_BLOCK, i * 4, buf, HOPE_CODE_LEN); //¥Ê¥¢Ã¯¬Î

            if(NORMAL != ret)
            {
                WARN(("<flash_write_error> %s(%d) - %s\n", __FILE__, __LINE__, __FUNCTION__));
            }
            
            flash_read_data(HOPE_CODE_BLOCK, i * 4, buf, HOPE_CODE_LEN); //∂¡»°Ã¯¬Î

            temp = ((u32)buf[3] << 24) | ((u32)buf[2] << 16) | ((u32)buf[1] << 8) | ((u32)buf[0] << 0);

            break;
        }
    }
    
    return (temp);
}

u8 set_chip_id(u8 *buf)
{
    u8 ret;
    

    if(NULL == buf)
    {
        return (!NORMAL);
    }
    
    ret = flash_write_data(CHIP_ID_BLOCK, CHIP_ID_OFFSET, buf, CHIP_ID_LEN);
    return (ret);
}

void get_chip_id(u8 *buf)
{
    if(NULL == buf)
    {
        return;
    }
    
    flash_read_data(CHIP_ID_BLOCK, CHIP_ID_OFFSET, buf, CHIP_ID_LEN);
}

u8 set_pwd(u8 *buf)
{
    u8 ret;
    

    if(NULL == buf)
    {
        return (!NORMAL);
    }
    
    ret = flash_write_data(PWD_BLOCK, PWD_OFFSET, buf, PWD_LEN);
    return (ret);
}

void get_pwd(u8 *buf)
{
    if(NULL == buf)
    {
        return;
    }
    
    flash_read_data(PWD_BLOCK, PWD_OFFSET, buf, PWD_LEN);
}


