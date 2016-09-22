#ifndef _ENCHO_DEAL_H_
#define _ENCHO_DEAL_H_

#include "typedef.h"
#include "string.h"
#include "mem/malloc.h"
#include "common/app_cfg.h"
#include "rtos/os_api.h"
#include "rtos/os_cfg.h"
#include "common/error.h"
#include "dac/ladc_api.h"
#include "dac/dac_api.h"
#include "reverb_api.h"

#define ECHO_RUN_NAME   "ECHO_RUN"

///echo spec define src
#define ECHO_11K_SR    10500
#define ECHO_12K_SR    11424
#define ECHO_08K_SR    7616

#define ECHO_22K_SR    21000
#define ECHO_24K_SR    22848
#define ECHO_16K_SR    15232

#define ECHO_44K_SR    42000
#define ECHO_48K_SR    45696
#define ECHO_32K_SR    30464

///normal src
#define NOR_11K_SR    11025
#define NOR_12K_SR    12000 
#define NOR_08K_SR    8000 

#define NOR_22K_SR    22050 
#define NOR_24K_SR    24000 
#define NOR_16K_SR    16000 

#define NOR_44K_SR    44100 
#define NOR_48K_SR    48000 
#define NOR_32K_SR    32000 

#define REVERB_SR 	NOR_16K_SR

typedef struct _REVERB_API_STRUCT_
{
    REVERB_PARM_STRUCT *reverb_parm_obj;  //参数
    unsigned char *ptr;                   //运算buf指针
    REVERB_FUNC_API *func_api;            //函数指针
    REVERB_IO *func_io;                   //输出函数接口
    LADC_CTL * ladc_p;
}REVERB_API_STRUCT;


extern void reverb_stop(void *reverb_api_obj);
extern REVERB_FUNC_API *get_reverb_func_api(void);

#if ECHO_PARM
void set_reverb_parm(void *reverb_obj, u16 sr, u16 pre_delay_ms);
#else
void set_reverb_parm(void *reverb_obj, u16 deep_val, u16 vol_val);
#endif

#endif
