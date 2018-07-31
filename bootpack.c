#include<stdio.h> 
#include "bootpack.h"


void HariMain(void)
{
	
	init_palette(); 
	init_gdtidt();
	init_pic(); 
	io_sti();
	
	int i;
	unsigned char *p;
	p=(char *)0xa0000;
	for(i=0;i<=0xaffff;i++)
	{
		*(p+i)=3;
	 } 
	 
	 char test[100];
	 sprintf(test,"this %d",i);
	 init_mouse16(p,160,50,320);
	 os_printfs(p,320,10,50,4,test);
	 
	 io_out8(PIC0_IMR, 0xf9); 
	io_out8(PIC1_IMR, 0xef); 
	
	 for(;;)
	 {
	 	io_hlt();
	 }

}








