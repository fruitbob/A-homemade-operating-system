;汇编语言实现的函数，用于使cpu进入休眠hlt

[FORMAT "WCOFF"]				; 制作目标文件的模式
[BITS 32]						; 制作32位模式用的机器语言
[INSTRSET "i486p"]				;用于486cpu


[FILE "naskfunc.nas"]			; 文件名称

		GLOBAL	_write_mem8		;声明全局函数 _io_hlt
		GLOBAL	_io_hlt, _io_cli, _io_sti, _io_stihlt
		GLOBAL	_io_in8,  _io_in16,  _io_in32
		GLOBAL	_io_out8, _io_out16, _io_out32
		GLOBAL	_io_load_eflags, _io_store_eflags,_load_gdtr,_load_idtr
		GLOBAL	_asm_inthandler21, _asm_inthandler27, _asm_inthandler2c
		EXTERN	_inthandler21, _inthandler27, _inthandler2c       ;21用于键盘中断，2c用于鼠标中断
		

; 函数内容

[SECTION .text]		; 目标文件中写了这些，再写程序
_io_hlt:	; void io_hlt(void);
		HLT
		RET			;相当于return

_write_mem8:      ;void write_mem8(int add,int data) ;向add内存写入一个字节data
		MOV	ECX,[ESP+4]
		MOV	AX,[ESP+8]
		MOV	[ECX],AX
		RET

_io_cli:	; void io_cli(void);禁止中断
		CLI
		RET

_io_sti:	; void io_sti(void);允许中断
		STI
		RET

_io_stihlt:	; void io_stihlt(void);允许中断后cpu暂停
		STI
		HLT
		RET

_io_in8:	; int io_in8(int port);从端口读取8bit
		MOV		EDX,[ESP+4]		; port
		MOV		EAX,0
		IN		AL,DX
		RET

_io_in16:	; int io_in16(int port);从端口p读取8比特到al,p+1读8bit到ah
		MOV		EDX,[ESP+4]		; port
		MOV		EAX,0
		IN		AX,DX
		RET

_io_in32:	; int io_in32(int port);
		MOV		EDX,[ESP+4]		; port
		IN		EAX,DX
		RET

_io_out8:	; void io_out8(int port, int data);将8bit的data写入port
		MOV		EDX,[ESP+4]		; port
		MOV		AL,[ESP+8]		; data
		OUT		DX,AL
		RET

_io_out16:	; void io_out16(int port, int data);将前8bit的data写入port，后8bit写入p+1
		MOV		EDX,[ESP+4]		; port
		MOV		EAX,[ESP+8]		; data
		OUT		DX,AX
		RET

_io_out32:	; void io_out32(int port, int data);
		MOV		EDX,[ESP+4]		; port
		MOV		EAX,[ESP+8]		; data
		OUT		DX,EAX
		RET

_io_load_eflags:	; int io_load_eflags(void);
		PUSHFD		;将32位的标志寄存器eflages压入堆栈
		POP		EAX
		RET

_io_store_eflags:	; void io_store_eflags(int eflags);修改eflag的值
		MOV		EAX,[ESP+4]
		PUSH	EAX
		POPFD		; POP EFLAGS 
		RET
_load_gdtr:		; void load_gdtr(int limit, int addr);
		MOV		AX,[ESP+4]		; limit
		MOV		[ESP+6],AX
		LGDT	[ESP+6]
		RET

_load_idtr:		; void load_idtr(int limit, int addr);
		MOV		AX,[ESP+4]		; limit
		MOV		[ESP+6],AX
		LIDT	[ESP+6]
		RET

_asm_inthandler21:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX,ESP
		PUSH	EAX
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	_inthandler21
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		IRETD

_asm_inthandler27:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX,ESP
		PUSH	EAX
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	_inthandler27
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		IRETD

_asm_inthandler2c:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX,ESP
		PUSH	EAX
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	_inthandler2c
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		IRETD






