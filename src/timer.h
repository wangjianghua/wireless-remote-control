#ifndef __TIMER_H__
#define __TIMER_H__

#ifdef __cplusplus
extern "C" {
#endif


extern tick g_app_tick; //应用的时钟节拍

void init_timer(void); //初始化定时器
tick get_tick(void); //获取g_app_tick的值					
bool check_timeout(tick start_time, tick interval); //检测是否超时
void mdelay(u16 msec); //精确延时一毫秒


#ifdef __cplusplus
}
#endif

#endif
