#include "clock.h"
#include "clock_api.h"
#include "uart.h"
#include "timer.h"
#include "key_drv/key_drv_ir.h"
#include "bluetooth/avctp_user.h"
#include "key_drv/key_drv_ad.h"
AT(.common)
static void set_spi_speed(u8 baud)
{
    while(!(JL_DSP->CON & BIT(5)));
	/* SPI2_BAUD = sp; */
	JL_SFC->BAUD = baud;
}


AT(.common)
void set_spi_speed_auto(void)
{
	u8 baud;

	for(baud=0; baud < 255; baud++)
	{
		if(SYS_CLK < (SPI_MAX_CLK*(baud+1)))
		{
			break;
		}
	}

	set_spi_speed(baud);
}

AT(.common)
void set_sys_freq(u32 out_freq)
{
	if(out_freq == SYS_CLK)
		return;
	
	user_prote_bt_process(1);
	clock_set_sys_freq(out_freq);
	user_prote_bt_process(0);

	set_spi_speed_auto();

	uart_set_baud(UART_BAUD_RAE);
	

#if KEY_IR_EN
	ir_key_ck_reset();
#endif

#if KEY_AD_EN
	ad_key0_init();
#endif

	timer0_clk_reset();

/* #if UART_UPDATA_EN */
	/* UT1_BAUD = (UART_CLK / 460800) / 4 -1; */
/* #endif // UART_UPDATA_EN */

	printf("set_sys_freq = %d\n",SYS_CLK);
}
