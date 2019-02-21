

	  
struct SEGMENT_DESCRIPTOR {
	short limit_low, base_low;
	char base_mid, access_right;
	char limit_high, base_high;
};

struct GATE_DESCRIPTOR {
	short offset_low, selector;
	char dw_count, access_right;
	short offset_high;
};
//内存管理，memery.c 
struct memnode
{
	unsigned int add,size;
};

struct mem_table
{
	int flag,size,size_good;//flag是否有碎片 
	struct memnode node[4009];
};
#define EFLAGS_AC_BIT		0x00040000
#define CR0_CACHE_DISABLE	0x60000000
void init_gdtidt(void);
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar);
void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar);
unsigned int memtest(unsigned int start, unsigned int end);
unsigned int memtest_sub(unsigned int start, unsigned int end);
void init_memtable(struct mem_table *table,unsigned int add,unsigned int size); 
unsigned int mem_alloc(struct mem_table *table,unsigned int size);
int mem_free(struct mem_table *table,unsigned int add,unsigned int size);
unsigned int mem_alloc_4k(struct mem_table *table,unsigned int size);
int mem_free_4k(struct mem_table *table,unsigned int add,unsigned int size);

//基本的寄存器操作，naskfunc.nas 
void io_hlt(void);
void io_cli(void);
void io_sti(void);
void io_stihit(void);
unsigned int memtest_sub(unsigned int start, unsigned int end);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);
void write_mem8(int add,int data);
void load_gdtr(int limit, int addr);//将idt与gdt写入寄存器  
void load_idtr(int limit, int addr);
void asm_inthandler21(void);
void asm_inthandler27(void);
void asm_inthandler2c(void);
void asm_inthandler20(void);
//图形交互，window.c 
#define COL8_000000		0
#define COL8_FF0000		1
#define COL8_00FF00		2
#define COL8_FFFF00		3
#define COL8_0000FF		4
#define COL8_FF00FF		5
#define COL8_00FFFF		6
#define COL8_FFFFFF		7
#define COL8_C6C6C6		8
#define COL8_840000		9
#define COL8_008400		10
#define COL8_848400		11
#define COL8_000084		12
#define COL8_840084		13
#define COL8_008484		14
#define COL8_848484		15

struct BOOTINFO { 
	char cyls; 
	char leds; 
	char vmode; 
	char reserve;
	short scrnx, scrny; 
	char *vram;
};
#define ADR_BOOTINFO	0x00000ff0

void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void init_mouse16(const struct BOOTINFO *binfo,int p_x,int p_y,int s_x);
void os_outchar16(const unsigned char *vram,int sc_x,int x0,int y0,int cl,char ch);
void os_draw_square(unsigned char *vram,int sc_x,int x0,int y0,unsigned char c,int s_x,int s_y);
void os_printfs(unsigned char *vram,int sc_x,int x0,int y0,int cl,int bc,char *s,int l);
void change_back(const struct BOOTINFO *binfo,int p_x,int p_y,int s_x) ;
void init_window(struct BOOTINFO *binfo);
void init_cmd(struct BOOTINFO *binfo);
void dis_cmd(struct BOOTINFO *binfo);
void init_cur(struct BOOTINFO *binfo, int cur_x, int cur_y, int cur_c);
void cur_out(struct BOOTINFO *binfo, int *cur_y);
//中断处理，int.c 

 
void init_pit(); 
void init_pic(void);
void inthandler21(int *esp);
void inthandler27(int *esp);
void inthandler2c(int *esp);
void inthandler20(int *esp);

#define  PORT_KEYDAT		0x0060
#define PIT_CTRL	0x0043
#define PIT_CNT0	0x0040

#define ADR_IDT			0x0026f800
#define LIMIT_IDT		0x000007ff
#define ADR_GDT			0x00270000
#define LIMIT_GDT		0x0000ffff
#define ADR_BOTPAK		0x00280000
#define LIMIT_BOTPAK	0x0007ffff
#define AR_DATA32_RW	0x4092
#define AR_CODE32_ER	0x409a
#define AR_INTGATE32	0x008e

#define PIC0_ICW1		0x0020
#define PIC0_OCW2		0x0020
#define PIC0_IMR		0x0021
#define PIC0_ICW2		0x0021
#define PIC0_ICW3		0x0021
#define PIC0_ICW4		0x0021
#define PIC1_ICW1		0x00a0
#define PIC1_OCW2		0x00a0
#define PIC1_IMR		0x00a1
#define PIC1_ICW2		0x00a1
#define PIC1_ICW3		0x00a1
#define PIC1_ICW4		0x00a1

//FIFO缓冲区   fifo.c 
struct FIFO
{
	unsigned char *add;
	int head,last,size,num;//flag 为元素个数 
};
struct llFIFO
{
	long long *add;
	int head, last, size, num;//flag 为元素个数 
};
void fifo_init(struct FIFO *fifo,int size,unsigned char *add);
int fifo_push(struct FIFO *fifo,unsigned char data);
int fifo_pop(struct FIFO *fifo);

void llfifo_init(struct llFIFO *fifo, int size, long long *add);
int llfifo_push(struct llFIFO *fifo, long long data);
long long llfifo_pop(struct llFIFO *fifo);


//键盘处理，keyboard.c
#define PORT_KEYDAT				0x0060
#define PORT_KEYSTA				0x0064
#define PORT_KEYCMD				0x0064
#define KEYSTA_SEND_NOTREADY	0x02
#define KEYCMD_WRITE_MODE		0x60
#define KBC_MODE				0x47
void init_keyboard();
void wait_KBC_sendready();

//mouse.c
#define KEYCMD_SENDTO_MOUSE		0xd4
#define MOUSECMD_ENABLE			0xf4
struct mouse_read
{
	int flag;
	unsigned char inf[3];
	int x,y,click;
};

void enable_mouse();
int mouse_decode(unsigned char data);

//time.c，用于处理时间，计时器等等

struct time0
{
	int num;
	int hour, mintue, second;
};
struct timesth0     //用于记录定时事件
{
	int job[500];
	int deadline[500];
	int f[500];
	int next; 
};
void init_timesth();
void init_timer(int job, int t);

//api.c 各种命令行api实现
void api_showtime(char *cmd, struct BOOTINFO *binfo, struct time0 time);
void api_cls(char *cmd, struct BOOTINFO *binfo);
void api_mem(char *cmd, struct BOOTINFO *binfo);
int api_time(char *cmd, struct BOOTINFO *binfo, struct time0 *time);
int ctn(char *s, int len);
int is_num(char c);
int api_settimer(char *cmd, int len, int num);
float api_calcu(char *cmd, int len, struct BOOTINFO *binfo);
int priority(char c);
float cal(char a, char c, char b);







