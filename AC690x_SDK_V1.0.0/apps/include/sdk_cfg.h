/*--------------------------------------------------------------------------*/
/**@file     sys_cfg.h
   @brief    系统配置
   @details
   @author
   @date    2015-9-3
   @note    ac46x
*/
/*----------------------------------------------------------------------------*/

#ifndef _CONFIG_
#define _CONFIG_

#include "includes.h"

#define BT_2_1_DEBUG    1
#define BT_4_0_DEBUG    1

#define SDK_VER  "AC461x_V100"



///任务堆栈大小配置测试
#define VM_TASK_STACK_SIZE          (1024 * 1)
#define MAIN_TASK_STACK_SIZE        (1024 * 2) //(1024 * 1)可行
#define MUSIC_TASK_STACK_SIZE       (1024 * 4)
#define MUSIC_PHY_TASK_STACK_SIZE   (1024 * 4)
#define TONE_TASK_STACK_SIZE        (1024 * 4)
#define TONE_PHY_TASK_STACK_SIZE    (1024 * 2)
#define UI_TASK_STACK_SIZE          (1024 * 2)



/*
*********************************************************************
                        DEBUG MACRO
*********************************************************************
*/
#define __DEBUG         ////<开启系统打印调试功能


/*
*********************************************************************
                        APPLICATION FUNCTION
*********************************************************************
*/


/*
*********************************************************************
                        COMMON MACRO
*********************************************************************
*/
#define EQ_EN			        1       ///<EQ模块开关
#define EQ_UART_BDG	    		0       ///<EQ uart online dbg switch
#define DAC_SOUNDTRACK_COMPOUND 0       ///<dac声道合并
#define LED_EN                  1       ///<LED指示使能
#define DAC_AUTO_MUTE_EN		1       ///<自动mute
#define SYS_LVD_EN              1       ///<电量监测
#define KEY_TONE_EN     	    1       ///<按键音
#define SYS_DEFAULT_VOL         0      ///<非0表示使用默认音量
#define UART_UPDATA_EN          0      ///<串口升级
#ifdef  MINI
    #define KEY_DOUBLE_CLICK                ///<按键双击功能
#endif

/*
*********************************************************************
                        DEVICE MACRO
*********************************************************************
*/
#ifdef MINI
    #define SDMMC0_EN       1
    #define SDMMC1_EN       0
    #define USB_DISK_EN     0
    #define USB_PC_EN       1
#else
    #define SDMMC0_EN       1
    #define SDMMC1_EN       0
    #define USB_DISK_EN     1
    #define USB_PC_EN       1
#endif

/*
*********************************************************************
                        APP MACRO
*********************************************************************
*/
#ifdef MINI
    #define SUPPORT_APP_RCSP_EN         0   ///<MINI_BT，手机app 功能暂时不支持
#else
    #define SUPPORT_APP_RCSP_EN         0   ///<标准SDK，手机app 功能暂时不支持
#endif // MINI_BT

#if SUPPORT_APP_RCSP_EN
    #define RCSP_LIGHT_HW               1   //light hardware support
#else
    #define RCSP_LIGHT_HW               0
#endif


/*
*********************************************************************
                        BT MACRO
*********************************************************************
*/
#include "bluetooth/bluetooth_api.h"

#define BT_MODE     NORMAL_MODE

#if BT_MODE==TEST_FCC_MODE
  #undef UART_UPDATA_EN
  #define UART_UPDATA_EN  0
#endif
#define BT_ANALOG_CFG   0//模拟配置
#define BT_XOSC         0//

#if SUPPORT_APP_RCSP_EN
#define BT_BACKMODE         1             ///<蓝牙后台
#else
#define BT_BACKMODE         0             ///<蓝牙后台
#endif

#if !BT_BACKMODE
    #define BT_HID_INDEPENDENT_MODE  0    ///<HID拍照的独立模式只支持非后台
#endif

#define BT_PHONE_NUMBER     1          ///<来电报号
#define AUTO_SHUT_DOWN_TIME 0          ///蓝牙无连接自动关机计时，u16类型，0表示不自动关机

/*
*********************************************************************
                       电源设置 
*********************************************************************
*/
#define    PWR_NO_CHANGE     0     //   0:  no change   
#define    PWR_LDO33         1     //   1:  LDOIN 5v -> VDDIO 3.3v -> DVDD 1.2v
#define    PWR_LDO15         2     //   2:  LDOIN 5v -> LDO   1.5v -> DVDD 1.2v, support bluetooth
#define    PWR_DCDC15        3     //   3:  LDOIN 5v -> DCDC  1.5v -> DVDD 1.2v, support bluetooth


#define PWR_MODE_SELECT    PWR_LDO15

/*
*********************************************************************
                       低功耗设置 
*********************************************************************
*/
#define    BT_OSC           0
#define    RTC_OSCH         1
#define    RTC_OSCL         2

#ifdef MINI
    #define SNIFF_MODE_CONF    0//SNIFF_EN
    #define BT_LOW_POWER_MODE  0//BT_POWER_DOWN_EN BT_POWER_OFF_EN
    #define LOWPOWER_OSC_TYPE  BT_OSC
    #define LOWPOWER_OSC_HZ    24000000L
#else
    #define SNIFF_MODE_CONF    0//SNIFF_EN
    #define BT_LOW_POWER_MODE  0//BT_POWER_DOWN_EN //BT_POWER_OFF_EN
    #define LOWPOWER_OSC_TYPE  BT_OSC//RTC_OSCL//BT_OSC
    #define LOWPOWER_OSC_HZ    24000000L//32768L//24000000L
#endif

#define D2SH_DIS_SW         0          //d2sh dis sw  rtcvdd口没有拉出来要置1
#define BLE_BREDR_MODE      BT_BREDR_EN
/*
*********************************************************************
                        MUSIC MACRO
*********************************************************************
*/
#define MUSIC_DECRYPT_EN 		1	//SMP加密文件支持
#define MUSIC_AB_RPT_EN 		1	//AB断点支持


#define DEC_TYPE_MP3_ENABLE         ///<MP3
#define DEC_TYPE_SBC_ENABLE         ///<SBC

#ifndef MINI
    #define DEC_TYPE_WAV_ENABLE     ///<3K_code_space
    #define DEC_TYPE_FLAC_ENABLE    ///<5K_code_space
    #define DEC_TYPE_APE_ENABLE     ///<8K_code_space
    #define DEC_TYPE_WMA_ENABLE     ///<30K_code_space
    // #define DEC_TYPE_F1A_ENABLE     ///<30K_code_space
#endif



/*
*********************************************************************
                        FM MACRO
*********************************************************************
*/
#ifdef MINI
    #define FM_RADIO_EN         0       ///<MINI_BT
#else
    #define FM_RADIO_EN         1       ///<标准SDK
#endif

#if FM_RADIO_EN
    #define RDA5807                 0       ///<RDA5807FM
    #define BK1080                  0       ///<BK1080FM
    #define QN8035                  0       ///<QN8035FM
    #define FM_INSIDE               1       ///<芯片内部FM
#endif

/*
*********************************************************************
                        RTC MACRO
*********************************************************************
*/
#ifdef MINI
    #define RTC_CLK_EN          0       ///<MINI_BT RTC时钟模式
#else
    #define RTC_CLK_EN          0       ///<标准SDK RTC时钟模式
#endif

#if RTC_CLK_EN
    #define RTC_ALM_EN          1       ///<RTC闹钟模式
    #define RTC_CLK_CHECK_EN    0       ///<RTC晶振检测
#endif

/*
*********************************************************************
                        ECHO MACRO
*********************************************************************
*/
#ifdef MINI
    #define ECHO_EN             0       ///<混响功能 MINI_BT
    #define ECHO_TASK_EN        0       ///<混响模式 MINI_BT
#else
    #define ECHO_EN             1       ///<混响功能 标准SDK
    #define ECHO_TASK_EN        0       ///<混响模式 标准SDK
#endif

/*
*********************************************************************
                        REC MACRO
*********************************************************************
*/
#ifdef MINI
    #define REC_EN             0    ///<MINI_BT录音功能
#else
    #define REC_EN             1    ///<标准SDK录音功能
#endif

#if SUPPORT_APP_RCSP_EN
    #undef  REC_EN
    #define REC_EN         0         ///<手机APP模式不支持录音
#endif

#if REC_EN
	#define BT_REC_EN		0       ///<蓝牙录音使能
	#define MIC_REC_EN		1       ///<MIC录音使能
	#define FM_REC_EN		1       ///<FM录音使能
	#define AUX_REC_EN		1       ///<AUX录音使能
#endif


/*
*********************************************************************
                        UI MACRO
*********************************************************************
*/

#if SUPPORT_APP_RCSP_EN
#define UI_ENABLE                    0
#else

#ifdef MINI
    #define UI_ENABLE                0     ///<UI_显示
#else
    #define UI_ENABLE                0     ///<UI_显示
#endif

#endif

#if UI_ENABLE
    ///二选一
    #define LCD_128X64_EN        0      ///<lcd 支持
    #define LED_7_EN                  1      ///<led 支持
#else
    ///都不支持
    #define LCD_128X64_EN        0
    #define LED_7_EN                   0
#endif //#if UI_ENABLE

#if LCD_128X64_EN
    #define LCD_SUPPORT_MENU     0       ///菜单显示
    #define LRC_LYRICS_EN               0       ///LRC歌词显示
#else
    ///都不支持
    #define LCD_SUPPORT_MENU     0
    #define LRC_LYRICS_EN        0
#endif//#if LCD_128X64_EN

#endif
