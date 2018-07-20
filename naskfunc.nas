;汇编语言实现的函数，用于使cpu进入休眠hlt

[FORMAT "WCOFF"]				; 制作目标文件的模式
[BITS 32]						; 制作32位模式用的机器语言



[FILE "naskfunc.nas"]			; 文件名称

		GLOBAL	_io_hlt			; 声明全局函数 _io_hlt


; 函数内容

[SECTION .text]		; 目标文件中写了这些，再写程序
_io_hlt:	; void io_hlt(void);
		HLT
		RET			;相当于return
