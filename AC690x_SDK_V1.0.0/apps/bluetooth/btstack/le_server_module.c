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
#include "bluetooth/le_profile_def.h"
#include "bluetooth/le_server_module.h"
#include "rtos/os_api.h"
//#include "error.h"
#include "common/msg.h"
#include "common/app_cfg.h"
#include "rtos/task_manage.h"
#include "rcsp/rcsp_interface.h"
#include "rcsp/rcsp_head.h"

static u8 test_device_buf[] = {1,1,1,1};
static u8 test_buf_str[] = {8,7,7,7,7,6,6,6,6,6};

const  u8 test_send_data[] = {0x10,0x11,0x12,0x13,0x14,0x15};

static u8 gap_privacy = 0;
static u8 test_send_flag = 0;
static uint16_t gap_appearance = 0;

static uint16_t server_notify_indicate_handle;

uint16_t ble_conn_handle;
static u8 ble_mutex_flag;

#define LOOP_BUFFER_SIZE 256
u16 loop_buffer_count = 0;
u8 test_loop_buffer[LOOP_BUFFER_SIZE];

extern u32 app_notify_indicate_send(u8 *data,u16 len);


void app_server_le_msg_callback(uint16_t msg, uint8_t *buffer, uint16_t buffer_size)
{
    printf("\n----%s, p= 0x%x\n",__FUNCTION__,msg);
    switch(msg)
    {
    case BTSTACK_LE_MSG_CONNECT_COMPLETE:
        ble_conn_handle = buffer[0]+ (buffer[1]<<8);
        printf("conn_handle= 0x%04x\n",ble_conn_handle);
#if SUPPORT_APP_RCSP_EN
            rcsp_event_com_start(2);
            rcsp_register_comsend(app_notify_indicate_send);
#endif // SUPPORT_APP_RCSP_EN
        break;

    case BTSTACK_LE_MSG_DISCONNECT_COMPLETE:
        printf("disconn_handle= 0x%04x\n",buffer[0]+ (buffer[1]<<8));
        server_notify_indicate_handle = 0;

        if(ble_conn_handle != 0)
        {
            ble_conn_handle = 0;
            if(ble_mutex_flag == 0)
            {
                server_advertise_enable();
    #if SUPPORT_APP_RCSP_EN
                rcsp_event_com_stop();
    #endif // SUPPORT_APP_RCSP_EN
            }
        }

        break;

    default:
        break;
    }

    puts("exit_le_msg_callback\n");
}


// ATT Client Read Callback for Dynamic Data
// - if buffer == NULL, don't copy data, just return size of value
// - if buffer != NULL, copy data and return number bytes copied
// @param con_handle of hci le connection
// @param attribute_handle to be read
// @param offset defines start of attribute value
// @param buffer
// @param buffer_size
uint16_t app_server_read_callback(uint16_t attribute_handle, uint8_t * buffer, uint16_t buffer_size)
{
    uint16_t  payload_len = 0;

#if 0
    switch (attribute_handle)
    {
    case ATT_CHARACTERISTIC_AE02_01_VALUE_HANDLE:
        payload_len = sizeof(test_device_buf);
        if (buffer)
        {
            memcpy(buffer, test_device_buf, payload_len);
        }
        break;

    // handle device name
    // handle appearance
    default:
        break;
    }

#endif

    if(payload_len > 0)
    {
        printf("\n read_cbk: handle= 0x%04x",attribute_handle);
        put_buf(buffer,payload_len);
    }

    return payload_len;
}

// ATT Client Write Callback for Dynamic Data
// @param con_handle of hci le connection
// @param attribute_handle to be read
// @param handle_uuid
// @param buffer
// @param buffer_size
// @return 0
uint16_t app_server_write_callback(uint16_t attribute_handle, uint8_t * buffer, uint16_t buffer_size)
{

    u16 handle = attribute_handle;
    u16 copy_len;

#if 0
    if(buffer_size > 0)
    {
        printf("\n write_cbk: handle= 0x%04x",handle);
        put_buf(buffer,buffer_size);
    }
#endif

    switch (handle)
    {
    case ATT_CHARACTERISTIC_ae02_01_CLIENT_CONFIGURATION_HANDLE:
        if(buffer[0] != 0)
        {
            server_notify_indicate_handle = handle -1;
        }
        else
        {
            server_notify_indicate_handle = 0;
        }

/*
        {
            server_ccc_info_t test_info;
            server_get_ccc_handle_info(handle,&test_info);
        }
*/
        break;


    case ATT_CHARACTERISTIC_ae01_01_VALUE_HANDLE:

 ///       memcpy(test_loop_buffer,buffer,buffer_size);
 ///       loop_buffer_count = buffer_size;

#if 1
        printf("\n--write, %d\n",buffer_size);
///        put_buf(buffer,buffer_size);
#endif
//        put_buf(test_loop_buffer,loop_buffer_count);

///        server_notify_indicate_handle = 0x0c;
///        os_taskq_post_msg("btmsg",3,'Z'+ 0x1000,test_loop_buffer,loop_buffer_count);


#if SUPPORT_APP_RCSP_EN
        rcsp_comdata_recieve(buffer,buffer_size);
#endif // SUPPORT_APP_RCSP_EN

        break;

    default:
        break;
    }
    return 0;

}

u32 app_notify_indicate_send(u8 *data,u16 len)
{
    int ret_val;

	if(server_notify_indicate_handle == NULL)
	{
	    return 4;//disconn
	}

    ret_val = server_notify_indicate_send(server_notify_indicate_handle,data,len);

    if(ret_val !=  0)
    {
        puts("\n app_ntfy_fail!!!\n");
        return 4;//disconn
    }
    return 0;
}

void app_server_loop_deal(void)
{
    if(test_send_flag > 0)
    {
        app_notify_indicate_send((void*)test_send_data,sizeof(test_send_data));
        test_send_flag = 0;
    }
}

void app_server_timer_1s(void)
{
    if(server_notify_indicate_handle > 0)
    {
        ///test_send_flag = 0x55;
//        app_notify_indicate_send((void*)test_send_data,sizeof(test_send_data));
    }
}


void app_server_init(void)
{
    printf("%s\n",__FUNCTION__);
    server_register_profile_data(profile_data);
    server_register_app_callbak(app_server_le_msg_callback,app_server_read_callback,app_server_write_callback);

}

extern void client_creat_connection(void);
extern void client_disconnect(void);
extern void client_write_test_send(void);

void le_recieve_msg_deal(int *msg)
{
    printf(" %s,-- %x,%x,%x\n",__FUNCTION__,msg[0],msg[1],msg[2]);
    printf("handle = 0x%x\n",server_notify_indicate_handle);

    switch (msg[0] &0xff)
    {
    case 'C':
        ///client_creat_connection();
        break;

    case 'D':
        ///client_disconnect();
        break;

    case 'T':
       /// client_write_test_send();
        break;

    case 'Z':
        if((server_notify_indicate_handle > 0) && (msg[2] > 0))
        {
            ///test_send_flag = 0x55;
            printf("\n---n_i_send, %d\n",msg[2]);
            put_buf((void*)msg[1],msg[2]);

            app_notify_indicate_send((void*)msg[1],msg[2]);
            loop_buffer_count = 0;
        }
        break;
    default:
        break;
    }
}


void ble_enter_mutex(void)
{
	P_FUNCTION
    ble_mutex_flag = 1;
#if SUPPORT_APP_RCSP_EN
#if RCSP_BLE_MODE_FW
    if(ble_conn_handle != 0)
    {
        ble_hci_disconnect(ble_conn_handle);
        rcsp_event_com_stop();
    }
    else
    {
        server_advertise_disable();
    }
#endif // RCSP_BLE_MODE_FW
#endif
    puts("exit_enter_mu\n");
}

void ble_exit_mutex(void)
{
    P_FUNCTION
    ble_mutex_flag = 0;
#if RCSP_BLE_MODE_FW
    server_advertise_enable();
#endif // RCSP_BLE_MODE_FW
    puts("exit_exit_mu\n");
}

void ble_server_disconnect(void)
{
    P_FUNCTION
    if(ble_conn_handle != 0)
    {
        printf("server discon handle= 0x%x\n ",ble_conn_handle); 
		ble_hci_disconnect(ble_conn_handle);
    }
    puts("exit_discnt\n");
}

