#ifndef __KEY_DRV_TOUCH_H__
#define __KEY_DRV_TOUCH_H__

#include "config.h"

#if USE_TCH_KEY

#define NO_KEY          0xff

extern void touch_key_init(void);

extern  u8  touch_key_get_value(void);

extern  u8  touch_key_pressed_to_msg(u8 key_style, u8 key_num);

#endif

#endif
