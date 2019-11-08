#include "button.h"
#include "delay.h"
#include "flash.h"
#include "gui.h"
#include "led.h"
#include "rtc.h"
#include "stdlib.h"
#include "sys.h"
#include "tim.h"
#include "touch.h"
#include "usart.h"
#include "lcd_driver.h"
#include "tkc/thread.h"
#include "platforms/common/rtos.h"

extern void systick_init(void);
extern ret_t platform_prepare(void);
extern int gui_app_start(int lcd_w, int lcd_h);

void* awtk_thread(void* args) {
  gui_app_start(320, 480);

  return NULL;
}

static ret_t awtk_start_ui_thread(void) {
  tk_thread_t* ui_thread = tk_thread_create(awtk_thread, NULL);
  return_value_if_fail(ui_thread != NULL, RET_BAD_PARAMS);

  tk_thread_set_priority(ui_thread, 3);
  tk_thread_set_name(ui_thread, "awtk");
  tk_thread_set_stack_size(ui_thread, 2048);

  return tk_thread_start(ui_thread);
}

static void hardware_prepare(void) {
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  systick_init();
  led_init();
  button_init();
  TFT_Init();
  TFT_ClearScreen(BLACK);
  FLASH_Init();
  TOUCH_Init();

  TIM3_Init(50, 7199);
  rtc_init();
}

int main() {
  hardware_prepare();
  platform_prepare();

  rtos_init();
  awtk_start_ui_thread();
  rtos_start();
}
