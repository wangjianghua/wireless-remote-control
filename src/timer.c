#include "includes.h"


tick g_app_tick;

void init_timerX(void)
{
    txmod0 = 0; //定时器模式
    txmod1 = 0;    
    txmod2 = 0;

#if 0
    txck0 = 0; //fclk = 10M，定时器X计数源选择，f1
    txck1 = 0;
    
    /* 溢出周期：1 / f * (prex+1) * (tx+1) */
    prex = 0xf9; //溢出周期1ms
    tx = 0x27;
#endif

    txck0 = 0; //fclk = 8M，定时器X计数源选择，f1
    txck1 = 0;
    
    /* 溢出周期：1 / f * (prex+1) * (tx+1) */
    prex = 0xf9; //溢出周期1ms
    tx = 0x20;

    ir_txic = 0; //清中断请求
    ilvl_txic = 2; //允许定时器溢出中断，2级优先

    txs = 1; //开始计数
}

void init_timerZ(void)
{
    tzmod0 = 0; //定时器模式
    tzmod1 = 0;    

#if 0
    tzck0 = 1; //fclk = 10M，定时器X计数源选择，f8
    tzck1 = 0;
    
    /* 溢出周期：1 / f * (prez+1) * (tzpr+1) */
    prez = 0xf9; //溢出周期20ms
    tzpr = 0x63;
#endif

    tzck0 = 1; //fclk = 8M，定时器X计数源选择，f8
    tzck1 = 0;
    
    /* 溢出周期：1 / f * (prez+1) * (tzpr+1) */
    prez = 0xf9; //溢出周期20ms
    tzpr = 0x50;

    ir_tzic = 0; //清中断请求
    ilvl_tzic = 2; //允许定时器溢出中断，2级优先

    tzs = 1; //开始计数
}

void init_timer(void)
{
    init_timerX();
    init_timerZ();
}

tick get_tick(void)						
{
	return (g_app_tick);
}

tick get_diff_tick(tick cur_tick, tick prior_tick)
{
	if(cur_tick < prior_tick)
    {
		return (cur_tick + (~prior_tick));
	}
	else
    {
		return (cur_tick - prior_tick);
	}
}

bool check_timeout(tick start_time, tick interval)
{
	return (get_diff_tick(get_tick(), start_time) >= interval);
} 

void mdelay(u16 msec)
{
 	tick old_tick;
    

	old_tick = get_tick();

	while(get_diff_tick(get_tick(), old_tick) < msec);
}

