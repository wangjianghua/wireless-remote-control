#ifndef __TIMER_H__
#define __TIMER_H__

#ifdef __cplusplus
extern "C" {
#endif


extern tick g_app_tick; //Ӧ�õ�ʱ�ӽ���

void init_timer(void); //��ʼ����ʱ��
tick get_tick(void); //��ȡg_app_tick��ֵ					
bool check_timeout(tick start_time, tick interval); //����Ƿ�ʱ
void mdelay(u16 msec); //��ȷ��ʱһ����


#ifdef __cplusplus
}
#endif

#endif
