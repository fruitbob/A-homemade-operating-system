#include "kernel.h"




struct time0 time;
extern struct timesth0 timesth;
void init_pic(void)
{
	io_out8(PIC0_IMR,  0xff  ); 
	io_out8(PIC1_IMR,  0xff  );

	io_out8(PIC0_ICW1, 0x11  ); 
	io_out8(PIC0_ICW2, 0x20  ); 
	io_out8(PIC0_ICW3, 1 << 2); 
	io_out8(PIC0_ICW4, 0x01  ); 

	io_out8(PIC1_ICW1, 0x11  ); 
	io_out8(PIC1_ICW2, 0x28  ); 
	io_out8(PIC1_ICW3, 2     ); 
	io_out8(PIC1_ICW4, 0x01  ); 

	io_out8(PIC0_IMR,  0xfb  ); 
	io_out8(PIC1_IMR,  0xff  ); 

	return;
}

struct FIFO keybuf, mousebuf;
struct FIFO timebuf,cmdbuf;
void inthandler21(int *esp)
{
	
	
	unsigned char data;
	io_out8(PIC0_OCW2, 0x61);	
	data = io_in8(PORT_KEYDAT);
	
	fifo_push(&keybuf, data);
	
	return;
}

void inthandler2c(int *esp)
{
	
	unsigned char data;
	io_out8(PIC1_OCW2, 0x64);	
	io_out8(PIC0_OCW2, 0x62);
	data = io_in8(PORT_KEYDAT);
	fifo_push(&mousebuf, data);
	//time.num++;
	return;
}

void inthandler27(int *esp)
{
	io_out8(PIC0_OCW2, 0x67); 
	return;
}


void inthandler20(int *esp)
{
	io_out8(PIC0_OCW2, 0x60); 
	time.num++;
	if (timesth.deadline[timesth.next] != time.num)
	{
		return;
	}
	fifo_push(&timebuf, timesth.job[timesth.next]);
	if (timesth.job[timesth.next] == 0)
	{
		time.second += 1;
		if (time.second == 60)
		{
			time.mintue += 1;
			time.second = 0;
			if (time.mintue == 60)
			{
				time.hour += 1;
				if (time.hour == 24)
				{
					time.hour = 0;
				}
				time.mintue = 0;
			}
		}
	}
	timesth.f[timesth.next] = 0;
	int cnt= 0xfffffff;
	int i;
	for (i = 1; i < 100; i++)
	{
		if (timesth.f[i] == 1)
		{
			if (cnt > timesth.deadline[i])
			{
				timesth.next = i;
			}
		}
	}
	return;
}

void init_pit()
{
	io_out8(PIT_CTRL, 0x34);
	io_out8(PIT_CNT0, 0x9c);
	io_out8(PIT_CNT0, 0x2e);
	time.num=0;
	time.hour = 0;
	time.mintue = 0;
	time.second = 0;
	return;
}

