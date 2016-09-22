#ifndef __KEY_DRV_IO_H__
#define __KEY_DRV_IO_H__

#include "sdk_cfg.h"
#include "sys_detect.h"
#include "rtc/rtc_api.h"

#if KEY_IO_EN

#ifdef MINI
    #define IS_KEY0_DOWN()    (!PORTR_IN(PORTR1))  //PP PR0
    #define IS_KEY1_DOWN()    (!(JL_PORTA->IN&BIT(1))) //V+
    #define IS_KEY2_DOWN()    0                    //V-
    #define IS_KEY3_DOWN()    0
    #define KEY_INIT()        do{PORTR_PU(PORTR1,1);PORTR_PD(PORTR1,0); JL_PORTA->PU |= BIT(1);JL_PORTA->PD &= ~BIT(1);\
                                 PORTR_DIR(PORTR1,1);JL_PORTA->DIR |= BIT(1);}while(0)
    //set usb_io to key_io
//    #define IS_KEY0_DOWN()    (!PORTR_IN(PORTR1))  //PP PR0
//    #define IS_KEY1_DOWN()    (!(JL_PORTA->IN&BIT(1))) //V+
//    #define IS_KEY2_DOWN()    (!USB_DM_IN())       //V-
//    #define IS_KEY3_DOWN()    0
//    #define KEY_INIT()        do{PORTR_PU(PORTR1,1);PORTR_PD(PORTR1,0); JL_PORTA->PU |= BIT(1);JL_PORTA->PD &= ~BIT(1);USB_DM_PU(1);\
//                                 USB_DM_PD(0);PORTR_DIR(PORTR1,1);JL_PORTA->DIR |= BIT(1);USB_DM_DIR(1);}while(0)
#else
    #define IS_KEY0_DOWN()    (!(JL_PORTB->IN&BIT(8)))
    #define IS_KEY1_DOWN()    (!(JL_PORTB->IN&BIT(9)))
    #define IS_KEY2_DOWN()    (!(JL_PORTB->IN&BIT(10)))
    #define IS_KEY3_DOWN()    0
    //#define KEY1_OUT_L()      do{PORTA_DIR &= ~BIT(13);PORTA_OUT &= ~BIT(13);}while(0)
    #define KEY_INIT()        do{JL_PORTB->PU |= BIT(8)|BIT(9)|BIT(10);JL_PORTB->DIR |= BIT(8)|BIT(9)|BIT(10);}while(0)
#endif/*MINI_BT*/


tu8 get_iokey_value(void);
extern void io_key_init(void);
extern  u8  io_key_get_value(void);
extern  u8  io_key_pressed_to_msg(u8 key_style, u8 key_num);

#endif/*KEY_IO_EN*/
#endif/*__KEY_DRV_IO_H__*/
