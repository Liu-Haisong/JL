/*
*********************************************************************************************************
*                                                AC46
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

#ifndef __RCSP_INTERFACE_H__
#define __RCSP_INTERFACE_H__

#include "typedef.h"
#include "common/msg.h"
#include "uart.h"


#define RDEF_MAIN_TASK_NAME        "MainTask"
#define RDEF_RTC_TASK_NAME         "RTCTask"
#define RDEF_MUSIC_TASK_NAME       "MusicTask"
#define RDEF_RECORD_TASK_NAME      "RECTask"
#define RDEF_RADIO_TASK_NAME       "FMRadioTask"
#define RDEF_LINEIN_TASK_NAME      "LineinTask"
#define RDEF_UDISK_TASK_NAME       "USBdevTask"
#define RDEF_BTMSG_TASK_NAME       "btmsg"
#define RDEF_BTSTACK_TASK_NAME     "BtStackTask"
#define RDEF_IDLE_TASK_NAME        "IdleTask"
#define RDEF_LIGHT_TASK_NAME       "LightTask"

//开关APK服务支持定义
#define RCSP_RTC_TASK      (1)
#define RCSP_MUSIC_TASK    (1)
#define RCSP_RECORD_TASK   (0)
#define RCSP_RADIO_TASK    (1)
#define RCSP_LINEIN_TASK   (1)
#define RCSP_UDISK_TASK    (1)
#define RCSP_BTSTACK_TASK  (1)

#define RCSP_TASK_NAME 			   "RCSP_Task"

#define RCSP_PC_DEBUG         0
#define RCSP_BLE_MODE_FW      1
#define RCSP_USB_MODE_OPEN    1
#define RCSP_FS_BS_DEBUG      0


///#define RCSP_BUFF_IDLE_FLAG      (0xDDEE1234)

//线程任务操作状态

#define RCSP_CBK_ERR_MASK_FLAG   0x8000

typedef enum
{
    STAUS_NONE            = 0x00,
    STAUS_STOP            = 0x01,
    STAUS_PLAY            = 0x02,
    STAUS_PAUSE           = 0x03,

    //Special status
    STAUS_SETTING_RTC     = 0x04,
    STAUS_SEARCHING       = 0x05,
    STAUS_CHANGE_DEVIECE  = 0x06,
    STAUS_PREPARE         = 0x07,
    STAUS_SAVE            = 0x08,
    STAUS_START_FM_RECORD = 0x09,
    STAUS_START_AUX_RECORD= 0x0a,
    STAUS_START_MIC_RECORD= 0x0b,
    STAUS_ALARMING        = 0x0c,
    STAUS_CLOSE_ALARM     = 0x0d,
    STAUS_DEL_OK          = 0x0e,
    STAUS_START_AUX_REC   = 0x0f,
    STAUS_NO_DEVICE       = 0x10,

    //蓝牙新增加
    STAUS_NOT_CONNECT     = 0x11,
    STAUS_PHONEING        = 0x12,

    STAUS_NODEAL          = 0xFF,
} cur_task_st_e;


//frame code type
typedef enum
{
    FRAME_TYPE_UTF16 = 0x00,
    FRAME_TYPE_ANSI,
    FRAME_TYPE_GBK,
}frame_code_type_e;

typedef enum
{
    RCSP_CBK_MSG_COMM_NULL = 0x00,
    RCSP_CBK_MSG_COMM_PREV,
    RCSP_CBK_MSG_COMM_NEXT,
    RCSP_CBK_MSG_COMM_PLAY,
    RCSP_CBK_MSG_COMM_PAUSE,
    RCSP_CBK_MSG_COMM_VOL_UP,
    RCSP_CBK_MSG_COMM_VOL_DOWN,
    RCSP_CBK_MSG_COMM_SET_VOL,
    RCSP_CBK_MSG_COMM_EQ,
    RCSP_CBK_MSG_COMM_SET_EQ,
    RCSP_CBK_MSG_COMM_SAVE_EQ,
    RCSP_CBK_MSG_COMM_RESET_EQ,
    RCSP_CBK_MSG_COMM_PLAYMODE,
    RCSP_CBK_MSG_CHANGE_MODE,

    //FRAME
    RSCP_CBK_MSG_REPORT_FRAME = 0x10,    //
    RSCP_CBK_MSG_REQUEST_FRAME,

    //BT private msg
    RCSP_CBK_MSG_BT_RES = 0x20,

    //MUSIC private msg
    RCSP_CBK_MSG_MUSIC_PREV_DEV = 0x30,
    RCSP_CBK_MSG_MUSIC_NEXT_DEV,
    RCSP_CBK_MSG_MUSIC_FF,
    RCSP_CBK_MSG_MUSIC_FR,
    RCSP_CBK_MSG_MUSIC_SET_DEV,
    RCSP_CBK_MSG_MUSIC_SET_BYNO,
    RCSP_CBK_MSG_MUSIC_DEL_CUR_FILE,

    RCSP_CBK_MSG_MUSIC_BS_SET_EXT,
    RCSP_CBK_MSG_MUSIC_BS_GET_INFO,

    //RADIO private msg
    RCSP_CBK_MSG_RADIO_SEARCH_ALL = 0x40,
    RCSP_CBK_MSG_RADIO_SEARCH_BACK,
    RCSP_CBK_MSG_RADIO_SEARCH_FORWARD,

    RCSP_CBK_MSG_RADIO_PREV_STEP,
    RCSP_CBK_MSG_RADIO_NEXT_STEP,
    RCSP_CBK_MSG_RADIO_SAVE_FREQ,
    RCSP_CBK_MSG_RADIO_DEL_CHL,
    RCSP_CBK_MSG_RADIO_SEL_CHL,
    RCSP_CBK_MSG_RADIO_SEL_FREQ,

    //LINEIN private msg
    RCSP_CBK_MSG_LINEIN_RES = 0x50,

    //USOUND private msg
    RCSP_CBK_MSG_USOUND_RES = 0x60,

    //USOUND private msg
    RCSP_CBK_MSG_LIGHT_MODE = 0x70,
    RCSP_CBK_MSG_LIGHT_BRIGHTNESS,
    RCSP_CBK_MSG_LIGHT_EFFECT,

    //NO msg
    RCSP_CBK_NO_MSG = 0xFF,

}rcsp_cbk_msg_e;

//rcsp_report_status_flag
#define RCSP_REPORT_STATUS_CONTROL       (1<<0)
#define RCSP_REPORT_VOL_VALUE            (1<<1)
#define RCSP_REPORT_EQ_VALUE             (1<<2)
#define RCSP_REPORT_START_STATE          (1<<3)
#define RCSP_REPORT_START_CONFIG         (1<<4)
#define RCSP_REPORT_EQ_CONFIG            (1<<5)

#define RCSP_REPORT_PLAYMODE_VALUE       (1<<8)
#define RCSP_REPORT_MUSIC_FILEINFO       (1<<9)
#define RCSP_REPORT_RADIO_TABLE          (1<<10)
#define RCSP_REPORT_RADIO_CHANNEL        (1<<11)
#define RCSP_REPORT_MUSIC_PLAYTIME       (1<<12)
#define RCSP_REPORT_LIGHT_INFO           (1<<13)
#define RCSP_REPORT_ID3_INFO             (1<<14)

typedef struct
{
    u8 id_str[4];
    u8 reserved[1];
    u8 code_type;
    u16 value_len;
    u8 *value_buf;
}task_frame_parm_t;


typedef u32 (*comsend_cbk_t)(u8 *data,u16 len);
typedef u32 (*task_subcmd_cbk_t)(u8 *data,u16 len);
typedef u32 (*rcsp_apptask_cbk_t)(rcsp_cbk_msg_e pkg_type,u8 *data,u16 len);

#define P_FUNCTION    //printf("@-%s \n",__FUNCTION__);

#define CHECK_MALLOC(a,b)   \
		do { \
			if((u32)(a) == 0){ \
				printf("MALLOC-FAILD, func= %s, LINE = %d\n",__FUNCTION__,__LINE__); \
				while(b); \
			} \
		}while(0)



#if SUPPORT_APP_RCSP_EN
#define Post_msg_to_rcsp_Task        Post_msg_to_rcsp_Task_do
#define Update_rcsp_Task_State       Update_rcsp_Task_State_do
#define rcsp_app_start               rcsp_apptask_start_deal_do
#define rcsp_report_status(x)        do{rcsp_com_report_flag |= x;}while(0)
#define rcsp_set_task_status         rcsp_set_task_status_do
#define rcsp_device_change           rcsp_device_change_do
#define rcsp_update_cur_device       rcsp_update_cur_device_do
#define rcsp_set_music_bs_op_api     rcsp_set_music_bs_op_api_do
#else
#define Post_msg_to_rcsp_Task(...)
#define Update_rcsp_Task_State(...)
#define rcsp_app_start(...)
#define rcsp_report_status(...)
#define rcsp_set_task_status(...)
#define rcsp_device_change(...)
#define rcsp_update_cur_device(...)
#define rcsp_set_music_bs_op_api(...)
#endif

///#define rcsp_set_event_buf_idle(buf_addr)  *(u32*)(buf_addr) = RCSP_BUFF_IDLE_FLAG


extern void rcsp_set_task_status_do(u8 status);
extern u32 rcsp_com_report_flag;
extern u8  rcsp_cur_task_status;

extern void rcsp_event_com_start(u8 type);
extern void rcsp_event_com_stop(void);
extern tbool rcsp_com_start(u8 type);
extern tbool rcsp_com_stop(void);
extern tbool rcsp_register_comsend(comsend_cbk_t cbk);
extern void  rcsp_comdata_recieve(u8 *data, u16 len);
extern void rcsp_register_support_app(u8 app_cnt, u8 *app_name_group);
extern void rcsp_set_music_bs_op_api_do(void *m_op_api);


extern void rcsp_sppdata_recieve(u8 type , u16 channel_id , u8 *data, u16 len);
extern u32 bt_spp_send_data(u8 *data,u16 len);
extern void bt_spp_disconnect(void);

extern u8 Post_msg_to_rcsp_Task_do(u32 msg, u32 param1, u32 param2);
extern u8 Update_rcsp_Task_State_do(int msg,u8 update_tag);


extern u32 rcsp_main_callback(rcsp_cbk_msg_e pkg_type,u8 *data,u16 len);
extern u32 rcsp_bt_callback(rcsp_cbk_msg_e pkg_type,u8 *data,u16 len);
extern u32 rcsp_music_callback(rcsp_cbk_msg_e pkg_type,u8 *data,u16 len);
extern u32 rcsp_radio_callback(rcsp_cbk_msg_e pkg_type,u8 *data,u16 len);
extern u32 rcsp_linein_callback(rcsp_cbk_msg_e pkg_type,u8 *data,u16 len);
extern u32 rcsp_udisk_callback(rcsp_cbk_msg_e pkg_type,u8 *data,u16 len);
extern u32 rcsp_light_callback(rcsp_cbk_msg_e pkg_type,u8 *data,u16 len);

extern void rcsp_apptask_start_deal_do(const char *name);
extern u8 rcsp_check_app_connection(void);
extern void rcsp_register_msg_cbk(void *cbk);

extern void rcsp_update_cur_device_do(u8 dev_id);
extern void rcsp_device_change_do(u8 *phydev_name);

extern void post_playfile_id3_info(void *hdl);

extern u32 rcsp_cbk_answer_apic_send(void);


#endif /* #ifndef __RCSP_INTERFACE_H__ */



