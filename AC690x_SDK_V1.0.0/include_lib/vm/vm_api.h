#ifndef _VM_API_H_
#define _VM_API_H_

#include "typedef.h"
#include "vm.h"
#include "common/flash_cfg.h"
//_VM_H_

//
enum
{
    VM_SYS_VOL = 0,
    VM_SYS_EQ,
    VM_DEV0_BREAKPOINT,
    VM_DEV1_BREAKPOINT,
    VM_DEV2_BREAKPOINT,
    VM_DEV3_BREAKPOINT,
    VM_MUSIC_DEVICE,
    VM_PC_VOL,
    VM_FM_INFO,
    MAX_VM,
    SYSCFG_BTSTACK,
    SYSCFG_FILE_BREAKPOINT,
    SYSCFG_REMOTE_DB = 14,
    SYSCFG_REMOTE_DB_END = (14+20),
    SYSCFG_REMOTE_DB_HID = 35,
    SYSCFG_REMOTE_DB_HID_END = (35+20),
};


#define VM_FILE_BK_LEN          20

#define VM_SYS_VOL_LEN          1
#define VM_SYS_EQ_LEN           1

#define VM_DEV0_BK_LEN          VM_FILE_BK_LEN///(VM_FILE_BK_LEN+8)
#define VM_DEV1_BK_LEN          VM_FILE_BK_LEN///(VM_FILE_BK_LEN+8)
#define VM_DEV2_BK_LEN          VM_FILE_BK_LEN///(VM_FILE_BK_LEN+8)
#define VM_DEV3_BK_LEN          VM_FILE_BK_LEN///(VM_FILE_BK_LEN+8)
#define VM_MUSIC_DEVICE_LEN     1

#define VM_PC_VOL_LEN           1
#define VM_FM_INFO_LEN          32


typedef struct __VM_CACHE
{
    void *buff;
    u8 index;
    u8 dat_len;
    s16 cnt;
} _VM_CACHE;

void vm_cache_start(void);
void vm_open_all(void);
s32 vm_write_api(u8 index ,const void *data_buf);
s32 vm_read_api(u8 index, void * data_buf);
vm_err vm_cache_write(u8 index ,const void *data_buf,s16 cnt);
vm_err vm_cache_submit(void);
vm_err vm_init_api(struct flash_cfg *cfg);
#endif
