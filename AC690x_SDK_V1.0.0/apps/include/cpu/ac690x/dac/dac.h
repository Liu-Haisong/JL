/*******************************************************************************************
 File Name: dac.h

 Version: 1.00

 Discription:


 Author:yulin deng

 Email :flowingfeeze@163.com

 Date:2014-01-13 17:09:41

 Copyright:(c)JIELI  2011  @ , All Rights Reserved.
*******************************************************************************************/
#ifndef _DAC_H_
#define _DAC_H_

#include "typedef.h"

typedef struct __DAC_CTL
{
	u8 sys_vol_l;
	u8 sys_vol_r;
	u8 *dac_dma_buf;
	u8 *dac_cbuf;
	u32 dma_buf_len;
	u32 cbuf_len;
}DAC_CTL;
extern DAC_CTL dac_ctl;

void audio_init(void);
void dac_on_control();
void dac_off_control();

#endif
