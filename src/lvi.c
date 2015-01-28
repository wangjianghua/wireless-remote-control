#include "includes.h"


void init_lvi(void)
{
	prc3 = 1; //允许写VCA2、VW1C和VW2C寄存器
	
    /* VW2C 寄存器 */
	vw2c0 = 1; //电压监视2中断/复位允许
	vw2c1 = 0; //数字滤波器电路有效
	vw2c2 = 0; //未检测
	vw2c3 = 0; //WDT，未检测
	vw2f0 = 0; //FRING-S的1分频
    vw2f0 = 0;
    vw2c6 = 0; //电压监视2中断模式
    vw2c7 = 1; //VCC在Vdet2以下时产生中断/复位

	vca27 = 1; //电压检测电路2有效
        
	prc3 = 0; //禁止写VCA2、VW1C和VW2C寄存器   
}

