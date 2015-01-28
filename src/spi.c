/** 
 * @file     spi.c
 * @brief    spi驱动
 * @details  瑞萨单片机(nec mcu)，R8C/1B - R5F211B4，I/O模拟spi实现
 * @author   华兄
 * @email    591881218@qq.com
 * @date     2013
 * @version  vX.XX
 * @par Copyright (c):  
 *           深圳市合尔凯科技有限公司
 * @par History:          
 *   version: author, date, desc\n 
 */ 

#include "includes.h"


void init_spi(void)
{
    pd3_3 = 0;
    pd3_4 = 1;
    pd3_5 = 1;
    pd3_7 = 1;

    SPI_CS_HIGH();
    SPI_SCLK_LOW();
    SPI_MOSI_LOW();
}

unsigned char spi_read_byte(void)
{
    unsigned char i, val;
    

    val = 0;
    
    for(i=0; i<8; i++)
    {
        SPI_SCLK_LOW();
        {
            NOP();
            NOP();
            NOP();
            NOP();
        }
        SPI_SCLK_HIGH();

        val = val << 1;

        if(SPI_MISO())
            val = val | 0x01;
        else 
            val = val & 0xfe;
    }

    SPI_SCLK_LOW();
    
    return (val);
}

void spi_write_byte(unsigned char val)
{
    unsigned char i, temp;
    

    temp = val;
    
    for(i=0; i<8; i++)
    {
        if(temp & 0x80)
            SPI_MOSI_HIGH();
        else
            SPI_MOSI_LOW();
    
        SPI_SCLK_LOW();
        {
            NOP();
            NOP();
            NOP();
            NOP();
        }
        SPI_SCLK_HIGH();
        
        temp = temp << 1;
    }

    SPI_SCLK_LOW();
    SPI_MOSI_LOW();
}

