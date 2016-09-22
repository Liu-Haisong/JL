#include "common/app_cfg.h"
#include "rtos/os_api.h"
#include "rtos/os_cfg.h"
#include "common/error.h"
#include "common/msg.h"
#include "rtos/task_manage.h"
#include "dac/dac_api.h"
#include "play_sel/play_sel.h"
#include "ui/led/led.h"
#include "dac/dac_api.h"
#include "dac/ladc.h"
#include "echo/echo_api.h"
#include "echo_ui.h"
#include "echo.h"


#if ECHO_EN

#define ECHO_DEF_DEEP_VAL  1024 //默认深度 0-4096
#define ECHO_DEF_VOL_VAL   128  //默认强度 0-128

void echo_switch(void **mode_reverb, u8 on)
{
    if(on)
    {
        if(*mode_reverb)//not init
        {
            //already init
            puts("already init echo mic\n");
        }
        else
        {
            //printf("%s\n",get_echo_lib_ver());
            puts("reverb_init\n");
            *mode_reverb = (void*)reverb_init(ECHO_DEF_DEEP_VAL,ECHO_DEF_VOL_VAL);
            if(*mode_reverb)
            {
                puts("init echo mic succ\n");
            }
            else
            {
                puts("init echo mic err\n");
            }
        }
    }
    else
    {
        puts("reverb_stop\n");
        reverb_stop(*mode_reverb);
        *mode_reverb = NULL;
    }
}

void echo_msg_deal(void **mode_reverb, int *msg)
{
    int flag = 1;

//    printf("echo-msg:%x\n",msg[0]);

    switch(msg[0])
    {
    /* case MSG_ECHO_PP: */
        /* if(*mode_reverb) */
        /* { */
            /* echo_switch(mode_reverb,0); */
        /* } */
        /* else */
        /* { */
            /* echo_switch(mode_reverb,1); */
        /* } */
        /* break; */

    case MSG_ECHO_START:
        if(*mode_reverb)
       	{
            puts("echo already running\n");
        }
        else
       	{
            echo_switch(mode_reverb,1);
        }

        break;

    case MSG_ECHO_STOP:
		if(*mode_reverb)
		{
			echo_switch(mode_reverb,0);
		}
		else
		{
			puts("echo not run\n");
		}

		break;

    default:
        flag = 0;
        break;
    }

    if(flag != 0)
    {
        UI_menu(MENU_REFRESH);
    }
}

void echo_exit(void **mode_reverb)
{
    reverb_stop(*mode_reverb);
    *mode_reverb = NULL;
}
#endif
