# AWTK 在 RT-Thread 上的移植笔记

本文以 STM32f103ze 为例，介绍了 AWTK 在 RTOS 上移植的经验。与其说移植，倒不如说是集成。所做的事情不过是把 AWTK 放到 RTOS 的一个线程中执行而已。

## 1. 加入 RT-Thread 相关文件。

AWTK 已经移植到 STM32f103ze 裸系统上，为了简单起见，直接在 [awtk-stm32f103ze-raw](https://github.com/zlgopen/awtk-stm32f103ze-raw/) 基础上加入 RT-Thread 支持。

* 在 Keil 中增加下列文件：

```
rtthread/
rtthread/bsp
rtthread/cortex-m3
rtthread/cortex-m3/context_gcc.S
rtthread/cortex-m3/context_iar.S
rtthread/cortex-m3/context_rvds.S
rtthread/cortex-m3/cpuport.c
rtthread/cortex-m3/SConscript
rtthread/include
rtthread/include/libc
rtthread/include/libc/libc_dirent.h
rtthread/include/libc/libc_errno.h
rtthread/include/libc/libc_fcntl.h
rtthread/include/libc/libc_fdset.h
rtthread/include/libc/libc_ioctl.h
rtthread/include/libc/libc_signal.h
rtthread/include/libc/libc_stat.h
rtthread/include/rtdbg.h
rtthread/include/rtdebug.h
rtthread/include/rtdef.h
rtthread/include/rthw.h
rtthread/include/rtlibc.h
rtthread/include/rtm.h
rtthread/include/rtservice.h
rtthread/include/rtthread.h
rtthread/rtconfig.h
rtthread/src
rtthread/src/clock.c
rtthread/src/cpu.c
rtthread/src/device.c
rtthread/src/idle.c
rtthread/src/ipc.c
rtthread/src/irq.c
rtthread/src/Kconfig
rtthread/src/kservice.c
rtthread/src/mem.c
rtthread/src/memheap.c
rtthread/src/mempool.c
rtthread/src/object.c
rtthread/src/scheduler.c
rtthread/src/SConscript
rtthread/src/signal.c
rtthread/src/slab.c
rtthread/src/thread.c
rtthread/src/timer.c
```

* 增加 include 的路径

```
rtthread
rtthread/include
```

* 修改配置文件

根据自己的需要修改配置 rtthread/rtconfig.h 

> 一般来说不需要修改，使用官方提供的即可。我用的是 [stm32f103-mini-system](https://github.com/RT-Thread/rt-thread/blob/master/bsp/stm32/stm32f103-mini-system/rtconfig.h) 项目中的。


## 2. 加入针对 RT-Thread 实现的线程和同步的函数。

```
src/platforms/rtt/mutex.c
src/platforms/rtt/semaphore.c
src/platforms/rtt/thread.c
src/platforms/common/sys_tick.c
```

## 3. 实现rtos.c。

> 参考stm32/libraries/HAL\_Drivers/drv\_common.c和components.c修改的。


```
#include "rthw.h"
#include "rtthread.h"

static bool_t s_kernel_inited = FALSE;

static bool_t rtos_is_inited(void) {
  return s_kernel_inited;
}

static uint32_t s_heap[2 * 1024];

ret_t rtos_init(void) {
  rt_hw_interrupt_disable();

  /* show version */
  rt_show_version();

#ifdef RT_USING_HEAP
  rt_system_heap_init((void*)s_heap, s_heap + sizeof(s_heap) / sizeof(s_heap[0]));
#endif

  /* initialize scheduler system */
  rt_system_scheduler_init();

  /* initialize timer */
  rt_system_timer_init();

  /* initialize timer thread */
  rt_system_timer_thread_init();

  /* initialize idle thread */
  rt_thread_idle_init();

  s_kernel_inited = TRUE;

  return RET_OK;
}

ret_t rtos_start(void) {
  /* start scheduler */
  rt_system_scheduler_start();

  return RET_OK;
}

void rtos_tick(void) {
  if (rtos_is_inited()) {
    rt_interrupt_enter();
    rt_tick_increase();
    rt_interrupt_leave();
  }
}

void rtos_delay(uint32_t ms) {
  rt_thread_delay(ms);
}

```

## 4. 在线程中启动 AWTK

```

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

int main() {
  hardware_prepare();
  platform_prepare();

  rtos_init();
  awtk_start_ui_thread();

  rtos_start();
}
```

这里与裸系统不同的地方，主要有两个：

* 1. 在线程中启动 AWTK。
* 2. 要提前调用 platform\_prepare，platform\_prepare 负责初始化内存，放在 tk_init 中就有些晚，需要单独提出来调用。

为此 platform\_prepare 函数做了防重复调用的处理。

```
static bool_t s_inited = FALSE;
static uint32_t s_heam_mem[4096];

ret_t platform_prepare(void) {
	if(!s_inited) {
		s_inited = TRUE;
    tk_mem_init(s_heam_mem, sizeof(s_heam_mem));
	}
  return RET_OK;
}
```

AWTK 集成 RTOS 是非常简单的，以上过程大概花了 2 个小时吧。只要 RTOS 本身好移植，集成 AWTK 和 RTOS 只是分分钟的问题。

