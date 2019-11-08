/**
 * File:   platform.c
 * Author: AWTK Develop Team
 * Brief:  platform dependent function of stm32
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * this program is distributed in the hope that it will be useful,
 * but without any warranty; without even the implied warranty of
 * merchantability or fitness for a particular purpose.  see the
 * license file for more details.
 *
 */

/**
 * history:
 * ================================================================
 * 2018-10-20 li xianjing <xianjimli@hotmail.com> created
 *
 */

#include "sys.h"
#include "tkc/mem.h"
#include "base/timer.h"

void systick_init(void) {
  u8 fac_us = 0;
  u16 fac_ms = 0;

  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
  fac_us = SystemCoreClock / 8000000;

  fac_ms = (u16)fac_us * 1000;

  SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
  SysTick->LOAD = fac_ms;
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

static bool_t s_inited = FALSE;
static uint32_t s_heam_mem[4096];

ret_t platform_prepare(void) {
	if(!s_inited) {
		s_inited = TRUE;
    tk_mem_init(s_heam_mem, sizeof(s_heam_mem));
	}
  return RET_OK;
}
