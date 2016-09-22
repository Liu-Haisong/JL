/*
 *********************************************************************************************************
 *                                                BC51
 *                                      Remote Control System Protocol (RCSP)
 *                                             CODE
 *
 *                          (c) Copyright 2015-2016, ZHUHAI JIELI
 *                                           All Rights Reserved
 *
 * File : *
 * By   : jamin.li
 * DATE : 8/20/2015 build this file
 *********************************************************************************************************
 */
#include "rcsp/light_head.h"
#include "rtos/os_api.h"
#include "rcsp/rcsp_head.h"

#if SUPPORT_APP_RCSP_EN

light_info_t  light_info_tab;
light_disp_t  *light_sence_table;

extern void bt_led_init(void);

void light_init(void)
{
    u32 err;

    P_FUNCTION

    memset(&light_info_tab,0x00,sizeof(light_info_t));
    light_info_tab.color_value[LGT_COLOR_BRIGHT] = 20;//bright

#if RCSP_LIGHT_HW	
    bt_led_init();
#endif

    light_info_tab.button = 1;
    light_info_tab.mode = LGT_MD_WHITE;
    light_info_tab.color_value[LGT_COLOR_WHITE] = 255;
    light_control_set(&light_info_tab,0);
    light_control_set(&light_info_tab,1);

}


void light_exit(void)
{
    P_FUNCTION
    ;
}


#endif //#if





