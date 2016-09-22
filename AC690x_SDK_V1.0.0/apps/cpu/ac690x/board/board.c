#include "sdk_cfg.h"
#include "timer.h"
#include "key_drv/key.h"
#include "key_drv/key.h"
#include "common/flash_cfg.h"
#include "vm/vm_api.h"
#include "rtos/os_api.h"
#include "common/common.h"
#include "clock_api.h"
#include "sys_detect.h"
#include "uart.h"
#include "hw_cpu.h"
#include "cpu.h"
#include "dac/dac.h"
#include "dac/ladc.h"
#include "irq_api.h"
#include "uart_api.h"
#include "uart_param.h"
#include "rtc/rtc_api.h"
#include "update.h"

#define FLASH_BASE_ADDR 0x1000000

extern OS_TCB  *OSTCBCur[1];
extern u8 vmm_fat[192];
extern u32 pm_usd_map[(MAX_SDRAM_SIZE + ((32*PAGE_SIZE)-1))/(32*PAGE_SIZE)];
extern void set_sydf_header_base(u32 base);
extern void set_cache_base(u32 base);
extern void bt_app_cfg();
extern void set_pwrmd(u8 sm);
extern int main_task_init(void);
extern int TaskBtTest();

APP_USE_FLASH_SYS_CFG app_use_flash_cfg;

AT(.ver_magic_start)
const char ver_start_magic[] = {0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x5F, 0x73, 0x74, 0x61, 0x72, 0x74, 0x00};

AT(.ver_magic_end)
const char ver_end_magic[] = {0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x5F, 0x65, 0x6E, 0x64, 0x00};

/*----------------------------------------------------------------------------*/
/**@brief  获取SDK版本信息
   @param
   @return
   @note
 */
/*----------------------------------------------------------------------------*/
char * get_sdk_version(void)
{
    return SDK_VER;
}



/*----------------------------------------------------------------------------*/
/**@brief  异常错误中断服务程序
   @param
   @return
   @note
 */
/*----------------------------------------------------------------------------*/
void exception_isr(void )
{
    u32 tmp;

    ///printf rets & reti
    asm("mov %0, RETS " : "=r" (tmp));

	printf("\nRETS=0x%x\n", tmp);

	printf("DEBUG_MSG = 0x%x\n", JL_DEBUG->MSG);
	printf("PRP MMU_MSG = 0x%x\n", JL_DEBUG->PRP_MMU_MSG);
	printf("LSB MMU MSG = 0x%x\n", JL_DEBUG->LSB_MMU_MSG_CH);
	printf("PRP WR LIMIT MSG = 0x%x\n", JL_DEBUG->PRP_WR_LIMIT_MSG);
	printf("LSB WR LIMIT MSG = 0x%x\n", JL_DEBUG->LSB_WR_LIMIT_CH);

	asm("mov %0, RETI " : "=r" (tmp));

	printf("\nRETI=0x%x\n", tmp);
//	printf_buf((u8 *)MMU_TAG_ADDR0, 256);
#ifdef __DEBUG
     //JL_POWER->CON |= BIT(4); //异常后软复位
     while(1);
#else
//    JL_POWER->CON |= BIT(4); //异常后软复位
#endif // __DEBUG
}
/* IRQ_REGISTER(IRQ_EXCEPTION_IDX, exception_isr);
 */

/*----------------------------------------------------------------------------*/
/**@brief  poweroff_ram_fun
   @param
   @return
   @note

 */
/*----------------------------------------------------------------------------*/
void poweroff_ram_fun()
{
    // close_wdt();//关看门狗
    uart_init(UART_BAUD_RAE); //<串口波特率
    delay(10000);
//    putchar('X');
//    putchar('K');
//    putchar('O');
    //puts("\n  ------ run ram ------- \n");
}


/*$PAGE*/


void set_g_sys_cfg(FLASH_SYS_CFG *sys_cfg,u32 cfg_addr)
{
    memcpy(sys_cfg, (void*)(FLASH_BASE_ADDR + cfg_addr), sizeof(FLASH_SYS_CFG));
    app_use_flash_cfg.cfg_zone_addr = sys_cfg->flash_cfg.cfg_zone_addr;
    app_use_flash_cfg.flash_size = sys_cfg->flash_cfg.flash_size;
    app_use_flash_cfg.cfg_zone_size = sys_cfg->flash_cfg.cfg_zone_size;
#if 0
    printf("flash_id : %x\n",   sys_cfg->flash_cfg.flash_id);
    printf("flash_size : %x\n",   sys_cfg->flash_cfg.flash_size);
    printf("flash_file_size : %x\n",   sys_cfg->flash_cfg.flash_file_size);
    printf("sdfile_head_addr : %x\n",   sys_cfg->flash_cfg.sdfile_head_addr);
    printf("spi_run_mode : %x\n",   sys_cfg->flash_cfg.spi_run_mode);
    printf("spi_div : %x\n",   sys_cfg->flash_cfg.spi_div);
    printf("flash_base : %x\n",   sys_cfg->flash_cfg.flash_base);
    printf("protected_arg : %x\n",   sys_cfg->flash_cfg.protected_arg);
    printf("cfg_zone_addr : %x\n",   sys_cfg->flash_cfg.cfg_zone_addr);
    printf("cfg_zone_size : %x\n",   sys_cfg->flash_cfg.cfg_zone_size);

    printf("sdram_refresh_cycle : %x\n",   sys_cfg->sdr_cfg.sdram_refresh_cycle);
    printf("sdram_refresh_time : %x\n",   sys_cfg->sdr_cfg.sdram_refresh_time);
    printf("sdram_cas_latency : %x\n",   sys_cfg->sdr_cfg.sdram_cas_latency);
    printf("sdram_trfc : %x\n",   sys_cfg->sdr_cfg.sdram_trfc);
    printf("sdram_trp : %x\n",   sys_cfg->sdr_cfg.sdram_trp);
    printf("sdram_trcd : %x\n",   sys_cfg->sdr_cfg.sdram_trcd);
    printf("sdram_colum : %x\n",   sys_cfg->sdr_cfg.sdram_colum);
    printf("sdram_clk_hd_en : %x\n",   sys_cfg->sdr_cfg.sdram_clk_hd_en);
    printf("sdram_trrd : %x\n",   sys_cfg->sdr_cfg.sdram_trrd);
    printf("sdram_clkoe : %x\n",   sys_cfg->sdr_cfg.sdram_clkoe);
    printf("sdram_size : %x\n",   sys_cfg->sdr_cfg.sdram_size);
    printf("sdram_fast_clock_rate : %x\n",   sys_cfg->sdr_cfg.sdram_fast_clock_rate);

    printf("pll_sel : %x\n",   sys_cfg->clk_cfg.pll_sel);
    printf("osc_freq : %x\n",   sys_cfg->clk_cfg.osc_freq);
    printf("osc_src :%x\n",   sys_cfg->clk_cfg.osc_src);
    printf("osc_hc_en : %x\n",   sys_cfg->clk_cfg.osc_hc_en);
    printf("osc_1pin_en : %x\n",   sys_cfg->clk_cfg.osc_1pin_en);
    printf("---------------------------------------------\r\n");
#endif
}

extern void power_init(u8 mode);
/*----------------------------------------------------------------------------*/
/**@brief  system start enter
   @param  cfg_addr - g_sys_cfg's addr ， addr - app_file's addr，res - no used
   @return
   @note

 */

/*----------------------------------------------------------------------------*/
extern void set_poweroff_wakeup_io();
extern void set_poweroff_wakeup_io_handle_register(void (*handle)());
void  board_main(u32 cfg_addr, u32 addr, u32 res,u32 update_flag)
{
    u32 tmp;
    FLASH_SYS_CFG sys_cfg;
    /* close_wdt(); */
    clr_PINR_ctl();
	update_check(update_flag);
    /* JL_SYSTEM->LVD_CON &= ~BIT(2); */


    /* JL_CLOCK->SYS_DIV = 0; */
    clock_init(SYS_CLOCK_IN, OSC_Hz, SYS_Hz);
	set_spi_speed_auto();
	
	/* SFR(CLK_CON2,0,2,0); */
	/* clock_out_PA4(PA4_CLOCK_OUT_LSB); */
    uart_init(UART_BAUD_RAE); //<串口波特率

    /* printf("HSB_DIV=%d\n", JL_CLOCK->SYS_DIV & 0xff);
 */
    /* printf("LSB_DIV=%d\n", (JL_CLOCK->SYS_DIV & (0x7<<8))>>8);
 */
    /* printf("SFC_BAUD=%d\n", JL_SFC->BAUD);
 */
    /* printf("hsb_clk_hz= %u HZ\n", clock_get_sys_freq());
 */
    /* printf("lsb_clk_hz= %u HZ\n", clock_get_lsb_freq());
 */

	puts("\n\n***************************AC69 PowerUp**********************************\n");
 
    u8 *protect_info;
    protect_info=(u8 *)0x40000;
    printf_buf(protect_info,32);
    printf("--JL_POWER->CON-- : 0x%x\n", JL_POWER->CON>>5);//0:PWR_ON 1:LVD 2:WDT 3:SRST 4:POWEROFF 

    set_g_sys_cfg(&sys_cfg, cfg_addr);

	__asm__ volatile ("mov %0,ie0" : "=r"(tmp));
	printf("ie0 =%08x \n",tmp);
	__asm__ volatile ("mov %0,icfg" : "=r"(tmp));
	printf("icfg =%08x \n",tmp);
	
	/* printf("syd_cfg_addr=0x%x\n", cfg_addr); */
    set_sydf_header_base(sys_cfg.flash_cfg.sdfile_head_addr);

    set_cache_base(FLASH_BASE_ADDR);

    vm_init_api(&sys_cfg.flash_cfg);
    vm_open_all();
#if BT_2_1_DEBUG
    bt_app_cfg();
#endif
	irq_handler_register(0, exception_isr, 0); //exception isr always in highest IP,can not set IP
    timer0_init();
    sys_init();

#if BT_2_1_DEBUG
	/* 2:LDO  3:DCDC*/
	power_init(PWR_MODE_SELECT);
#endif

	/* set_poweroff_wakeup_io_handle_register(set_poweroff_wakeup_io); */
	/* soft_power_ctl(PWR_OFF); */

//    audio_init();
//    set_sys_vol(30, 30, FADE_ON);
//    ladc_reg_init(ENC_ENC_LINE_LR_CHANNEL , LADC_SR16000);
//    dac_set_samplerate(16000,0);


    main_task_init();
    OSStart();
    irq_global_enable();
    __asm__ volatile ("mov %0,ie0" : "=r"(tmp));
    printf("ie0 =%08x \n",tmp);
    __asm__ volatile ("mov %0,ie1" : "=r"(tmp));
    printf("ie1 =%08x \n",tmp);
	__asm__ volatile ("mov %0,icfg" : "=r"(tmp));
    printf("icfg =%08x \n",tmp);
    while(1)
    {
        __asm__ volatile ("idle");
        __asm__ volatile ("nop");
        __asm__ volatile ("nop");
    }
}
volatile unsigned long jiffies;


