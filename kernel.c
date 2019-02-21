#include<stdio.h> 
#include<string.h>
#include "kernel.h"

extern struct FIFO keybuf, mousebuf, timebuf, cmdbuf;
extern struct  mouse_read mouseread;
extern struct mem_table table; 
extern struct time0 time;
extern struct timesth0 timesth;

static char keytable1[0x80] = {
	0,   0,   '!','@', '#', '$', '%', '^', '&', '*','(', ')', '-', '+', 0,   0,
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 0,   0,   'A', 'S',
	'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', 0,   0,   '|', 'Z', 'X', 'C', 'V',
	'B', 'N', 'M', '<', '>', '?', 0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', '+', '1',
	'2', '3', '0', '.', 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0,   '_', 0,   0,   0,   0,   0,   0,   0,   0,   0,   '|', 0,   0
};
static char keytable0[0x80] = {
		0,   0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0,   0,
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', 0,   0,   'A', 'S',
		'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', 0,   0,   '\\', 'Z', 'X', 'C', 'V',
		'B', 'N', 'M', ',', '.', '/', 0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0,
		0,   0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', '+', '1',
		'2', '3', '0', '.', 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		0,   0,   0,   0x5c, 0,  0,   0,   0,   0,   0,   0,   0,   0,   0x5c, 0,  0
};
void HariMain()
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	unsigned int i=0;
	unsigned int pm;
	unsigned char key_buf[32],c[50],mouse_buf[32], time_buf[500],cmd_buf[200];
	unsigned int k;
	int c_x=1,c_y=1; 
	int m_x=0,m_y=0;
	int cur_x, cur_y, cur_c, cur_f = 0;
	int cur_buf = -1;
	
	cur_c = 0;
	init_palette(); 
	init_gdtidt();
	init_pic(); 
	init_timesth();
	io_sti();
	fifo_init(&keybuf,32,key_buf);
	 fifo_init(&mousebuf,32,mouse_buf);
	 fifo_init(&timebuf, 500, time_buf);
	 fifo_init(&cmdbuf, 200, cmd_buf);
	 init_pit();
	

	 init_window(binfo);
	 io_out8(PIC0_IMR, 0xf8);
	 io_out8(PIC1_IMR, 0xef); 
	 
	 
	 
	 init_keyboard();
 	 init_mouse16(binfo,m_x,m_y,binfo->scrnx);
 	 
	 
	 enable_mouse();
	 int f_cmd = 0;
	//os_printfs(binfo->vram,binfo->scrnx,20,100,0,1,c,3);
	 int te = 8;
	 int key_shift = 0;
	 int key_leds = (binfo->leds >> 4) & 7;
	 pm = memtest(0x00400000, 0xbfffffff);
	 init_memtable(&table,0x00400000,pm-0x00400000); 
	 k=mem_alloc_4k(&table,1);
	 mem_free(&table, 0x00001000, 0x0009e000);
	 //sprintf(c, "add:%9d size_tol%9d size_good%9d i%9d a%10d",
		 //k, table.size /1024, table.size_good / 1024, pm/1024, &te);
	// os_printfs(binfo->vram, binfo->scrnx, c_x, c_y + 17, 0, 14, c, 100);
	

	 cur_x = 18;
	 cur_y = binfo->scrny / 5 * 4 + 2;
	 cur_c = 0;
	 init_timer(0, time.num + 150);
	 cur_f = 0;
	 
	
	 for(;;)
	 {
		 
		 //sprintf(c, "%10d  %10d", time.num, cur_buf);
		//os_draw_square(binfo->vram,binfo->scrnx,c_x,c_y+32,14,100,16) ;
		//os_printfs(binfo->vram,binfo->scrnx,c_x,c_y+32,0,14,c,22);
		change_back(binfo,m_x,m_y,binfo->scrnx) ;
		init_mouse16(binfo,m_x,m_y,binfo->scrnx);
	 	io_cli();
		if (keybuf.num ==0&& mousebuf.num==0 && timebuf.num==0)
		 {
			io_stihlt();
		} 
		else 
		{
			
			if(keybuf.num)
			{
				i = fifo_pop(&keybuf);
				io_sti();
				
				if (cur_f == 1)
				{
					if (i == 0x1c)//回车键
					{
						os_draw_square(binfo->vram, binfo->scrnx, cur_x, cur_y, 7, 13, 16);
						cur_y += 18;
						cur_x = 18;
						cur_out(binfo, &cur_y);
						
						
						os_printfs(binfo->vram, binfo->scrnx, 0, cur_y, 0, 7, ">>", 2);
						int op = 0;

						char cmd[205] = { 0 };
						char pa[205] = { 0 };
						int cut_f = 0;
						for (; cmdbuf.num; op++)
						{
							char k;
							k = fifo_pop(&cmdbuf);
							if (k == ' ')
							{
								
								cut_f = 1;
								op = -1;
								continue;
							}
							if (cut_f == 0)
							{
								cmd[op] = k;
							}
							else //if (cut_f = 1)
							{
								pa[op] = k;
								cut_f++;
							}
							
							
						}
						cut_f--;
						pa[op] = 0;
						
						//os_printfs(binfo->vram, binfo->scrnx, c_x, c_y + 52, 0, 14, cmd, 20);
						
						//os_printfs(binfo->vram, binfo->scrnx, c_x, c_y + 68, 0, 14, pa, 20);
						//api处理
						if (strcmp(cmd, "showtime") == 0)
						{
							sprintf(cmd, "The computer has been running for %d seconds. |  Now the time is %d : %d : %d  ", time.num/150, time.hour, time.mintue, time.second);
							api_showtime(cmd, binfo, time);
						}
						else if (strcmp(cmd, "cls") == 0)
						{
							//api_cls(cmd, binfo);
							dis_cmd(binfo);
							init_cmd(binfo);
							cur_x = 18;
							cur_y = binfo->scrny / 5 * 4 + 2;
						}
						else if (strcmp(cmd, "mem") == 0)
						{
							
							sprintf(cmd, " Recorded memory : %4d MB | Available memory : %4d MB | Total physical memory : %4d MB   ",
								table.size / 1024/1024, table.size_good / 1024/1024, pm / 1024/1024);
							api_mem(cmd, binfo);

							sprintf(cmd, "Warning : ");
							os_printfs(binfo->vram, binfo->scrnx, binfo->scrnx / 4, 130 + 14 * 16, 7, 1, cmd, 10);

							sprintf(cmd, "Recorded memory refers to the memory written to the memory");
							os_printfs(binfo->vram, binfo->scrnx, binfo->scrnx / 4 + 16, 130 + 15 * 16+8, 7, 1, cmd, 59);

							sprintf(cmd, "table.");
							os_printfs(binfo->vram, binfo->scrnx, binfo->scrnx / 4 , 130 + 16 * 16+8*2, 7, 1, cmd, 7);

							sprintf(cmd, "Available memory is allocatable memory.");
							os_printfs(binfo->vram, binfo->scrnx, binfo->scrnx / 4 + 16, 130 + 17 * 16+8*3, 7, 1, cmd, 40);
							
							sprintf(cmd, "Physical memory refers to the memory bar capacity.");
							os_printfs(binfo->vram, binfo->scrnx, binfo->scrnx / 4 + 16, 130 + 18 * 16+8*4, 7, 1, cmd, 51);
						}
						else if (strcmp(cmd, "time") == 0)
						{
							if (cut_f == 0 || pa[2] != ':' || pa[5] != ':')
							{
								sprintf(cmd, "   Error: incorrect parameters!!! ");
								os_printfs(binfo->vram, binfo->scrnx, 0, cur_y, 0, 7, cmd, 30);
								cur_y += 18;
								cur_out(binfo, &cur_y);
								sprintf(cmd, "   Example: time 08:15:13");
								os_printfs(binfo->vram, binfo->scrnx, 0, cur_y, 0, 7, cmd, 25);
								cur_y += 18;
								cur_out(binfo, &cur_y);
								cur_x = 18;
								os_printfs(binfo->vram, binfo->scrnx, 0, cur_y, 0, 7, ">>", 2);
							}
							else if (!api_time(pa, binfo, &time))
							{
								sprintf(cmd, "   Successfully set the time to %s", pa);
								os_printfs(binfo->vram, binfo->scrnx, 0, cur_y, 0, 7, cmd, 40);
								cur_y += 18;
								cur_out(binfo, &cur_y);
								os_printfs(binfo->vram, binfo->scrnx, 0, cur_y, 0, 7, ">>", 2);
							}
							else
							{
								sprintf(cmd, "   Error: incorrect parameters!!! ");
								os_printfs(binfo->vram, binfo->scrnx, 0, cur_y, 0, 7, cmd, 35);
								cur_y += 18;
								cur_out(binfo, &cur_y);
								sprintf(cmd, "   The parameters should not be greater than 60 !!!");
								os_printfs(binfo->vram, binfo->scrnx, 0, cur_y, 0, 7, cmd, 52);
								cur_y += 18;
								cur_out(binfo, &cur_y);
								cur_x = 18;
								os_printfs(binfo->vram, binfo->scrnx, 0, cur_y, 0, 7, ">>", 2);
							}
						}
						else if (strcmp(cmd, "settimer") == 0)
						{
							i = 3;
							int u_f = 1;
							
							if (cut_f == 0 )
							{
								u_f = 0;
								sprintf(cmd, "   Error: incorrect parameters!!! ");
								os_printfs(binfo->vram, binfo->scrnx, 0, cur_y, 0, 7, cmd, 35);
								cur_y += 18;
								cur_out(binfo, &cur_y);
								sprintf(cmd, "   Example: settimer job201/10");
								os_printfs(binfo->vram, binfo->scrnx, 0, cur_y, 0, 7, cmd, 31);
								cur_y += 18;
								cur_out(binfo, &cur_y);
								cur_x = 18;
								os_printfs(binfo->vram, binfo->scrnx, 0, cur_y, 0, 7, ">>", 2);
							}
							if (u_f)
							{
								if (!api_settimer(pa, cut_f,time.num))
								{
									sprintf(cmd, "   Error: incorrect parameters!!! The job number must be larger than 200");
									os_printfs(binfo->vram, binfo->scrnx, 0, cur_y, 0, 7, cmd, 73);
									cur_y += 18;
									cur_out(binfo, &cur_y);
									sprintf(cmd, "   Example: settimer job201/10");
									os_printfs(binfo->vram, binfo->scrnx, 0, cur_y, 0, 7, cmd, 31);
									cur_y += 18;
									cur_out(binfo, &cur_y);
									cur_x = 18;
									os_printfs(binfo->vram, binfo->scrnx, 0, cur_y, 0, 7, ">>", 2);
								}
								else
								{
									sprintf(cmd, "   Successfully set a timer for %s", pa);
									os_printfs(binfo->vram, binfo->scrnx, 0, cur_y, 0, 7, cmd, 35);
									cur_y += 18;
									cur_out(binfo, &cur_y);
									os_printfs(binfo->vram, binfo->scrnx, 0, cur_y, 0, 7, ">>", 2);
								}
							}


						}
						else if (strcmp(cmd, "window") == 0)
						{
							init_window(binfo);
							f_cmd = 0;
							cur_f = 0;

						}
						/*else if (strcmp(cmd, "calcu") == 0)
						{
							sprintf(cmd, "   Successfully %4f ", cal('1', '/', '1'));//api_calcu(pa, cut_f, binfo));
							os_printfs(binfo->vram, binfo->scrnx, 0, cur_y, 0, 7, cmd, 40);
							cur_y += 18;
							cur_out(binfo, &cur_y);
							os_printfs(binfo->vram, binfo->scrnx, 0, cur_y, 0, 7, ">>", 2);
							
						
						}*/
						
						
					}
					else if (i == 0x3a)//大小写转换
					{
						key_leds ^= 4;
					}
					else if (i == 0x2a)
					{
						key_shift |= 1;
					}
					else if (i == 0x36)
					{
						key_shift |= 2;
					}
					else if (i == 0xaa)
					{
						key_shift &= ~1;
					}
					else if (i == 0xb6)
					{
						key_shift &= ~2;
					}
					else if (i == 0x0e && cur_x > 18)//退格键
					{
						cur_x = cur_x - 9;
						os_draw_square(binfo->vram, binfo->scrnx, cur_x, cur_y, 7, 13, 16);
						cmdbuf.last--;
						cmdbuf.last %= cmdbuf.size;
						cmdbuf.num--;

					}else if (0x00 < i && i < 0x54)//字符输入
					{
						if (key_shift == 0)
						{
							c[0] = keytable0[i];
						}
						else
						{
							c[0]= keytable1[i];
						}
						
						
						if (((key_leds & 4) == 0 && key_shift == 0)|| ((key_leds & 4) != 0 && key_shift != 0))
						{
							if ((i <= 25 && i >= 16) || (i >= 30 && i <= 38) || (i >= 44 && i <= 50))
							{
								c[0] += 0x20;
							}
							
						}
						if (i == 0x39)
						{
							c[0] = ' ';
						}

						
						fifo_push(&cmdbuf, c[0]);
						os_printfs(binfo->vram, binfo->scrnx, cur_x , cur_y, 0, 7, c, 1);
						cur_x = cur_x + 9;
					} 
				}
				//sprintf(c, "  %02X   ", i);
				//c[0] = keytable0[i];
				//os_printfs(binfo->vram, binfo->scrnx, 0, c_y + 100, 0, 14, c, 20);
				
				
				
				
			}
			else if(mousebuf.num)
			{
				i = fifo_pop(&mousebuf);
				io_sti();
				//sprintf(c, "%4d %4d %4d ",mouseread.click ,m_x, m_y);
				
				if(mouse_decode(i)==1)
				{
					

					os_draw_square(binfo->vram,binfo->scrnx,20,1,14,200,16) ;
					//os_printfs(binfo->vram,binfo->scrnx,20,1,0,14,c,15);
					change_back(binfo,m_x,m_y,binfo->scrnx) ;
					m_x+=mouseread.x;
					m_y+=mouseread.y;
					if(m_x>binfo->scrnx-18) 
					{
						if(m_x<0) m_x=0;
						if(m_y<0) m_y=0;
						if(m_y>binfo->scrny-2) m_y=binfo->scrny-2;
						m_x=binfo->scrnx-18;
						init_mouse16(binfo,m_x,m_y,binfo->scrnx);
					}else
					{
						if(m_x<0) m_x=0;
						if(m_y<0) m_y=0;
						if(m_y>binfo->scrny-2) m_y=binfo->scrny-2;
						init_mouse16(binfo,m_x,m_y,binfo->scrnx);
					}
					
				}
				if (mouseread.click & 0x01)
				{
					c[1] = 'L';
					if (0 <= m_x && m_x <= 39 && m_y <= binfo->scrny&&m_y >= binfo->scrny - 17 && f_cmd == 1)
					{
						dis_cmd(binfo); 
						f_cmd = 0;  
						cur_f = 0;
						 
					}
					else if (0 <= m_x && m_x <= 39 && m_y <= binfo->scrny&&m_y >= binfo->scrny - 17 && f_cmd == 0)
					{
						init_cmd(binfo);
						f_cmd = 1;
						cur_x = 18;
						cur_y = binfo->scrny / 5 * 4 + 2;
						cur_f = 1;
						
					}

				}
				else if (mouseread.click & 0x02)
				{
					c[2] = 'R';
				}
				else if (mouseread.click & 0x04)
				{
					c[3] = 'C';
				}
			}
			//int k = 0xfffffff;
			//sprintf(c, "%d                        ", k);
			//os_printfs(binfo->vram, binfo->scrnx, c_x, c_y + 52, 0, 14, c, 22);
			
			//init_timer(0, time.num + 50, time);
			//init_cur(binfo, cur_x, cur_y, cur_c);
			if (timebuf.num)
			{
				int job = fifo_pop(&timebuf);
				
				
				if (job == 0 )
				{
					sprintf(c, "%2d:%2d:%2d ", time.hour, time.mintue, time.second);
					c[8] = 0;
					os_printfs(binfo->vram, binfo->scrnx, binfo->scrnx - 67, binfo->scrny - 17, 0, COL8_C6C6C6, c, 8);
					init_timer(0, time.num + 150);
					if (cur_f == 1)
					{
						cur_c = cur_c ^ 0 ^ 7;
						init_cur(binfo, cur_x, cur_y, cur_c);
						
					}
				
				}
				else if (job >= 200)//自定义计时器
				{
					sprintf(c, "*****************   Note: Task %4d timer arrives!!!    ***************** ", job);
					os_printfs(binfo->vram, binfo->scrnx, 0, cur_y, 0, 7, c, 74);
					cur_y += 18;
					cur_out(binfo, &cur_y);
					
					cur_x = 18;
					os_printfs(binfo->vram, binfo->scrnx, 0, cur_y, 0, 7, ">>", 2);
				}
			}
				
		}
	 }

}
 





