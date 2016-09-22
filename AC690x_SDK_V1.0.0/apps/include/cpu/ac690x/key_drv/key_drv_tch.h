#ifndef	_TOUCH_KEY_API_H_
#define _TOUCH_KEY_API_H_

#include "includes.h"
#include "key_drv/key.h"

#if KEY_TCH_EN

#define TOUCH_KEY_CH_MAX	1//16

typedef struct _CTM_KEY_VAR
{
    s32 touch_release_buf[TOUCH_KEY_CH_MAX]; 		//按键释放值滤波器buffer
    u16 touch_cnt_buf[TOUCH_KEY_CH_MAX];			//按键计数值滤波器buffer
    s16 FLT1CFG1;					//滤波器1配置参数1
    s16 FLT1CFG2;					//滤波器1配置参数2, 等于(-RELEASECFG0)<<FLT1CFG0
    s16 PRESSCFG;					//按下判决门限
    s16 RELEASECFG0;				//释放判决门限0
    s16 RELEASECFG1;				//释放判决门限1
    s8  FLT0CFG;					//滤波器0配置参数(0/1/2/3)
    s8  FLT1CFG0;					//滤波器1配置参数0
    u16 touch_key_state;			//按键状态标志，随时可能被中断改写，按键处理程序需要将此标志复制出来再行处理
    u8  touch_init_cnt[TOUCH_KEY_CH_MAX];				//初始化计数器，非0时进行初始化
} sCTM_KEY_VAR;


#define  TOUCH_IO_TYPE   0 ///0--PORT A,1--PORT B,2--PORT C,3--PORT D

//占用PB8~PB15
#define  TCH_PORT_OUT      PORTA_OUT
#define  TCH_PORT_HD       PORTA_HD
#define  TCH_PORT_DIR      PORTA_DIR
#define  TCH_PORT_PD       PORTA_PD
#define  TCH_PORT_PU       PORTA_PU

//IO port denfine
#define  TCH_KEY0_IO_BIT   7
#define  TCH_KEY1_IO_BIT   8
#define  TCH_KEY2_IO_BIT   9
//#define  TCH_KEY3_IO_BIT   11
//#define  TCH_KEY4_IO_BIT   12
//#define  TCH_KEY5_IO_BIT   13
//#define  TCH_KEY6_IO_BIT   14



tu8 get_tchkey_value(void);
void tch_key_init(void);
void scan_capkey(void);

extern u8 touchkey_short_msg;
extern u8 g_touch_len; //触摸按键数

#endif

#endif  /*  _TOUCH_KEY_API_H_   */


