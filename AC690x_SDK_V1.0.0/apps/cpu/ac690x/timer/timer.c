/***********************************Jieli tech************************************************
  File : timer.c
  By   : Juntham
  date : 2014-07-04 11:21
********************************************************************************************/
#include "sdk_cfg.h"
#include "timer.h"
#include "common/jiffies.h"
#include "rtos/os_api.h"
#include "key_drv/key.h"
#include "rtos/task_manage.h"
#include "clock_api.h"
#include "rtc/rtc.h"
#include "sys_detect.h"
#include "ui/led/led.h"
#include "key_drv/key_drv_ad.h"
#include "key_drv/key_drv_ir.h"
#include "ui/led/led7_drv.h"
#include "irq_api.h"
/*
�ж��õ��ı������Զ����嵽�ṹ�壬
Ȼ��ͨ���ӿڻ�ȡ���������
*/
extern void sys_timer_schedule();
struct sys_global_var_str
{
    volatile u8 sys_halfsec;
    volatile u32 g_2ms_seed ;
};

struct sys_global_var_str g_var;

///��ʱ��0��������ʱ��Ƭ�ֻ�
__timer_handle  *timer0_hl;
u16 t0_cnt1;
u16 t0_cnt2;

/*----------------------------------------------------------------------------*/
/**@brief  get halfsec flag
   @param
   @return
   @note

 */
/*----------------------------------------------------------------------------*/
u8 get_sys_halfsec(void)
{
    return g_var.sys_halfsec;
}

/*----------------------------------------------------------------------------*/
/**@brief  get 2ms's count
   @param
   @return counkt
   @note

 */
/*----------------------------------------------------------------------------*/
u32 get_sys_2msCount(void)
{
    return g_var.g_2ms_seed;
}
void timer0_isr_callback()
{
    t0_cnt1++;
    t0_cnt2++;
    g_var.g_2ms_seed++;

    if((t0_cnt1 % 5) == 0)
    {
        OSTimeTick();
    }

    if((t0_cnt2 % 5) == 0)
    {
#if BT_4_0_DEBUG
		sys_timer_schedule();
#endif/*BT_4_0_DEBUG*/

#if RCSP_LIGHT_HW		
		light_timer_10ms();
#endif    

	}

    if(t0_cnt1 == 250)
    {
        t0_cnt1 = 0;
		puts(".");
        g_var.sys_halfsec = !g_var.sys_halfsec;//led7 driver
        os_taskq_post_msg(keymsg_task_name,1,MSG_HALF_SECOND);
    }

#if RTC_CLK_CHECK_EN
        if(OSA_CON & BIT(7))
        {
            rest_rtc_osc();
            OSA_CON |= BIT(6);
        }
#endif

	struct list_head *pos;
    __timer_handle  *timer;
	list_for_each(pos,&(timer0_hl->list))
	{
	    timer = list_entry(pos,__timer_handle,list);

   		if(++timer->cnt >= timer->time)
   		{
   			timer->cnt = 0;
   			if(timer->handler)
   			{
   		        timer->handler(timer->param);
   			}
   		}
	}
}

s32 timer0_init(void)
{
	s32 ret;
    __timer_param   timer_parm;
	timer_module_on();
	timer0_hl = timer_open(TIMER0,TIMER_MAX_ISR_FUN);
	if(NULL == timer0_hl)
	{
		printf("timer_open err");
		ret = TIMER_DRV_OPEN_ERR;
		return ret;
	}
    timer_parm.work_mode  = TIMER_WORK_MODE_COUNTER;
    timer_parm.tick_time  = 2000;//(2ms)
	ret = timer_init_api(timer0_hl,&timer_parm);

	if(ret != TIMER_NO_ERR)
	{
		printf("timer_init err = %x\n",ret);
		return ret;
	}

	ret = timer_start(timer0_hl);
	if(ret != TIMER_NO_ERR)
	{
		printf("timer_start err = %x\n",ret);
		return ret;
	}
	ret = timer_reg_isr_callback_fun(timer0_hl,1,timer0_isr_callback);
	if(ret != TIMER_NO_ERR)
	{
		printf("timer_reg_isr_callback_fun err = %x\n",ret);
		return ret;
	}
	return ret;
}

s32 timer0_clk_reset(void)
{
	s32 ret;
    __timer_param   timer_parm;
	puts("timer_clk_reset\n");

    timer_parm.work_mode  = TIMER_WORK_MODE_COUNTER;
    timer_parm.tick_time  = 2000;//(2ms)

	ret = timer_init_api(timer0_hl,&timer_parm);
	if(ret != TIMER_NO_ERR)
	{
		printf("timer_init err = %x\n",ret);
		return ret;
	}

	ret = timer_start(timer0_hl);
	if(ret != TIMER_NO_ERR)
	{
		printf("timer_start err = %x\n",ret);
		return ret;
	}

	puts("timer_init_OK\n");
	return ret;
}


