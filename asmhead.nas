; haribote-os boot asm
; TAB=4
[INSTRSET "i486p"]				; 486偺柦椷傑偱巊偄偨偄偲偄偆婰弎
BOTPAK	EQU		0x00280000		; kernel偺儘乕僪愭
DSKCAC	EQU		0x00100000		; 僨傿僗僋僉儍僢僔儏偺応強
DSKCAC0	EQU		0x00008000		; 僨傿僗僋僉儍僢僔儏偺応強乮儕傾儖儌乕僪乯

; BOOT_INFO娭學
CYLS	EQU		0x0ff0			; 僽乕僩僙僋僞偑愝掕偡傞
LEDS	EQU		0x0ff1
VMODE	EQU		0x0ff2			; 怓悢偵娭偡傞忣曬丅壗價僢僩僇儔乕偐丠
SCRNX	EQU		0x0ff4			; 夝憸搙偺X
SCRNY	EQU		0x0ff6			; 夝憸搙偺Y
VRAM	EQU		0x0ff8			; 僌儔僼傿僢僋僶僢僼傽偺奐巒斣抧
VBEMODE	EQU		0x105           ;1024*768*8bit
		ORG		0xc200			; 偙偺僾儘僌儔儉偑偳偙偵撉傒崬傑傟傞偺偐
;设置显卡模式

		MOV		AX,0x9000       ;测试是否可以使用vbe
		MOV		ES,AX
		MOV		DI,0
		MOV		AX,0x4f00
		INT		0x10
		CMP		AX,0x004f
		JNE		scrn320     ;不能测试用原始的320*200

;vbe可用，则测试vbe版本，2.0以下仍不能使用高分辨率

		MOV		AX,[ES:DI+4]
		CMP		AX,0x0200
		JB		scrn320			; if (AX < 0x0200) goto scrn320

;测试是否可以使用指定的分辨率

		MOV		CX,VBEMODE
		MOV		AX,0x4f01
		INT		0x10
		CMP		AX,0x004f
		JNE		scrn320

;测试模式是否可用

		CMP		BYTE [ES:DI+0x19],8     ;颜色数是否为8，不是不能用
		JNE		scrn320
		CMP		BYTE [ES:DI+0x1b],4      ;指定颜色，不为4不能用
		JNE		scrn320
		MOV		AX,[ES:DI+0x00]          ;测试模式属性第7bit是否为1，为1表示可以使用
		AND		AX,0x0080                ;与运算，结果为0说明7bit为0
		JZ		scrn320			; 若7bit为0，则不能用

; 通过测试，保存图像信息

		MOV		BX,VBEMODE+0x4000
		MOV		AX,0x4f02
		INT		0x10
		MOV		BYTE [VMODE],8	; 夋柺儌乕僪傪儊儌偡傞乮C尵岅偑嶲徠偡傞乯
		MOV		AX,[ES:DI+0x12]
		MOV		[SCRNX],AX
		MOV		AX,[ES:DI+0x14]
		MOV		[SCRNY],AX
		MOV		EAX,[ES:DI+0x28]
		MOV		[VRAM],EAX
		JMP		keystatus

scrn320:
		MOV		AL,0x13			; VGA僌儔僼傿僢僋僗丄320x200x8bit僇儔乕
		MOV		AH,0x00
		INT		0x10
		MOV		BYTE [VMODE],8	; 夋柺儌乕僪傪儊儌偡傞乮C尵岅偑嶲徠偡傞乯
		MOV		WORD [SCRNX],320
		MOV		WORD [SCRNY],200
		MOV		DWORD [VRAM],0x000a0000


; 僉乕儃乕僪偺LED忬懺傪BIOS偵嫵偊偰傕傜偆
keystatus:
		MOV		AH,0x02
		INT		0x16 			; keyboard BIOS
		MOV		[LEDS],AL

; PIC偑堦愗偺妱傝崬傒傪庴偗晅偗側偄傛偆偵偡傞
;	AT屳姺婡偺巇條偱偼丄PIC偺弶婜壔傪偡傞側傜丄
;	偙偄偮傪CLI慜偵傗偭偰偍偐側偄偲丄偨傑偵僴儞僌傾僢僾偡傞
;	PIC偺弶婜壔偼偁偲偱傗傞

		MOV		AL,0xff
		OUT		0x21,AL
		NOP						; OUT柦椷傪楢懕偝偣傞偲偆傑偔偄偐側偄婡庬偑偁傞傜偟偄偺偱
		OUT		0xa1,AL

		CLI						; 偝傜偵CPU儗儀儖偱傕妱傝崬傒嬛巭

; CPU偐傜1MB埲忋偺儊儌儕偵傾僋僙僗偱偒傞傛偆偵丄A20GATE傪愝掕

		CALL	waitkbdout
		MOV		AL,0xd1
		OUT		0x64,AL
		CALL	waitkbdout
		MOV		AL,0xdf			; enable A20
		OUT		0x60,AL
		CALL	waitkbdout

; 僾儘僥僋僩儌乕僪堏峴



		LGDT	[GDTR0]			; 巄掕GDT傪愝掕
		MOV		EAX,CR0
		AND		EAX,0x7fffffff	; bit31傪0偵偡傞乮儁乕僕儞僌嬛巭偺偨傔乯
		OR		EAX,0x00000001	; bit0傪1偵偡傞乮僾儘僥僋僩儌乕僪堏峴偺偨傔乯
		MOV		CR0,EAX
		JMP		pipelineflush
pipelineflush:
		MOV		AX,1*8			;  撉傒彂偒壜擻僙僌儊儞僩32bit
		MOV		DS,AX
		MOV		ES,AX
		MOV		FS,AX
		MOV		GS,AX
		MOV		SS,AX

; kernel偺揮憲

		MOV		ESI,kernel	; 揮憲尦
		MOV		EDI,BOTPAK		; 揮憲愭
		MOV		ECX,512*1024/4
		CALL	memcpy

; 偮偄偱偵僨傿僗僋僨乕僞傕杮棃偺埵抲傊揮憲

; 傑偢偼僽乕僩僙僋僞偐傜

		MOV		ESI,0x7c00		; 揮憲尦
		MOV		EDI,DSKCAC		; 揮憲愭
		MOV		ECX,512/4
		CALL	memcpy

; 巆傝慡晹

		MOV		ESI,DSKCAC0+512	; 揮憲尦
		MOV		EDI,DSKCAC+512	; 揮憲愭
		MOV		ECX,0
		MOV		CL,BYTE [CYLS]
		IMUL	ECX,512*18*2/4	; 僔儕儞僟悢偐傜僶僀僩悢/4偵曄姺
		SUB		ECX,512/4		; IPL偺暘偩偗嵎偟堷偔
		CALL	memcpy

; asmhead偱偟側偗傟偽偄偗側偄偙偲偼慡晹偟廔傢偭偨偺偱丄
;	偁偲偼kernel偵擟偣傞

; kernel偺婲摦

		MOV		EBX,BOTPAK
		MOV		ECX,[EBX+16]
		ADD		ECX,3			; ECX += 3;
		SHR		ECX,2			; ECX /= 4;
		JZ		skip			; 揮憲偡傞傋偒傕偺偑側偄
		MOV		ESI,[EBX+20]	; 揮憲尦
		ADD		ESI,EBX
		MOV		EDI,[EBX+12]	; 揮憲愭
		CALL	memcpy
skip:
		MOV		ESP,[EBX+12]	; 僗僞僢僋弶婜抣
		JMP		DWORD 2*8:0x0000001b

waitkbdout:
		IN		 AL,0x64
		AND		 AL,0x02
		JNZ		waitkbdout		; AND偺寢壥偑0偱側偗傟偽waitkbdout傊
		RET

memcpy:
		MOV		EAX,[ESI]
		ADD		ESI,4
		MOV		[EDI],EAX
		ADD		EDI,4
		SUB		ECX,1
		JNZ		memcpy			; 堷偒嶼偟偨寢壥偑0偱側偗傟偽memcpy傊
		RET
; memcpy偼傾僪儗僗僒僀僘僾儕僼傿僋僗傪擖傟朰傟側偗傟偽丄僗僩儕儞僌柦椷偱傕彂偗傞

		ALIGNB	16
GDT0:
		RESB	8				; 僰儖僙儗僋僞
		DW		0xffff,0x0000,0x9200,0x00cf	; 撉傒彂偒壜擻僙僌儊儞僩32bit
		DW		0xffff,0x0000,0x9a28,0x0047	; 幚峴壜擻僙僌儊儞僩32bit乮kernel梡乯

		DW		0
GDTR0:
		DW		8*3-1
		DD		GDT0

		ALIGNB	16
kernel:
