#include "includes.h"


/******************************************************************************
*                                 射频数据结构                                *
******************************************************************************/

RF_PARAM g_rf_param;

/******************************************************************************
*                                寄存器配置参数                               *
******************************************************************************/

typedef struct _rf_common_reg {
    u8 system1_reg;	        //00    
    u8 mod_ctrl_reg;        //12
    u8 mod_type_reg;        //13
    u8 prea_len_reg;        //14
    u8 sync_len_reg;	    //15    
    u8 prea_th_reg;		    //34
    u8 chdr_ctrl_reg;       //35
} rf_common_reg;

typedef struct _rf_freq_reg {
	struct {
        u8 pll1_reg;        //5D
        u8 pll_numrx2_reg;  //62
        u8 pll_numtx1_reg;  //63
        u8 pll_numtx2_reg;  //64
        u8 pll_n_tx_reg;    //66
        u8 pll_n_reg;       //67
        u8 vco_band_reg;    //6A
	} common;
    
    struct {
        u8 lpf_ana1_reg;    //5B
        u8 pll_numrx1_reg;  //61
        u8 pll_n_rx_reg;    //65
    } tx;
    
    struct {
        u8 lpf_ana1_reg;    //5B
        u8 pll_numrx1_reg;  //61
        u8 pll_n_rx_reg;    //65
    } rx;
} rf_freq_reg;

typedef struct _rf_rate_reg {
    u32 rate;
    u8 data_rate1_reg;      //0F
    u8 data_rate2_reg;      //10
    u8 fdev_reg;            //11
    u8 dagc_cfg_reg;        //26
    u8 cic_ctrl_reg;        //42
    u8 cfilt_ctrl_reg;      //43
    u8 demod_reg;           //44
    u8 rx_osr_reg;          //45
    u8 afc_mod_reg;         //4A
    u8 afc_sym_reg;         //4B
    u8 afc_dr_reg;          //4C
    u8 afc_limit1_reg;      //4F
    u8 afc_limit2_reg;      //50
} rf_rate_reg;

typedef struct _rf_power_reg {
    u8 pa_ctrl_reg;         //69
} rf_power_reg;

typedef struct _rf_threshold_reg {
    u8 fifo_ctrl1_reg;      //7C
    u8 fifo_ctrl2_reg;      //7D
    u8 fifo_ctrl3_reg;      //7E
} rf_threshold_reg;

typedef struct _rf_sync_word_reg {
    u8 sync_word3_reg;      //18 
    u8 sync_word2_reg;      //19
    u8 sync_word1_reg;      //1A
    u8 sync_word0_reg;      //1B
} rf_sync_word_reg;

/******************************************************************************
*                              寄存器默认配置                                 *
******************************************************************************/

static const rf_common_reg rf_common_reg_default = 
{
    0x3b,		            //system1_reg	  00 #芯片软复位，数据通路复位
    0x33,                   //mod_ctrl_reg    12 #包长信息使能，发送、接收包处理使能，数据白化，CRC-16
    0x49,	 	            //mod_type_reg    13 #前导检测回退步长，数字电路做间接调制，FIFO模式，FSK调制
    0x18,		            //prea_len_reg    14 #前导码个数，寄存器每加一将增加一个10101010序列
    0x21,		            //sync_len_reg    15 #有1bit的同步字容忍错误，传输过程中没有包头信息，同步字3/2   
    0x0a,		            //prea_th_reg	  34 #同步字检测，需要检测的前导码数量以4比特(1010)为单位
    0x00                    //chdr_ctrl_reg   35 #不校验广播信息，不校验接收的包头信息
};

static const rf_freq_reg rf_freq_reg_default = {
/**********************************************
*     Default frequency: 433.82859375MHz      *
**********************************************/
    {//common
        0x71,               //pll1_reg        5D #工作频段选择，默认为频段3；分频比选择，Fcomp=Fxtal/(pll_r+1)，默认为2分频
        0xdc,               //pll_numrx2_reg  62 #num_rx[7:0]，接收频率合成器小数分频分子值默认是470.5MHz
        0x84,               //pll_numtx1_reg  63 #num_tx[15:8]，发射频率合成器小数分频分子值默认是470.5MHz
        0xdc,               //pll_numtx2_reg  64 #num_tx[7:0]，发射频率合成器值默认是470.5MHz
        0x6d,               //pll_n_tx_reg    66 #n_tx[5:0]，发射频率合成器分频A值控制寄存器
        0x22,               //pll_n_reg       67 #n_tx[9:6]，n_rx[9:6]，发射/接收频率合成器分频B值控制寄存器
        0x01                //vco_band_reg    6A #Vco_band[0]=1'b1: 400~500MHz工作频段，Vco_band[1]=1'b0：800~1GMHz工作频段
    }, 

    {//tx
        0xf9,               //lpf_ana1_reg    5B #LDO电压等级1.8v，模拟低通滤波器带宽选择
        0x84,               //pll_numrx1_reg  61 #num_rx[15:8]，接收频率合成器小数分频分子值默认是470.5MHz
        0x6d                //pll_n_rx_reg    65 #n_rx[5:0]，接收频率合成器分频A值控制寄存器
    },

    {//rx
        0xf1,               //lpf_ana1_reg    5B #LDO电压等级1.8v，模拟低通滤波器带宽选择
        0x52,               //pll_numrx1_reg  61 #num_rx[15:8]，接收频率合成器小数分频分子值默认是470.5MHz
        0x6d                //pll_n_rx_reg    65 #n_rx[5:0]，接收频率合成器分频A值控制寄存器
    }
};

#if (19200UL == RF_AIR_BAUD_RATE)

static const rf_rate_reg rf_rate_reg_default = {
/**********************************************
*     Default rate: 19531.25bps               *
**********************************************/
    RF_AIR_BAUD_RATE,       //19200ul

    0x05,                   //data_rate1_reg  0F #data_rate[15:8]，数据速率控制寄存器1
    0x00,                   //data_rate2_reg  10 #data_rate[7:0]，数据速率控制寄存器2
    0x14,                   //fdev_reg        11 #调制频偏，初始配置20kbps频偏
    0x91,                   //dagc_cfg_reg    26 #
    0x03,                   //cic_ctrl_reg    42 #DC消除滤波器使能信号(普通模式)，DC消除滤波器带宽选择，CIC滤波器降采样级数选择

#if (RF_WORK_MONITOR_EN > 0U)
    0x02,		            //cfilt_ctrl_reg  43 #AFC使用RFIC内部电路计算的结果，接收机信道滤波器抽取速率，信道滤波器带宽选择
#else
    0x82,                   //cfilt_ctrl_reg  43 #关闭AFC，接收机信道滤波器抽取速率，信道滤波器带宽选择
#endif

    0x50,                   //demod_reg       44 #信道滤波器带宽选择，解调延时配置
    0x60,                   //rx_osr_reg      45 #AFC解调数据滤波器带宽选择，接收机过采样速率
    0x58,                   //afc_mod_reg     4A #
    0x02,                   //afc_sym_reg     4B #
    0x00,                   //afc_dr_reg      4C #
    0x50,                   //afc_limit1_reg  4F #
    0x21                    //afc_limit2_reg  50 #
};

#elif (4800UL == RF_AIR_BAUD_RATE)

static const rf_rate_reg rf_rate_reg_default =
{
    RF_AIR_BAUD_RATE,       //4800ul
        
    0x01,       
    0x40,       
    0x05,       
    0x91,       
    0x05,       
    
#if (RF_WORK_MONITOR_EN > 0U)
    0x04,	                
#else
    0x84,                   
#endif		

    0x50,       
    0x60,       
    0x58,       
    0x02,      
    0x00,       
    0x50,       
    0x84        
};

#else

static const rf_rate_reg rf_rate_reg_default =
{
    RF_AIR_BAUD_RATE,       //9600ul
        
    0x02,       
    0x80,       
    0x0a,       
    0x91,       
    0x04,       
    
#if (RF_WORK_MONITOR_EN > 0U)
    0x03,	                
#else
    0x83,                   
#endif		

    0x50,       
    0x60,       
    0x58,       
    0x02,      
    0x00,       
    0x50,       
    0x42        
};

#endif

static const rf_power_reg rf_power_reg_default = 
{
    RF_TX_POWER_VALUE       //pa_ctrl_reg     69 #
};

static const rf_threshold_reg rf_threshold_reg_default = 
{
    RF_TX_AF_THRESHOLD,     //fifo_ctrl1_reg  7C #TX FIFO将满门限
    RF_TX_AE_THRESHOLD,     //fifo_ctrl2_reg  7D #TX FIFO将空门限
    RF_RX_AF_THRESHOLD      //fifo_ctrl3_reg  7E #RX FIFO将满门限
};

static const rf_sync_word_reg rf_sync_word_reg_default = 
{
    RF_SYNC_WORD3_VALUE,    //sync_word3_reg  18 #sync_word[31:24]
    RF_SYNC_WORD2_VALUE,    //sync_word2_reg  19 #sync_word[23:16]
    RF_SYNC_WORD1_VALUE,    //sync_word1_reg  1A #sync_word[15:8]
    RF_SYNC_WORD0_VALUE     //sync_word0_reg  1B #sync_word[7:0]
};

/******************************************************************************
*                                    结束                                     *
******************************************************************************/

#if 0

u8 rf_read_reg(u8 addr)	
{
    u8 c;


    SPI_CS_LOW();
    c = addr & 0x7F;
    g_tx_buf[0] = c;
    g_tx_buf[1] = 0xff;
    CSI01_SendReceiveData(g_tx_buf, 2 , g_rx_buf);
    c = g_rx_buf[1];
    SPI_CS_HIGH();

    return c;
}

void rf_write_reg(u8 addr, u8 val)
{
    u8 c;
    

    SPI_CS_LOW();
    c = addr | 0x80;
    g_tx_buf[0] = c;
    g_tx_buf[1] = val;
    CSI01_SendReceiveData(g_tx_buf, 2 , g_rx_buf);
    SPI_CS_HIGH();
}

#else

u8 rf_read_reg(u8 addr)	
{
    u8 temp;
    

    SPI_CS_LOW();
    temp = addr & RF_READ_REG_MASK;
    spi_write_byte(temp);
    temp = spi_read_byte();
    SPI_CS_HIGH();

    return temp;
}

void rf_write_reg(u8 addr, u8 val)
{
    u8 temp;
    

    SPI_CS_LOW();
    temp = addr | RF_WRITE_REG_MASK;
    spi_write_byte(temp);
    spi_write_byte(val);
    SPI_CS_HIGH();
}

#endif

static void rf_init_reg(void)
{
	/* Tx power registers */
	rf_write_reg(PA_CTRL_ADDR, rf_power_reg_default.pa_ctrl_reg); //69

	/* common registers */
	rf_write_reg(SYSTEM1_ADDR, rf_common_reg_default.system1_reg); //00
	rf_write_reg(MOD_CTRL_ADDR, rf_common_reg_default.mod_ctrl_reg); //12	
	rf_write_reg(MOD_TYPE_ADDR, rf_common_reg_default.mod_type_reg); //13
    rf_write_reg(PREA_LEN_ADDR, rf_common_reg_default.prea_len_reg); //14
	rf_write_reg(SYNC_LEN_ADDR, rf_common_reg_default.sync_len_reg); //15
	rf_write_reg(PREA_TH_ADDR, rf_common_reg_default.prea_th_reg); //34
	rf_write_reg(CHDR_CTRL_ADDR, rf_common_reg_default.chdr_ctrl_reg); //35

	/* freq registers */
	rf_write_reg(PLL1_ADDR , rf_freq_reg_default.common.pll1_reg); //5D
	rf_write_reg(PLL_NUMRX2_ADDR, rf_freq_reg_default.common.pll_numrx2_reg); //62
	rf_write_reg(PLL_NUMTX1_ADDR, rf_freq_reg_default.common.pll_numtx1_reg); //63
	rf_write_reg(PLL_NUMTX2_ADDR, rf_freq_reg_default.common.pll_numtx2_reg); //64
	rf_write_reg(PLL_N_TX_ADDR, rf_freq_reg_default.common.pll_n_tx_reg); //66
	rf_write_reg(PLL_N_ADDR, rf_freq_reg_default.common.pll_n_reg); //67
	rf_write_reg(VCO_BAND_ADDR, rf_freq_reg_default.common.vco_band_reg); //6A
    
	/* rate registers */
	rf_write_reg(DATA_RATE1_ADDR, rf_rate_reg_default.data_rate1_reg); //0F
	rf_write_reg(DATA_RATE2_ADDR, rf_rate_reg_default.data_rate2_reg); //10
	rf_write_reg(FDEV_ADDR, rf_rate_reg_default.fdev_reg); //11
	rf_write_reg(DAGC_CFG_ADDR, rf_rate_reg_default.dagc_cfg_reg); //26
	rf_write_reg(CIC_CTRL_ADDR, rf_rate_reg_default.cic_ctrl_reg); //42
	rf_write_reg(CFILT_CTRL_ADDR, rf_rate_reg_default.cfilt_ctrl_reg); //43
	rf_write_reg(DEMOD_ADDR, rf_rate_reg_default.demod_reg); //44
	rf_write_reg(RX_OSR_ADDR, rf_rate_reg_default.rx_osr_reg); //45
	rf_write_reg(AFC_MODE_ADDR, rf_rate_reg_default.afc_mod_reg); //4A
	rf_write_reg(AFC_SYM_ADDR, rf_rate_reg_default.afc_sym_reg); //4B
	rf_write_reg(AFC_DR_ADDR, rf_rate_reg_default.afc_dr_reg); //4C
	rf_write_reg(AFC_LIMIT1_ADDR, rf_rate_reg_default.afc_limit1_reg); //4F
	rf_write_reg(AFC_LIMIT2_ADDR, rf_rate_reg_default.afc_limit2_reg); //50
    
	/* fifo threshold registers */
	rf_write_reg(FIFO_CTRL1_ADDR, rf_threshold_reg_default.fifo_ctrl1_reg); //7C
    rf_write_reg(FIFO_CTRL2_ADDR, rf_threshold_reg_default.fifo_ctrl2_reg); //7D
    rf_write_reg(FIFO_CTRL3_ADDR, rf_threshold_reg_default.fifo_ctrl3_reg); //7E

    /* sync word registers */
    rf_write_reg(SYNC_WORD3_ADDR, rf_sync_word_reg_default.sync_word3_reg); //18
    rf_write_reg(SYNC_WORD2_ADDR, rf_sync_word_reg_default.sync_word2_reg); //19
    rf_write_reg(SYNC_WORD1_ADDR, rf_sync_word_reg_default.sync_word1_reg); //1A
    rf_write_reg(SYNC_WORD0_ADDR, rf_sync_word_reg_default.sync_word0_reg); //1B
}

void init_rf(void)
{
    for(u16 i=0; i<5000; i++); //20MHz，3ms > 2ms

    RF_DIS_INTP();

    u8 i = 0, j = 5, tst_reg = 0x00;
    do 
    {
        CLR_WDT();
        
        rf_init_reg();
        tst_reg = rf_read_reg(PA_CTRL_ADDR);

        i++; //寄存器初始化次数
            
        if(3 == i) //多次寄存器配置失败，闪灯告警，并自我关闭
        {
            tick start_time;


            EI();
            start_time = get_tick();
            
            while(j)
            {
                CLR_WDT();
                                
                if(check_timeout(start_time, KEY_LED_SWAP_PERIOD))
                {
                    KEY_LED_SWAP();
                    j--;
                    start_time = get_tick();
                }
            }
            
            SHUTDOWN(); //射频异常，系统自我关闭
        }
    }while(rf_power_reg_default.pa_ctrl_reg != tst_reg);
    
    rf_state_idle();
	rf_state_listen();

    RF_EN_INTP();
}

void rf_reset_chip(void)
{
    RF_DIS_INTP();
    
	rf_write_reg(SYSTEM1_ADDR, SRST_REG); //reset chip
	
	for(u16 i=0; i<5000; i++); //20MHz，3ms > 2ms
    
	rf_init_reg();
    rf_state_idle();
	rf_state_listen();
    
    RF_EN_INTP();
}

void rf_rssi_monitor(void)
{
    if(g_rf_param.rssi_monitor)
    {
        u8 sign, val;
        

        sign = rf_read_reg(DAGC_STA_ADDR) & BIT3;
        val = rf_read_reg(RSSI_VAL_ADDR);

        if(sign)
        {//负数
            val = ~(val - 1);
            g_rf_param.rssi = (-1) * val / 2;
        }
        else
        {
            g_rf_param.rssi = val / 2;
        }
    }
}

void rf_fdev_monitor(void)
{
    if( RF_AIR_BAUD_RATE <= 19200UL && 
        RF_STATE_RX == g_rf_param.state &&
        g_rf_param.fdev_monitor) 
    {
        u8 ber0;
        u8 prea_sync_det;
        u16 afc_val;
        
        
        ber0 = rf_read_reg(BER0_ADDR);
        afc_val = rf_read_reg(AFC_VAL_ADDR) + (((ber0 & 0x78) >> 3) << 8);
        prea_sync_det = rf_read_reg(INTP_STA2_ADDR) & 0x03;

        if(ber0 & 0x80)
        {//负数
        	afc_val = (~afc_val + 1) & 0xfff;
        }

        if(prea_sync_det & 0x02)
        {
            int sign = 1;


            if(ber0 & 0x80)
            {//负数
                sign = -1;
            }

            g_rf_param.cfo = sign * ((afc_val * RF_AIR_BAUD_RATE) / 64);
        }
    }	
}

static void rf_clear_rx_fifo(void)
{
	rf_write_reg(INTP_CTRL2_ADDR, rf_read_reg(INTP_CTRL2_ADDR) | RXFIFO_CLR); //write rxfifo_clr 1
	rf_write_reg(INTP_CTRL2_ADDR, rf_read_reg(INTP_CTRL2_ADDR) & ~RXFIFO_CLR); //write rxfifo_clr 0    
}

static void rf_clear_tx_fifo(void)
{  
	rf_write_reg(INTP_CTRL2_ADDR, rf_read_reg(INTP_CTRL2_ADDR) | TXFIFO_CLR); //write txfifo_clr 1
	rf_write_reg(INTP_CTRL2_ADDR, rf_read_reg(INTP_CTRL2_ADDR) & ~TXFIFO_CLR); //write txfifo_clr 0    
}

static void rf_disable_intp(void)
{
    rf_write_reg(INTP_CTRL1_ADDR, rf_read_reg(INTP_CTRL1_ADDR) & ~(EXT_IEN | CRCERR_IEN | RXPKT_IEN | TXPKT_IEN | RXFFAF_IEN | TXFFAE_IEN | TXFFAF_IEN | FFERR_IEN));
    rf_write_reg(INTP_CTRL2_ADDR, rf_read_reg(INTP_CTRL2_ADDR) & (RXFIFO_CLR | TXFIFO_CLR));
}

static void rf_clear_intp(void)
{
	rf_read_reg(INTP_STA1_ADDR); //clear interrupt1
	rf_read_reg(INTP_STA2_ADDR); //clear interrupt2 
}

void rf_state_shutdown(void)
{
    /* enter shutdown mode */
    RF_EXT_EN(0);
    
    g_rf_param.state = RF_STATE_SHUTDOWN;
}

void rf_state_sleep(void)
{
    /* enter sleep mode */
	rf_write_reg(SYSTEM2_ADDR, (rf_read_reg(SYSTEM2_ADDR) & ~(TX_REQ | RX_REQ | PLL_REQ | XTAL_REQ))); 

    g_rf_param.state = RF_STATE_SLEEP;
}

void rf_wake_up(void)
{
    RF_EXT_EN(1);

    rf_state_idle();
    rf_state_listen();
}

static void rf_state_ready(void)
{
    /* enter ready mode */
	rf_write_reg(SYSTEM2_ADDR, (rf_read_reg(SYSTEM2_ADDR) & ~(TX_REQ | RX_REQ | PLL_REQ)) | XTAL_REQ);

#if (RF_DEBUG_EN > 0U)
    if(!(rf_read_reg(SYSTEM2_ADDR) & XTAL_REQ))
    {
        g_rf_param.error_reg_assign_times++;
    }
#endif

    rf_clear_rx_fifo();
    rf_clear_tx_fifo();
    rf_disable_intp();
    rf_clear_intp();
	
	g_rf_param.state = RF_STATE_READY;
}

static void rf_state_tune(void)
{
    /* enter tune mode */
	rf_write_reg(SYSTEM2_ADDR, (rf_read_reg(SYSTEM2_ADDR) & ~(TX_REQ | RX_REQ)) | PLL_REQ | XTAL_REQ);

#if (RF_DEBUG_EN > 0U)
    if((PLL_REQ | XTAL_REQ) != (rf_read_reg(SYSTEM2_ADDR) & (PLL_REQ | XTAL_REQ)))
    {
        g_rf_param.error_reg_assign_times++;
    }
#endif

    rf_clear_rx_fifo();
    rf_clear_tx_fifo();
    rf_disable_intp();
    rf_clear_intp();
	
	g_rf_param.state = RF_STATE_TUNE;
}

void rf_state_idle(void)
{
    if(RF_STATE_READY == g_rf_param.idle_state)
    {
        rf_state_ready();
    }
    else if(RF_STATE_TUNE == g_rf_param.idle_state)
    {
        rf_state_tune();
    }
    else
    {
        rf_state_ready(); //默认
    }
}

void rf_state_tx(void)
{
    /* config tx mode */
	rf_write_reg(LPF_ANA1_ADDR, rf_freq_reg_default.tx.lpf_ana1_reg);
	rf_write_reg(PLL_NUMRX1_ADDR, rf_freq_reg_default.tx.pll_numrx1_reg);
	rf_write_reg(PLL_N_RX_ADDR, rf_freq_reg_default.tx.pll_n_rx_reg);

    /* enable intp */
	rf_write_reg(INTP_CTRL1_ADDR, TXPKT_IEN | TXFFAE_IEN | TXFFAF_IEN | FFERR_IEN);

    /* enter tx mode */
	rf_write_reg(SYSTEM2_ADDR, (rf_read_reg(SYSTEM2_ADDR) & ~(TX_REQ | RX_REQ)) | TX_REQ);

#if (RF_DEBUG_EN > 0U)
    if(!(rf_read_reg(SYSTEM2_ADDR) & TX_REQ))
    {
        g_rf_param.error_reg_assign_times++;
    }
#endif
    
	g_rf_param.state = RF_STATE_TX;
}

void rf_state_listen(void)
{
    /* config rx mode */
	rf_write_reg(LPF_ANA1_ADDR, rf_freq_reg_default.rx.lpf_ana1_reg);
	rf_write_reg(PLL_NUMRX1_ADDR, rf_freq_reg_default.rx.pll_numrx1_reg);
	rf_write_reg(PLL_N_RX_ADDR, rf_freq_reg_default.rx.pll_n_rx_reg);

    /* enable intp */
	rf_write_reg(INTP_CTRL1_ADDR, RXPKT_IEN | RXFFAF_IEN | FFERR_IEN);

    /* enter rx mode */
	rf_write_reg(SYSTEM2_ADDR, (rf_read_reg(SYSTEM2_ADDR) & ~(TX_REQ | RX_REQ)) | RX_REQ);

#if (RF_DEBUG_EN > 0U)
    if(!(rf_read_reg(SYSTEM2_ADDR) & RX_REQ))
    {
        g_rf_param.error_reg_assign_times++;
    }
#endif
    
	g_rf_param.state = RF_STATE_LISTEN;

    g_rf_param.last_listen_operate_time = get_tick(); //记录本次监听操作的时间
}

void rf_read_fifo(u8 len)
{
	SPI_CS_LOW();
	spi_write_byte(RF_READ_FIFO_CMD); //读fifo命令
	while(len--)
	{
		g_rf_param.rx.buf[g_rf_param.rx.index++] = spi_read_byte();

#if (RF_DEBUG_EN > 0U)			
		g_rf_param.recv_bytes++; //接收字节数统计
#endif       
	}
	SPI_CS_HIGH();
}

void rf_write_fifo(void)
{
	u8 len;
	u8 *ptr;
    

	ptr = (u8 *)g_rf_param.tx.read_index;
	len = g_rf_param.tx.len;
    
	if(len > 0)
	{
		if(len > (RF_TX_AF_THRESHOLD - 1 - RF_TX_AE_THRESHOLD))
		{
			len = RF_TX_AF_THRESHOLD - 1 - RF_TX_AE_THRESHOLD;
		}
        
		g_rf_param.tx.read_index += len;
		g_rf_param.tx.len -= len;
		
		SPI_CS_LOW();
		spi_write_byte(RF_WRITE_FIFO_CMD); //写fifo命令
		while(len--)
		{
			spi_write_byte(*ptr++); //fifo模式

#if (RF_DEBUG_EN > 0U)			
		    g_rf_param.send_bytes++; //发送字节数统计
#endif
		}
		SPI_CS_HIGH();
	}
}

void rf_send_data(u8 *buf, u8 len)
{
    if(!len || !buf) 
    {
#if (RF_DEBUG_EN > 0U)
        g_rf_param.send_nothing_times++; //空发送次数
#endif
    
        return;
    }

    if(len >= MAX_RF_SEND_BUF_LEN)
    {
#if (RF_DEBUG_EN > 0U)
        g_rf_param.send_pkt_overlength_times++; //发送包过长的包数目
#endif

        return;
    }

    if((RF_STATE_TX == g_rf_param.state) || (RF_STATE_RX == g_rf_param.state)) //射频处于发送或者接收状态
    {
#if (RF_DEBUG_EN > 0U)
        g_rf_param.send_busy_times++; //发送忙次数
#endif

        if(RF_STATE_TX == g_rf_param.state)
        {
            g_rf_param.tx.busy = TRUE; //单工，发送忙            

        }
        else
        {
            DEBUG_PRINT(("rf simplex operation!\n")); //射频单工操作
        }

        return;
    }

    TX_LED_ON(); 
    g_rf_param.tx.timeout = RF_TRANSMIT_TIMEOUT;
    g_rf_param.tx.complete = FALSE; //正在发送

	memcpy(g_rf_param.tx.buf, buf, len);
	g_rf_param.tx.read_index = g_rf_param.tx.buf;
	g_rf_param.tx.len = len;

	rf_write_reg(TXPKT_LEN_ADDR, len); //设置发射包长度 
	
	rf_state_idle();
    rf_write_fifo();
    rf_state_tx();
}

void rf_state_monitor(void)
{
    if(RF_STATE_LISTEN == g_rf_param.state &&
       check_timeout(g_rf_param.last_listen_operate_time, MAX_RF_LISTEN_OPERATE_CHECK_TIME) &&
       g_rf_param.state_monitor)
    {
    	bool rf_state_err = FALSE;
    	u8 val;
        
        
        do{
            val = rf_read_reg(SYS_STA_ADDR);
            if(FSM_STA_RX != (val & FSM_STA_MASK))
            {//rf is not in rx state
#if (RF_DEBUG_EN > 0U)
/**
 *  3'b000(0): Reset;
 *  3'b001(1): Idle;
 *  3'b010(2): Sleep;
 *  3'b011(3): Ready;
 *  3'b100(4): Tune;
 *  3'b101(5): TX;
 *  3'b110(6): RX;
 *  Others(7): Reserved;
 *
 */
                g_rf_param.reg_val = val;
                g_rf_param.error_state_times++;
#endif

            	rf_state_err = TRUE;
            	break;
            }
            
            val = rf_read_reg(SYSTEM2_ADDR);
            if((val & RX_REQ) && (val & TX_REQ))
            {//rf both set rx_req and tx_req 
#if (RF_DEBUG_EN > 0U)
                g_rf_param.reg_val = val;
                g_rf_param.error_state_times++;
#endif

            	rf_state_err = TRUE;
            	break;
            }

            val = rf_read_reg(INTP_STA2_ADDR);
            if(!(val & PREA_DET) && !(val & SYNC_DET)) //一定时间内还没有检测到前导码和同步字
            {//not detect a preable and also not detect syncword 
#if (RF_DEBUG_EN > 0U)
                g_rf_param.reg_val = val;
#endif

            	rf_state_err = TRUE;
            	break;
            }
        }while(0);
		
        if(TRUE == rf_state_err)
        {//软复位芯片
        	rf_reset_chip();

#if (RF_DEBUG_EN > 0U)
            g_rf_param.reset_chip_times++;
#endif
        }
	}    
}

void rf_afc(void)
{
    if( RF_AIR_BAUD_RATE <= 19200UL && 
        RF_STATE_LISTEN == g_rf_param.state &&
        g_rf_param.rst_dp) 
    {
        u8 ber0;
        u8 prea_sync_det;
        u16 afc_val, afc_limit;
        
        
        ber0 = rf_read_reg(BER0_ADDR);
        afc_val = rf_read_reg(AFC_VAL_ADDR) + (((ber0 & 0x78) >> 3) << 8);
        afc_limit = ((rf_read_reg(AFC_LIMIT1_ADDR) & 0x0f) << 8) + rf_read_reg(AFC_LIMIT2_ADDR);
        prea_sync_det = rf_read_reg(INTP_STA2_ADDR) & 0x03;

        if(ber0 & 0x80)
        {//负数
        	afc_val = (~afc_val + 1) & 0xfff;
        }

        if((afc_val >= afc_limit) && (!prea_sync_det))
        {
        	rf_write_reg(SYSTEM1_ADDR, rf_read_reg(SYSTEM1_ADDR) | SRST_DP);
        	rf_write_reg(SYSTEM1_ADDR, rf_read_reg(SYSTEM1_ADDR) & (~SRST_DP));

#if (RF_DEBUG_EN > 0U)
            g_rf_param.reset_dp_times++;
#endif
        }
    }	
}

void rf_work_monitor(void)
{
    rf_state_monitor();
    
    rf_afc();
}

