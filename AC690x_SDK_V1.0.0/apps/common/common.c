/*--------------------------------------------------------------------------*/
/**@file     common.c
   @brief    常用公共函数
   @details
   @author
   @date    2011-3-7
   @note    CD003
*/
/*----------------------------------------------------------------------------*/
#include "typedef.h"
#include "common/common.h"

volatile u16 delay_cnt;

struct mem_stats sys_mem_sta;

//void delay_count() sec(.comm);
void delay_count()
{
    if(delay_cnt){
        delay_cnt--;
    }
}

void delay_2ms(u32 cnt)
{
    delay_cnt = cnt;
    while(delay_cnt);
}

void delay(u32 d)sec(.poweroff_text);
void delay(u32 d)
{
    while(d--){
        __builtin_pi32_nop();
    }
}


void my_memzero(u32 * dst, tu16 len)
{
    while (len)
    {
        *(u32*)dst++ = 0;
        len -= 4;
    }
}



void store_word(void *buf, u32 index, u32 word)
{
    u8 *p = ((u8 *)buf) + index;
    p[0] = (u8)word;
    p[1] = (u8)(word>>8);
}

void store_dword(void *buf, u32 index, u32 word)
{
    u8 *p = ((u8 *)buf) + index;
    p[0] = (u8)word;
    p[1] = (u8)(word>>8);
    p[2] = (u8)(word>>16);
    p[3] = (u8)(word>>24);
}


