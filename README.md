# A-homemade-operating-system
这个想法已经诞生很久了。苦于专业基础知识的缺乏，直到今天才开始正式动笔。这是一个可以运行在Intel_x86硬件架构上的操作系统软件，提供必要的硬件管理功能。本着由浅入深，由简单到复杂的原则，先写出一个可以在裸机上运行的简易程序，再在此基础上进行更新优化，功能扩展。主要使用汇编语言以及C语言进行编写。
  文件说明：
    ipl10.nas     ：os加载程序，硬件开机后首先加载进内存，用于将os加载进内存
    asmhead.nas   ：ipl10运行后首先加载的程序，用于设置显卡格式，键盘，提供c语言入口
    bootpack.nas  ：设置c语言的运行规范，编译参数等等
    naskfunc.nas  ：用于汇编语言编写的基础函数，供c语言调用，将其作为c语言控制硬件的接口（作为系统API）
    bootpack.c    ：os功能实现的核心代码
