# AWTK 针对RT-Thread(STM32f103ze) 的移植。

* [AWTK](https://github.com/zlgopen/awtk) 全称 Toolkit AnyWhere，是 [ZLG](http://www.zlg.cn/) 开发的开源 GUI 引擎，旨在为嵌入式系统、WEB、各种小程序、手机和 PC 打造的通用 GUI 引擎，为用户提供一个功能强大、高效可靠、简单易用、可轻松做出炫酷效果的 GUI 引擎。

* [RT-Thread](https://github.com/RT-Thread/rt-thread) RT-Thread is an open source IoT operating system from China, which has strong scalability: from a tiny kernel running on a tiny core, for example ARM Cortex-M0, or Cortex-M3/4/7, to a rich feature system running on MIPS32, ARM Cortex-A8, ARM Cortex-A9 DualCore etc.

* [awtk-stm32f103ze-rtthread](https://github.com/zlgopen/awtk-stm32f103ze-rtthread) 是 AWTK 在  [RT-Thread](https://github.com/RT-Thread/rt-thread) 上的移植。

> 本项目以 [普中科技 STM32F103ZET6 开发实验板](https://item.taobao.com/item.htm?spm=a230r.1.14.1.50a130e8TMKYMC&id=558855281660&ns=1&abbucket=5#detail) 为载体移植，其它开发板可能要做些修改，有问题请请创建 issue。

## 编译

1. 获取源码

```
git clone https://github.com/zlgopen/awtk-stm32f103ze-rtthread.git
cd awtk-stm32f103ze-rtthread
git clone https://github.com/zlgopen/awtk.git
```

2. 用 keil 打开 user/awtk.uvproj

## 文档

[AWTK 在 RT-Thread 上的移植笔记]( docs/rtt-port.md)
