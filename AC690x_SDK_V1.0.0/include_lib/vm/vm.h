#ifndef _VM_H_
#define _VM_H_

#include "typedef.h"
#include "peripherals/winbond_flash.h"
//
//#ifndef  AT_VM
//#include "mango_dev_spi0flash.h"
//#endif



typedef enum _vm_err
{
    VM_ERR_NONE = 0,
    VM_INDEX_ERR,
    VM_INDEX_EXIST,
    VM_READ_NO_INDEX,
    VM_READ_DATA_ERR,
    VM_WRITE_OVERFLOW,
    VM_ERR_INIT,
    VM_ERR_PROTECT
}vm_err;

typedef u8 vm_hdl;

/*格式化VM_区域*/
extern void vm_eraser(void);

/*
每次上电初始化VM系统,
need_defrag_when_write_full: 当vm区域写满时是否需要整理碎片
*/
extern void vm_run_mode_and_keyt_init(u8 mode);
extern vm_err vm_init(u32 vm_addr, u32 spi_base, u8 mode, bool need_defrag_when_write_full);

/*
在 vm_write,vm_read 前必须先打开 指定index 获得句柄,
允许多次打开: vm_read/vm_write(vm_open(index,data_len),data_buf)
*/
extern vm_hdl vm_open(u8 index, u8 data_len);

/*不再需用调用vm_write,vm_read 时可以关闭句柄释放内存*/
extern vm_err vm_close(vm_hdl hdl);

/*读写函数，返回正数表示实际读写长度，返回负数表示错误*/
extern s32 vm_write(vm_hdl hdl, const void *data_buf);
extern s32 vm_read(vm_hdl hdl, void * data_buf);

/*手动整理碎片*/
extern vm_err vm_defrag(void);

/*侦查 vm 区域 使用情况, 返回使用的百分比*/
extern int vm_status(void);

extern const struct DEV_IO * dev_reg_vm(void *parm);


bool sfc_erase(FLASH_ERASER cmd, u32 addr);
u32 sfc_write(const u8 *buf, u32 adr, u32 len);
u32 sfc_read(u8 *buf, u32 adr, u32 len);

#endif  //_VM_H_



