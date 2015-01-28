#ifndef __WDT_H__
#define __WDT_H__

#ifdef __cplusplus
extern "C" {
#endif


void init_wdt(void); //初始化看门狗
void restart_wdt(void); //喂狗


#ifdef __cplusplus
}
#endif

#endif
