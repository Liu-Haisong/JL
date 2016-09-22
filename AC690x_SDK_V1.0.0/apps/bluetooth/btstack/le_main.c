/*
 *********************************************************************************************************
 *                                            br16
 *                                            btstack
 *                                             CODE
 *
 *                          (c) Copyright 2016-2016, ZHUHAI JIELI
 *                                           All Rights Reserved
 *
 * File : *
 * By   : jamin.li
 * DATE : 2016-04-12 10:17AM    build this file
 *********************************************************************************************************
 */
#include "typedef.h"
#include "rtos/os_api.h"
//#include "error.h"
#include "common/msg.h"
#include "common/app_cfg.h"

extern void app_server_init(void);
extern void ble_main(void);

extern void ble_register_init_config(void *cbk);

void ble_init_config(void)
{
    ///register callback funtions
	puts("----ble_init_config\n");
	ble_register_init_config(app_server_init);
}




