#include<stdio.h> 
#include "kernel.h"

extern struct time0 time;



struct timesth0 timesth;
void init_timesth()
{
	int i;
	for (i = 0; i < 500; i++)
	{
		timesth.f[i] = 0;
	}
	timesth.next = 0;
	timesth.deadline[0] = 0xffffff;
	return;
}
void init_timer(int job,int t)
{
	int i;
	
	for (i = 1; i < 500; i++)
	{
		if (timesth.f[i] == 0)
		{
			timesth.deadline[i] = t;
			timesth.f[i] = 1;
			timesth.job[i] = job;
			if (timesth.deadline[i] < timesth.deadline[timesth.next])
			{
				timesth.next = i;
			}
			break;
		}
	}
	
	return;
}




