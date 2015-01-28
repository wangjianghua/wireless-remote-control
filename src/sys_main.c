/** 
 * @file     main.c
 * @brief    无线钥匙 
 * @details  无线钥匙，主控：R5F211B4SP，射频：MH1801
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


static const EVENT_PROC_PTR _sys_evt_proc[] =
{
    on_tick,
    on_second,
    on_rf_intp,
    on_uart_recv,
    on_rf_send,
    on_key,
    on_user
};

void  main(void)
{
    _sys_pos_t pos;
    _sys_event_t test_event_mask;


    /***********/
    init_port();
    /***********/
    
    _variable_init();
    _device_init();

test_evt:
    test_event_mask = 1;
    for( pos=0; pos<SYS_EVENT_MAX; pos++ )
    {
        if( test_event_mask & SYS_EVT_VAR )
        {
            SYS_EVT_PROC(pos);
            sys_evt_del(test_event_mask);
            clr_wdt();
            goto test_evt;
        }
        
        if( ( SYS_EVT_VAR & SYS_EVENT_MASK ) == 0 )
        {
#if (LOW_POWER_TEST_EN > 0U)

#endif

            break;
        }
        
        test_event_mask <<= 1;
    }
    goto test_evt;
}

