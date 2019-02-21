#include "kernel.h"



void fifo_init(struct FIFO *fifo,int size,unsigned char *add)
{
	fifo->add=add;
	fifo->head=0;
	fifo->last=0;
	fifo->size=size;
	fifo->num=0;
	return; 
}
int fifo_push(struct FIFO *fifo,unsigned char data)
{
	if(fifo->head==fifo->last+1)
	{
		//»º³åÇøÂú 
		return 1;
	}
	else
	{
		fifo->add[fifo->last]=data;
		fifo->last++;
		fifo->last%=fifo->size;
		fifo->num++; 
	}
	
	return 0;
}

int fifo_pop(struct FIFO *fifo)
{
	int data;
	if(fifo->head==fifo->last)
	{
		//»º³åÇø¿Õ 
		return -1;
	}
	else
	{
		data=fifo->add[fifo->head];
		fifo->head++;
		fifo->head%=fifo->size;
		fifo->num--;
	}
	
	return data;
}
///////////////////////////////////////////
void llfifo_init(struct llFIFO *fifo, int size, long long *add)
{
	fifo->add = add;
	fifo->head = 0;
	fifo->last = 0;
	fifo->size = size;
	fifo->num = 0;
	return;
}
int llfifo_push(struct llFIFO *fifo, long long data)
{
	if (fifo->head == fifo->last + 1)
	{
		//»º³åÇøÂú 
		return 1;
	}
	else
	{
		fifo->add[fifo->last] = data;
		fifo->last++;
		fifo->last %= fifo->size;
		fifo->num++;
	}

	return 0;
}
long long llfifo_pop(struct llFIFO *fifo)
{
	int data;
	if (fifo->head == fifo->last)
	{
		//»º³åÇø¿Õ 
		return -1;
	}
	else
	{
		data = fifo->add[fifo->head];
		fifo->head++;
		fifo->head %= fifo->size;
		fifo->num--;
	}

	return data;
}















