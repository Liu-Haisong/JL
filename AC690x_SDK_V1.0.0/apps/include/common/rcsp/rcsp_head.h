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

#ifndef __RCSP_HEAD_H__
#define __RCSP_HEAD_H__

#include "typedef.h"
#include "common/app_cfg.h"
#include "rtos/os_api.h"
#include "common/error.h"
//#include "dev_manage/dev_manage.h"
#include "music/music.h"
#include "common/msg.h"
#include "rtos/task_manage.h"
//#include "mango_dev_sd.h"
//#include "dac/dac_api.h"
#include "dac/dac.h"
#include "dev_manage/dev_ctl.h"
//#include "main_key.h"
#include "linein/linein.h"
#include "usb_device/usb_device.h"
#include "fm/fm_radio.h"
#include "sys_detect.h"
#include "vm/vm_api.h"
#include "file_operate/file_op.h"
//#include "resource_manage.h"
//#include "key.h"
//#include "key_voice.h"
//#include "sys_cfg.h"
#include "sdk_cfg.h"
//#include "led.h"
//#include "rtc_cfg.h"
#include "fm/fm_api.h"
#include "common/app_cfg.h"
#include "fat/tff.h"
//#include "rtc_setting.h"
//#include "music_ui.h"

//#include "file_bs_deal.h"

//#include "rtc.h"
//#include "timer.h"
#include "common/common.h"

#include "rcsp/rcsp_interface.h"
#include "light_head.h"

#define  RCSP_APP_DEBUG               //打开模块调试流程打印

#ifdef  RCSP_APP_DEBUG

#define rcsp_err_puts         puts
#define rcsp_err_printf       printf
#define rcsp_put_buf          put_buf
#define rcsp_db_puts          puts
#define rcsp_db_printf        printf
#define rcsp_tag_puts          puts
#define rcsp_tag_printf        printf

#else

//#define rcsp_err_puts(...)
//#define rcsp_err_printf(...)
//#define rcsp_tag_puts(...)
//#define rcsp_tag_printf(...)

#define rcsp_err_puts          puts
#define rcsp_err_printf        printf
#define rcsp_tag_puts          puts
#define rcsp_tag_printf        printf


#define rcsp_put_buf(...)
#define rcsp_db_puts(...)
#define rcsp_db_printf(...)

#endif /* #ifdef RCSP_APP_DEBUG */


#define DEVICE_UDISK                    0
#define DEVICE_SDMMC                    1
#define DEVICE_NONE                     0xFF



#define RCSP_SYS_EQ_SL                    0x3f
#define RCSP_SYS_PLAYMODE_SL              0x1f

#define RCSP_SEND_TMP_BUF_SIZE            (128)


#if 1//EQ_EN
#define RCSP_EQ_EN  1
#else
#define RCSP_EQ_EN  0
#endif

//CBK操作结果状态
typedef enum
{
    RCSP_CBK_ERR_NONE      = 0x00,
    RCSP_CBK_ERR_OPT,
    RCSP_CBK_ERR_RESS,
    RCSP_CBK_ERR_CRC,
    RCSP_CBK_ERR_TASK_MODE,
    RCSP_CBK_ERR_SYS_BUSY,
    RCSP_CBK_ERR_NOACK,

    //1~0x0f,sdk内部使用
    //--------------------------
    RCSP_CBK_BT_ERR_RES    = 0x10,

    RCSP_CBK_MUSIC_ERR_NO_DEV = 0x20,
    RCSP_CBK_MUSIC_ERR_NO_APIC = 0x21,
    RCSP_CBK_MUSIC_ERR_READ_DEV = 0x22,
    RCSP_CBK_MUSIC_ERR_FS_BS = 0x23,

    RCSP_RCSP_OPT_REJECT_ERR = 0x30,


}cbk_opt_status_e;

#define RCSP_MSG_DEAL_REPORT_FRAME       (1<<0)
#define RCSP_MSG_DEAL_CTL_STATE          (1<<1)
#define RCSP_MSG_DEAL_CHANGE_MODE        (1<<2)
#define RCSP_MSG_DEAL_LIGHT_STATE        (1<<3)
#define RCSP_MSG_DEAL_LONG_FRAME         (1<<4)
#define RCSP_MSG_DEAL_GET_ID3            (1<<5)

#define READ_BIG_U16(a)   ((*((u8*)(a)) <<8)  + *((u8*)(a)+1))
#define READ_BIG_U32(a)   ((*((u8*)(a)) <<24) + (*((u8*)(a)+1)<<16) + (*((u8*)(a)+2)<<8) + *((u8*)(a)+3))

#define READ_LIT_U16(a)   (*((u8*)(a))  + (*((u8*)(a)+1)<<8))
#define READ_LIT_U32(a)   (*((u8*)(a))  + (*((u8*)(a)+1)<<8) + (*((u8*)(a)+2)<<16) + (*((u8*)(a)+3)<<24))


#define WRITE_BIG_U16(a,src)   {*((u8*)(a)+0) = (u8)(src>>8); *((u8*)(a)+1) = (u8)(src&0xff); }
#define WRITE_BIG_U32(a,src)   {*((u8*)(a)+0) = (u8)((src)>>24);  *((u8*)(a)+1) = (u8)(((src)>>16)&0xff);*((u8*)(a)+2) = (u8)(((src)>>8)&0xff);*((u8*)(a)+3) = (u8)((src)&0xff);}

#define WRITE_LIT_U16(a,src)   {*((u8*)(a)+1) = (u8)(src>>8); *((u8*)(a)+0) = (u8)(src&0xff); }
#define WRITE_LIT_U32(a,src)   {*((u8*)(a)+3) = (u8)((src)>>24);  *((u8*)(a)+2) = (u8)(((src)>>16)&0xff);*((u8*)(a)+1) = (u8)(((src)>>8)&0xff);*((u8*)(a)+0) = (u8)((src)&0xff);}


extern u8 curr_device;
extern u8 play_mode;
extern u8 eq_mode;
extern DAC_CTL dac_ctl;
extern FM_MODE_VAR *fm_mode_var;
extern FM_INFO *fm_info;

extern u8 *rcsp_send_BigData_buf;
extern u8 *rcsp_send_tmp_buf;
extern u8 usb_spk_vol_value;
extern u8 getting_music_info;

extern const int eq_seg_gain[6][10];
extern s8 user_eq_seg_gain[6][10];
extern const s8 user_eq_seg_gain_default[6][10];
extern const u8 rcsp_vol_max_value;
extern u8  rcsp_eq_sw_flag;
extern u32 play_file_clust;


extern u32 file_operate_get_total_phydev();
extern void get_eq_table_value(s8 *eq_buf);

extern u32 get_decode_time(void);
extern void packet_frame_data(task_frame_parm_t *frame_p,u8 *value_buf_addr,char *id_str,u8 code_type,u8 *data,u16 len);

extern void light_control_set(void *set_param,u32 set_mode);
extern u8 get_bt_eq_mode(void);
extern void rcsp_fw_disconnect(void);

#endif /* #ifndef __RCSP_HEAD_H__ */





