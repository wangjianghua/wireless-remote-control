#ifndef __SPI_H__
#define __SPI_H__

#ifdef __cplusplus
extern "C" {
#endif


#define SPI_CS_LOW()        (p3_4 = 0) //P3.4
#define SPI_CS_HIGH()       (p3_4 = 1)

#define SPI_MOSI_LOW()      (p3_7 = 0) //P3.7
#define SPI_MOSI_HIGH()     (p3_7 = 1)

#define SPI_MISO()          (p3_3)     //P3.3

#define SPI_SCLK_LOW()      (p3_5 = 0) //P3.5
#define SPI_SCLK_HIGH()     (p3_5 = 1)


void init_spi(void);
unsigned char spi_read_byte(void);
void spi_write_byte(unsigned char val);


#ifdef __cplusplus
}
#endif

#endif
