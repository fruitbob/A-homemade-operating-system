#include "kernel.h"
#include<stdio.h> 

//extern struct time0 time;
void api_showtime(char *cmd, struct BOOTINFO *binfo, struct time0 time)
{
	//char c[100] = { 0 };
	//sprintf(c, "num:%d ||  hour:%d || minute:%d ||second:%d  0",time.num , time.hour, time.mintue, time.second);
	os_draw_square(binfo->vram, binfo->scrnx, 0, 0, 14, binfo->scrnx, binfo->scrny / 5 * 4 + 2);
	int i;
	for (i = 0;; i++)
	{
		if (cmd[i] == '|')
		{
			break;
		}
	}
	os_draw_square(binfo->vram, binfo->scrnx, binfo->scrnx / 4, binfo->scrny / 6, 1, binfo->scrnx / 4 * 2, binfo->scrny / 4 * 2);
	os_printfs(binfo->vram, binfo->scrnx, binfo->scrnx / 4, 130+5*16, 7, 1, cmd, i);
	os_printfs(binfo->vram, binfo->scrnx, binfo->scrnx / 4, 166+5*16, 7, 1, cmd + i+1, 30);
	
}


void api_cls(char *cmd,struct BOOTINFO *binfo)
//未使用的函数
{
	//os_printfs(binfo->vram, binfo->scrnx, 0, 120, 0, 14, cmd, 100);
	os_draw_square(binfo->vram, binfo->scrnx, 0, binfo->scrny / 5 * 4 + 2, 7, binfo->scrnx, binfo->scrny / 5 - 16);
	os_printfs(binfo->vram, binfo->scrnx, 0, binfo->scrny / 5 * 4 + 4, 0, 7, ">>", 2);
	os_printfs(binfo->vram, binfo->scrnx, 0, binfo->scrny - 17, 0, COL8_00FFFF, "BASIC ", 5);
}



void api_mem(char *cmd, struct BOOTINFO *binfo)
{
	//os_printfs(binfo->vram, binfo->scrnx, 0, 120, 0, 14, cmd, 100);
	os_draw_square(binfo->vram, binfo->scrnx, 0, 0, 14, binfo->scrnx, binfo->scrny / 5 * 4 + 2);
	int i,j;
	for (i = 0;; i++)
	{
		if (cmd[i] == '|')
		{
			break;
		}
	}
	for (j = i+1;; j++)
	{
		if (cmd[j] == '|')
		{
			break;
		}
	}
	os_draw_square(binfo->vram, binfo->scrnx, binfo->scrnx / 4, binfo->scrny / 6, 1, binfo->scrnx / 4 * 2, binfo->scrny / 4 * 2);
	os_printfs(binfo->vram, binfo->scrnx, binfo->scrnx / 4+8, 130 + 5 * 16, 7, 1, cmd+1, i-1);
	os_printfs(binfo->vram, binfo->scrnx, binfo->scrnx / 4, 130 + 8 * 16, 7, 1, cmd + i + 1, j-i-1);
	os_printfs(binfo->vram, binfo->scrnx, binfo->scrnx / 4, 130 + 11 * 16, 7, 1, cmd + j + 1, 33);
	
	//os_printfs(binfo->vram, binfo->scrnx, binfo->scrnx / 4+16, 130 + 15 * 16, 7, 1, 'Warning : ', 10);

}


int api_time(char *cmd, struct BOOTINFO *binfo, struct time0 *time)
{
	int a1 =(int) (cmd[1] - '0');
	//a1 +=(int) (cmd[0] - '0') * 10;
	a1 = ctn(cmd, 2);
	if (a1 >= 60)
	{
		return -1;
	}
	
	time->hour = a1;
	//a1 = (int)(cmd[4] - '0');
	//a1 += (int)(cmd[3] - '0' )* 10;
	a1 = ctn(cmd + 3, 2);
	if (a1 >= 60)
	{
		return -1;
	}
	time->mintue = a1;
	a1 = ctn(cmd + 6, 2);
	//a1 = (int)(cmd[7] - '0');
	//a1 += (int)(cmd[6] - '0' )* 10;
	if (a1 >= 60)
	{
		return -1;
	}
	time->second = a1;
	return 0;
}
int ctn(char *s, int len)
//用于字符串转数值
{
	int a, i;
	int k = (int)(s[0] - '0');
	for (i = 1, a = k; i < len; i++)
	{
		k = (int)(s[i] - '0');
		a = a * 10 + k;
	}
	return a;
}
int is_num(char c)
{
	int k = (int)(c - '0');
	if (k < 0 || k>9)
	{
		return 0;
	}
	return 1;
}

int api_settimer(char *cmd,int len,int num)
{
	int i;
	int cut = 0;
	int f = 1;
	if (cmd[0] != "j"|| cmd[1] != "o" || cmd[2] != "b")
	{
		//f = 0;
	}
	for (i = 3; i < len; i++)
	{
		if (!is_num(cmd[i]))
		{
			cut = i;
		}
	}
	if (cut == 0)
	{
		f = 0;
	}
	if (f)
	{
		int job = ctn(cmd + 3, cut - 3);
		int t = ctn(cmd + cut + 1, len - cut - 1);
		if (job < 200)
		{
			return 0;
		}
		init_timer(job, t*150+num);
		return 1;
	}
	return 0;
}

float api_calcu(char *cmd, int len, struct BOOTINFO *binfo)
{
	int i = 1;
	float re = 0;
	char s1[100] = { 0 };
	int f[200] ;
	int top1 = 0;
	s1[top1] = (char)ctn(cmd , 1);
	f[top1] = 1;
	top1++;
	if (!is_num(cmd[0]))
	{
		return 0;
	}
	char s2[100] = { 0 };
	int top2 = 0;
	for (; i < len; i++)
	{
		if (is_num(cmd[i]))
		{
			if (is_num(cmd[i - 1]))
			{
				top1--;
				int k = (int)s1[top1];
				k = k * 10 + ctn(cmd + i, 1);
				s1[top1] = (char)k;
				top1++;
			}
			else
			{
				s1[top1] = (char)ctn(cmd + i, 1);
				f[top1] = 1;
				top1++;
			}
		}
		else
		{
			if (cmd[i] == '(' )
			{
				s2[top2] = cmd[i];
				top2++;
			}
			else if (cmd[i] == ')')
			{
				while (s2[top2-1] != '(')
				{
					top2--;
					s1[top1] = (char)cal(s1[top1 - 2], s2[top2], s1[top1 - 1]);//

					//s1[top1] = s2[top2];
					
					//f[top1] = 2;
					top1--;
					
					
				}
				
				top2--;
			}
			else if (top2 == 0)
			{
				s2[top2] = cmd[i];
				top2++;
			}
			else if (priority(cmd[i]) > priority(s2[top2-1]))
			{
				s2[top2] = cmd[i];
				top2++;
			}
			else if (priority(cmd[i]) <= priority(s2[top2 - 1]))
			{
				while (priority(cmd[i]) <= priority(s2[top2-1]))
				{
					top2--;
					s1[top1] = (char)cal(s1[top1 - 2], s2[top2], s1[top1 - 1]);
					//s1[top1] = s2[top2];
					//f[top1] = 2;
					top1--;
					
				}
				s2[top2] = cmd[i];
				top2++;
			}
			
		}
	}
	
	while (top2 > 0)
	{
		top2--;
		s1[top1] = (char)cal(s1[top1 - 2], s2[top2], s1[top1 - 1]);
		//s1[top1] = s2[top2];
		top1--;
	}
	//os_draw_square(binfo->vram,binfo->scrnx,c_x,c_y+32,14,100,16) ;
	os_printfs(binfo->vram,binfo->scrnx,0,0+32,0,14,s1,50);
	sprintf(s1, "%c ",s1[0]);
	os_printfs(binfo->vram, binfo->scrnx, 0, 0 + 32*2, 0, 14, s1, 1);
	re = (float)s1[0];
	return re;

}
int priority(char c)
{
	if (c == '(' || c == ')')
	{
		return 0;
	}
	if (c == '+' || c == '-')
	{
		return 1;
	}
	if (c == '*' || c == '/')
	{
		return 2;
	}
}
float cal(char a, char c, char b)
{
	
	if (c == '+') return (float)a + (float)b;
	if (c == '-') return (float)a - (float)b;
	if (c == '*') return (float)a * (float)b;
	if (c == '/') return (float)a / (float)b;
}