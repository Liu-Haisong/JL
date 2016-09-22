#include "sdk_cfg.h"
#include "includes.h"
#include "main_key.h"
#include "rtos/os_api.h"
#include "common/app_cfg.h"
#include "dev_manage/dev_ctl.h"
#include "file_operate/file_op_err.h"
#include "rtos/task_manage.h"
#include "usb_device.h"
#include "key_drv/key.h"
#include "common/flash_cfg.h"
#include "vm/vm_api.h"
#include "ui/ui_api.h"
#include "rcsp/rcsp_interface.h"
#include "dac/dac_api.h"
#include "linein.h"
#include "sys_detect.h"
#include "bluetooth/avctp_user.h"
#include "key_drv/key_drv_io.h"
#include "ui/led/led.h"
#include "key_drv/key_voice.h"
#include "dac/dac.h"
#include "rtc/rtc_api.h"
#include "update.h"

extern tbool mutex_resource_apply(char *resource,int prio ,void (*apply_response)(), void (*release_request)());
extern tbool mutex_resource_release(char *resource);
extern void resourse_manage_init(void);

extern u32 lg_dev_status_chg(u32 status,void *parm,u32 lgdevnum);
extern u32 lg_dev_mount(void *parm,u8 first_let,u32 total_lgdev);
extern u8 *dev_get_phydev_name(void *drv_it);


extern void set_poweroff_wakeup_io_handle_register(void (*poweroff_wakeup_io)());

extern volatile u8 new_lg_dev_let;

extern void ble_init_config(void);
extern void light_init(void);



void set_poweroff_wakeup_io()
{
	 soft_poweroff_wakeup_io(WAKE_UP_PR1,FALL_EDGE_PR1); 
}
/*----------------------------------------------------------------------------*/
/**@brief  main task
   @param
   @return
   @note   task always running,can't delete

 */
/*----------------------------------------------------------------------------*/

extern void ram1_fast_run_standby(void);
extern void call_main_deal();
extern u32 test_power_off(u32 usec);
extern void test_power_off_init(void);
extern u32 pc_limh;
extern u32 pc_liml;


extern void printf_test(void);
static void TaskMain(void *p)
{
    int msg[6];
    u32 status;
    u32 bt_status=0;
    curr_task = NULL;
    soft_poweroff_cnt = 0;
    going_to_pwr_off = 0;
#if(BT_MODE!=NORMAL_MODE)
    bd_work_mode = BD_PAGE_MODE;
#else
    bd_work_mode = BD_CONN_MODE;
#endif
	puts("****TaskMain********\n");

	update_deal(UPDATE_OK_WHILE);
	set_poweroff_wakeup_io_handle_register(set_poweroff_wakeup_io);

#if 0
	    #include "iic.h"
	    eeprom_verify();
		while(1); 
#endif

    /*soft_power_ctl*/
#if 0
  #if 0 //soft power ctl by mos transistor
    soft_power_ctl(PWR_OFF);
    while(1)
    {
        os_taskq_pend(0, ARRAY_SIZE(msg), msg);
        if(msg[0] == MSG_POWER_ON)
        {	
            puts("\n****************power_on***************\n");
            soft_power_ctl(PWR_ON);
            break;
        }
    }
  #else
	while(1)
	{	
	    memset(msg,0x00,sizeof(msg));
		os_taskq_pend(0, ARRAY_SIZE(msg), msg);
		if(msg[0] != 0xa9)
		{
			printf("key:%x\n",msg[0]);
		}
	    if(msg[0] == MSG_POWER_ON)
		{	 
			puts("****************power_on***************\n");
	        soft_poweroff_cnt = 0;
            soft_power_ctl(PWR_ON);
	    }
		else if(msg[0] == MSG_POWER_OFF)
		{	   
			puts("****************power_off***************\n");
		    soft_power_ctl(PWR_OFF);
		}
	}
#endif		
#endif		

    led_init();
    JL_SYSTEM->LVD_CON |= BIT(2);
#if DAC_AUTO_MUTE_EN
	digit_auto_mute_set(AUTO_MUTE_ENABLE, 4, 1200, 200);
#endif
#if BT_2_1_DEBUG
    resourse_manage_init();
#endif
    vm_cache_start();
    audio_init();
    dev_ctrl_sys_init(0);

#if RCSP_LIGHT_HW    
	light_init();
#endif

    while(1)
    {
        memset(msg,0x00,sizeof(msg));
        os_taskq_pend(0, ARRAY_SIZE(msg), msg);
        clear_wdt();

        if(msg[0] != MSG_HALF_SECOND)
        {
            printf("\nmain_msg= 0x%x\n",msg[0]);
        }

        //printf("main_msg %08x %08x \n",msg[0],msg[1]);
        switch(msg[0])
        {
        case SYS_EVENT_LGDEV_ONLINE:
            printf("LGDEV ONLINE--%08x-%08x\n",msg[1],msg[2]);

            status=lg_dev_status_chg(SYS_EVENT_LGDEV_ONLINE,(void *)msg[1],msg[2]);
            if((status!=FILE_OP_ERR_LGDEV_FULL)/*&&(status!='A')*/)
            {
                if(((char)status == 'A'))       ///< SPIFLASH璁惧mount
                {
                    puts("up\n");
                    lg_dev_mount((void *)msg[1],status,msg[2]);
                    puts("power up\n");

#if UI_ENABLE
                    ui_init_api();///必须挂载flash后才能初始化LCD显示
#endif

#ifdef  MINI
                    task_switch(BTSTACK_TASK_NAME, 0, SWITCH_SPEC_TASK);

#else
					puts("*************Select Task***************\n");
                   task_switch(BTSTACK_TASK_NAME, 0, SWITCH_SPEC_TASK);
                    //task_switch(LINEIN_TASK_NAME, 0, SWITCH_SPEC_TASK);
//                    task_switch(RTC_TASK_NAME, 0,SWITCH_SPEC_TASK);
#endif // MINI_BT

                    os_taskq_post_event((char *)"CheckTask", 1, SYS_EVENT_LGDEV_ONLINE);
                    break;
                }
                else
                {
                    printf("\n---------new phydev name: %s ---------\n",dev_get_phydev_name((void *)msg[1]));
                    rcsp_device_change(dev_get_phydev_name((void *)msg[1]));
                    new_lg_dev_let=status;
                }
#ifdef POWERUP_DEV_IGNORE
                if(!msg[3])
                    break;
#endif

                if((os_time_get() > 200) && (!bt_status))
                {
                    if(compare_task_name(UDISK_TASK_NAME)) ///<PC Audio
                        break;
                    if((task_switch(MUSIC_TASK_NAME, 0,SWITCH_SPEC_TASK)==RUN_TASK_ERR_HAS_RPT))
                    {
                        printf("music has run...\n");
                        os_taskq_post_event((char *)MUSIC_TASK_NAME, 2, SYS_EVENT_DEV_ONLINE,status);
                    }
                }
            }
            break;

        case SYS_EVENT_LGDEV_OFFLINE:
            printf("------dev offline ---\n");
            os_taskq_post_event((char *)keymsg_task_name, 2, SYS_EVENT_DEV_OFFLINE,msg[1]);
            lg_dev_status_chg(SYS_EVENT_LGDEV_OFFLINE,(void *)msg[1],1);
            break;

		case SYS_EVENT_AUX_IN:
			puts("SYS_EVENT_AUX_IN\n");
			if(os_time_get() > 200)
				task_switch(LINEIN_TASK_NAME,0,SWITCH_SPEC_TASK);
            break;

		case SYS_EVENT_AUX_OUT:
			puts("SYS_EVENT_AUX_OUT\n");
            if(compare_task_name(LINEIN_TASK_NAME))
                task_switch(0,0,SWITCH_NEXT_TASK);
			break;

        case SYS_EVENT_PC_IN:
            puts("SYS_EVENT_PC_IN\n");
			if(os_time_get() > 200)
            	task_switch(PC_TASK_NAME, 0, SWITCH_SPEC_TASK);
            break;

        case SYS_EVENT_PC_OUT:
            puts("SYS_EVENT_PC_OUT\n");
            if(compare_task_name(UDISK_TASK_NAME))
                task_switch(0, 0, SWITCH_NEXT_TASK);
            break;

#if RTC_ALM_EN
        case MSG_ALM_ON:
            puts("\n\n\n\n------MSG_ALM_ON\n");
            task_switch("RTCTask",0,SWITCH_SPEC_TASK);
//            Post_msg_to_rcsp_Task(MSG_RSCP_CHANGE_WORDMODE_ACTION,0,0);
            break;
#endif
        case MSG_CHANGE_WORKMODE:
            puts("MSG_CHANGE_WORKMODE\n");
            task_switch(0,0,SWITCH_NEXT_TASK);
//            Post_msg_to_rcsp_Task(MSG_RSCP_CHANGE_WORDMODE_ACTION,0,0);
            break;

        case SYS_EVENT_TASK_RUN_REQ:
            task_switch((char *)msg[1], 0,SWITCH_SPEC_TASK);
//            Post_msg_to_rcsp_Task(MSG_RSCP_CHANGE_WORDMODE_ACTION,0,0);
            break;


        case SYS_EVENT_DEV_BEGIN_MOUNT:
            if(compare_task_name(MUSIC_TASK_NAME))
            {
                os_taskq_post_event((char *)MUSIC_TASK_NAME, 1, SYS_EVENT_DEV_BEGIN_MOUNT);
            }
            break;
		case SYS_EVENT_DEV_MOUNT_ERR:
			puts("SYS_EVENT_DEV_MOUNT_ERR\n");
            if(compare_task_name(MUSIC_TASK_NAME))
            {
                os_taskq_post_event((char *)MUSIC_TASK_NAME, 1, SYS_EVENT_DEV_MOUNT_ERR);
            }
			break;

        case MSG_VOL_DOWN:
            if(dac_ctl.sys_vol_l)
                dac_ctl.sys_vol_l--;
			user_send_cmd_prepare(USER_CTRL_AVCTP_OPID_SEND_VOL,0,NULL);
            printf("V- = %d\n",dac_ctl.sys_vol_l);
            dac_ctl.sys_vol_r = dac_ctl.sys_vol_l;
            if(is_dac_mute()) {
                dac_mute(0,1);
            }
            set_sys_vol(dac_ctl.sys_vol_l, dac_ctl.sys_vol_r, FADE_OFF);
#if (SYS_DEFAULT_VOL==0)
            vm_cache_write(VM_SYS_VOL,&dac_ctl.sys_vol_l,2);
#endif
            UI_menu_arg(MENU_MAIN_VOL,0);
			
			rcsp_report_status(RCSP_REPORT_VOL_VALUE);
            break;

        case MSG_VOL_UP:
		
			if(msg[1]&0x80)
            {
				dac_ctl.sys_vol_l = (u8)(msg[1]&0x7f);
                printf("set_vol=  %d\n",dac_ctl.sys_vol_l);
                if(dac_ctl.sys_vol_l > MAX_SYS_VOL_L)
                dac_ctl.sys_vol_l = MAX_SYS_VOL_L;
            }
            else
            {
                if(dac_ctl.sys_vol_l < MAX_SYS_VOL_L)
                	dac_ctl.sys_vol_l++;
				user_send_cmd_prepare(USER_CTRL_AVCTP_OPID_SEND_VOL,0,NULL);
			}
		
            
#if KEY_TONE_EN
             if(dac_ctl.sys_vol_l == MAX_SYS_VOL_L)
                 send_key_voice(500);
#endif
            printf("V+ = %d\n",dac_ctl.sys_vol_l);
            dac_ctl.sys_vol_r = dac_ctl.sys_vol_l;
            if(is_dac_mute()) {
                dac_mute(0,1);
                //led_fre_set(15,0);
            }
            set_sys_vol(dac_ctl.sys_vol_l, dac_ctl.sys_vol_r, FADE_OFF);
#if (SYS_DEFAULT_VOL==0)
            vm_cache_write(VM_SYS_VOL,&dac_ctl.sys_vol_l,2);
#endif
            UI_menu_arg(MENU_MAIN_VOL,0);
			
			rcsp_report_status(RCSP_REPORT_VOL_VALUE);
            break;

        case MSG_MUTE:
			puts("MSG_MUTE\n");
            if(is_dac_mute()) {
                dac_mute(0,1);
            }
			else {
                dac_mute(1,1);
            }
            break;
        
        case MSG_LOW_POWER:
            puts("**MSG_LOW_POWER,auto shutdown**\n");
            /*
                You can add your process flow here when low-voltage.
                e.g. Auto shutdown.
            */
			dac_off_control();
            soft_power_ctl(PWR_OFF);
            break;

        case MSG_POWER_OFF:
            puts("\n*************power_off******************\n");

    		if(keymsg_task_name)
   			{
                /* os_taskq_post_msg(keymsg_task_name, 1, MSG_PROMPT_PLAY); */
    		}

			dac_off_control();
            going_to_pwr_off = 1;
            os_taskq_post(MAIN_TASK_NAME,1,MSG_POWER_OFF_HOLD);
            break;
        case MSG_POWER_OFF_HOLD:
            puts("MSG_POWER_OFF_HOLD\n");
            if(going_to_pwr_off)
                soft_poweroff_cnt = 2;//wait power_off key up
            break;
        case MSG_POWER_KEY_UP:
            puts("MSG_POWER_KEY_UP\n");
            if(going_to_pwr_off)
                soft_power_ctl(PWR_OFF);
            break;
        case MSG_UPDATA:
			if(msg[1])
			{
	           dev_updata_mode(NULL,BT_UPDATA);
			}
			else
			{
			    dev_updata_mode(0,UART_UPDATA);
			}
			break;
        case MSG_0:
        case MSG_1:
        case MSG_2:
        case MSG_3:
        case MSG_4:
        case MSG_5:
        case MSG_6:
        case MSG_7:
        case MSG_8:
        case MSG_9:

            input_number = input_number * 10 + msg[0];
            input_number_cnt++;
            if ((input_number > 9999) ||(input_number_cnt>4))
            {
                input_number = msg[0];
                input_number_cnt= 1;
            }
            printf("input_number = %u   \n",input_number);

#if UI_ENABLE
            UI_menu_arg(MENU_INPUT_NUMBER,input_number|(input_number_cnt<<24));
#endif
            break;

#if LCD_SUPPORT_MENU
        case MSG_ENTER_MENUMAIN:
            UI_menu_arg(MENU_LIST_DISPLAY,UI_MENU_LIST_MAIN);
            break;
#endif
		case SYS_EVENT_SYS_TIMER_DELAY:
			break;
        //case MSG_HALF_SECOND:
           // puts("half-sec \n");
            //break;
        }
    }
}

/*----------------------------------------------------------------------------*/
/**@brief  创建main task
   @param
   @return
   @note
*/
/*----------------------------------------------------------------------------*/
int main_task_init(void)
{
    u32 err;
    puts("create main task\n");
    err = os_task_create_ext(TaskMain,
                             (void*)0,
                             TaskMainPrio,
                             10
                             ,MAIN_TASK_NAME,
                             MAIN_TASK_STACK_SIZE
                            );

    if(OS_NO_ERR == err)
    {
        key_msg_register(MAIN_TASK_NAME, main_ad_table, main_io_table, main_ir_table, NULL);
    }
    else
    {
        printf("main task err = %08x \n",err);
    }

    return 0;
}
