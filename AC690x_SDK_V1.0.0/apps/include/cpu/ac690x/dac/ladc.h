/*
*********************************************************************************************************
*                                             BC51
*
*                                             CODE
*
*                          (c) Copyright 2015-2016, ZHUHAI JIELI
*                                           All Rights Reserved
*
* File : *
* By   : jamin.li
* DATE : 11/11/2015 build this file
*********************************************************************************************************
*/

#ifndef _LADC_H_
#define _LADC_H_

#include "typedef.h"
#include "common/printf.h"
#include "cbuf/circular_buf.h"
#include "dac/ladc_api.h"


extern void ladc_reg_init(u8 ch_sel, u16 sr);   //ch_sel ´«²Î ENC_MIC_CHANNEL/ENC_DAC_CHANNEL/ENC_FM_CHANNEL
extern void ladc_close(u8 ch_sel);
extern void ladc_ctl_free(LADC_CTL *ladc);
extern void ladc_isr(void);
void ladc_buf_init(void);
extern void usb_mic_ladc_input(s16 *buf , u32 inlen);
#endif
