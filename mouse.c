#include "kernel.h"

struct  mouse_read mouseread;

void enable_mouse()
{
	wait_KBC_sendready();
	io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);
	mouseread.flag=0;
	return; 
}
int mouse_decode(unsigned char data)
{
	if(mouseread.flag==0)
	{
		//0xfa
		mouseread.flag=1;
		return 0;
	}
	else if(mouseread.flag==1)
	{
		if((data&0xc8)==0x08)
		{
			mouseread.inf[0]=data;
			mouseread.flag=2;
			return 0;
		}
	
	}
	else if(mouseread.flag==2)
	{
		mouseread.inf[1]=data;
		mouseread.flag=3;
		return 0;
	}
	else if(mouseread.flag==3)
	{
		mouseread.inf[2]=data;
		mouseread.flag=1;
		mouseread.click=mouseread.inf[0]&0x07;
		mouseread.x=mouseread.inf[1];
		mouseread.y=mouseread.inf[2];
		if(mouseread.inf[0]&0x10)
		{
			mouseread.x|=0xffffff00;
		}
		if(mouseread.inf[0]&0x20)
		{
			mouseread.y|=0xffffff00;
		}
		mouseread.y=-mouseread.y;
		return 1;
	}
}
