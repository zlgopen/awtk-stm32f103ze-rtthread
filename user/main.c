#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "tftlcd.h"
#include "time.h"
#include "key.h"
#include "touch.h"
#include "rtc.h"
#include "stdlib.h"

#include "tkc/thread.h"
#include "platforms/common/rtos.h"

extern ret_t platform_prepare(void);
extern void sys_tick_enable(bool_t enable);
extern int gui_app_start(int lcd_w, int lcd_h);

void* awtk_thread(void* args) {
  gui_app_start(tftlcd_data.width, tftlcd_data.height);

  return NULL;
}

static ret_t awtk_start_ui_thread(void) {
  tk_thread_t* ui_thread = tk_thread_create(awtk_thread, NULL);
  return_value_if_fail(ui_thread != NULL, RET_BAD_PARAMS);

  tk_thread_set_priority(ui_thread, 3);
  tk_thread_set_name(ui_thread, "awtk");
  tk_thread_set_stack_size(ui_thread, 8000);

  return tk_thread_start(ui_thread);
}

void hardware_prepare(void) {
	SysTick_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
	LED_Init();
	USART1_Init(9600);
	TFTLCD_Init();			
	KEY_Init();
	TOUCH_Init();
	TIM3_Init(50,7199);
	RTC_Init();
	LCD_Clear(RED);
}

int main() {
  hardware_prepare();
  platform_prepare();
	sys_tick_enable(TRUE);
	
  rtos_init();
  awtk_start_ui_thread();
  rtos_start();
	
	return 0;
}
