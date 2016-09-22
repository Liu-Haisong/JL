#ifndef _LED_H_
#define _LED_H_

#include "includes.h"
#include "sys_detect.h"
#include "sdk_cfg.h"

#ifdef MINI
    #ifdef __DEBUG
    #define LED_INIT_EN()     do{JL_PWM4->CON = 0;JL_IOMAP->CON1 |= BIT(11)|BIT(12)|BIT(13);}while(0)
    #define LED_INIT_DIS()    do{JL_PWM4->CON = 0;JL_IOMAP->CON1 &= ~(BIT(11)|BIT(12)|BIT(13));}while(0)
    #define R_LED_ON()        do{JL_PWM4->CON = BIT(1)|BIT(0); JL_PORTA->DIR &= ~BIT(2);JL_PORTA->PU |= BIT(2);JL_PORTA->PD |= BIT(2);}while(0)
    #define R_LED_OFF()       do{JL_PORTA->DIR |= BIT(2);JL_PORTA->PU &= ~BIT(2);JL_PORTA->PD &= ~BIT(2);}while(0)
    #define B_LED_ON()        do{JL_PWM4->CON = BIT(3);JL_PORTA->DIR &=  ~BIT(3);JL_PORTA->PU |= BIT(3);JL_PORTA->PD |= BIT(3);}while(0)
    #define B_LED_OFF()       do{JL_PORTA->DIR |= BIT(3);JL_PORTA->PU &= ~BIT(3);JL_PORTA->PD &= ~BIT(3);}while(0)
    #else
//    #define LED_INIT_EN()     {JL_PORTA->PU &= ~(BIT(2)|BIT(3));JL_PORTA->PD &= ~(BIT(2)|BIT(3));JL_PORTA->DIR &= ~(BIT(2)|BIT(3));}
//    #define LED_INIT_DIS()    {JL_PORTA->PU &= ~(BIT(2)|BIT(3));JL_PORTA->PD &= ~(BIT(2)|BIT(3));JL_PORTA->DIR |=  (BIT(2)|BIT(3));}
//    #define R_LED_ON()        JL_PORTA->OUT |=  BIT(2);
//    #define R_LED_OFF()       JL_PORTA->OUT &= ~BIT(2);
//    #define B_LED_ON()        JL_PORTA->OUT |=  BIT(3);
//    #define B_LED_OFF()       JL_PORTA->OUT &= ~BIT(3);
    #define LED_INIT_EN()     do{JL_PWM4->CON = 0;JL_IOMAP->CON1 |= BIT(11)|BIT(12)|BIT(13);}while(0)
    #define LED_INIT_DIS()    do{JL_PWM4->CON = 0;JL_IOMAP->CON1 &= ~(BIT(11)|BIT(12)|BIT(13));}while(0)
    #define R_LED_ON()        do{JL_PWM4->CON = BIT(1)|BIT(0); JL_PORTA->DIR &= ~BIT(2);JL_PORTA->PU |= BIT(2);JL_PORTA->PD |= BIT(2);}while(0)
    #define R_LED_OFF()       do{JL_PORTA->DIR |= BIT(2);JL_PORTA->PU &= ~BIT(2);JL_PORTA->PD &= ~BIT(2);}while(0)
    #define B_LED_ON()        do{JL_PWM4->CON = BIT(3);JL_PORTA->DIR &=  ~BIT(3);JL_PORTA->PU |= BIT(3);JL_PORTA->PD |= BIT(3);}while(0)
    #define B_LED_OFF()       do{JL_PORTA->DIR |= BIT(3);JL_PORTA->PU &= ~BIT(3);JL_PORTA->PD &= ~BIT(3);}while(0)
    #endif
#else
    #define LED_BIT           BIT(11) ///PB11
    #define LED_INIT_EN()     {JL_PORTB->PU &= ~LED_BIT;JL_PORTB->PD &= ~LED_BIT;JL_PORTB->DIR &= ~LED_BIT;}
    #define LED_INIT_DIS()    {JL_PORTB->PU &= ~LED_BIT;JL_PORTB->PD &= ~LED_BIT;JL_PORTB->DIR |= LED_BIT;}
    #define B_LED_ON()        JL_PORTB->OUT |= LED_BIT;
    #define B_LED_OFF()       JL_PORTB->OUT &= ~LED_BIT;
    #define R_LED_ON(...)
    #define R_LED_OFF(...)
#endif/*MINI_BT*/

extern u32 g_led_fre;

void led_init(void);
void led_close(void);
void led_open(void);
void led_fre_set(u32 fre,u8 led_flash);
void led_scan(void *param);

#endif/*_LED_H_*/

