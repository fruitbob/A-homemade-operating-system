void io_hlt(void);
void io_cli(void);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);
void write_mem8(int add,int data);

void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void draw_square(unsigned char *vram,int x0,int y0,unsigned char c,int s_x,int s_y);

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





void HariMain(void)
{
	init_palette(); 
	int i;
	unsigned char *p;
	p=0xa0000;
	for(i=0;i<=0xaffff;i++)
	{
		*(p+i)=3;
	 } 
	 draw_square(p, 50,50,4,10,100);
	 for(;;)
	 {
	 	io_hlt();
	 }

}

void init_palette(void)
{
    static unsigned char table_rgb[16 * 3] = {
        0x00, 0x00, 0x00,    /*  0:黑 */
        0xff, 0x00, 0x00,    /*  1:亮红*/
        0x00, 0xff, 0x00,    /*  2:亮绿 */
        0xff, 0xff, 0x00,    /*  3:亮黄 */
        0x00, 0x00, 0xff,    /*  4:亮蓝 */
        0xff, 0x00, 0xff,    /*  5:亮紫 */
        0x00, 0xff, 0xff,    /*  6:浅亮蓝 */
        0xff, 0xff, 0xff,    /*  7:白 */
        0xc6, 0xc6, 0xc6,    /*  8:亮灰色 */
        0x84, 0x00, 0x00,    /*  9:暗红色 */
        0x00, 0x84, 0x00,    /* 10:暗绿色 */
        0x84, 0x84, 0x00,    /* 11:暗黄色 */
        0x00, 0x00, 0x84,    /* 12:暗青色 */
        0x84, 0x00, 0x84,    /* 13:暗紫色 */
        0x00, 0x84, 0x84,    /* 14:浅暗蓝 */
        0x84, 0x84, 0x84    /* 15:暗灰色 */
    };
    set_palette(0, 15, table_rgb);//将调色板写入硬件 
    return;

}
void draw_square(unsigned char *vram,int x0,int y0,unsigned char c,int x1,int y1)
{
	int x,y;

		for(y=y0;y<=y0+y1;y++)
		{
			for(x=x0;x<=x0+x1;x++)
			{
				vram[x+y*320]=c;
			}
		}
		return;
}
void set_palette(int start, int end, unsigned char *rgb)
{
	int i, eflags;
	eflags = io_load_eflags();	//记录标志信息 
	io_cli(); 					//禁止中断 
	io_out8(0x03c8, start);	//将想要设置的调色板编号写入0x03c8端口 
	for (i = start; i <= end; i++) {
		io_out8(0x03c9, rgb[0] / 4);
		io_out8(0x03c9, rgb[1] / 4);
		io_out8(0x03c9, rgb[2] / 4);
		rgb += 3;
	}
	io_store_eflags(eflags);	//回复标志信息 
	return;
}
