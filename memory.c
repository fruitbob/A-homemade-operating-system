#include "kernel.h"


struct mem_table table; 

void init_gdtidt(void)
{
	struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *) ADR_GDT;
	struct GATE_DESCRIPTOR    *idt = (struct GATE_DESCRIPTOR    *) ADR_IDT;
	int i;

	/* GDTÇÃèâä˙âª */
	for (i = 0; i <= LIMIT_GDT / 8; i++) {
		set_segmdesc(gdt + i, 0, 0, 0);
	}
	set_segmdesc(gdt + 1, 0xffffffff,   0x00000000, AR_DATA32_RW);
	set_segmdesc(gdt + 2, LIMIT_BOTPAK, ADR_BOTPAK, AR_CODE32_ER);
	load_gdtr(LIMIT_GDT, ADR_GDT);

	/* IDTÇÃèâä˙âª */
	for (i = 0; i <= LIMIT_IDT / 8; i++) {
		set_gatedesc(idt + i, 0, 0, 0);
	}
	load_idtr(LIMIT_IDT, ADR_IDT);

	
	set_gatedesc(idt + 0x20, (int) asm_inthandler20, 2 * 8, AR_INTGATE32);
	set_gatedesc(idt + 0x21, (int) asm_inthandler21, 2 * 8, AR_INTGATE32);
	set_gatedesc(idt + 0x27, (int) asm_inthandler27, 2 * 8, AR_INTGATE32);
	set_gatedesc(idt + 0x2c, (int) asm_inthandler2c, 2 * 8, AR_INTGATE32);
   
	return;
}

void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar)
{
	if (limit > 0xfffff) {
		ar |= 0x8000; /* G_bit = 1 */
		limit /= 0x1000;
	}
	sd->limit_low    = limit & 0xffff;
	sd->base_low     = base & 0xffff;
	sd->base_mid     = (base >> 16) & 0xff;
	sd->access_right = ar & 0xff;
	sd->limit_high   = ((limit >> 16) & 0x0f) | ((ar >> 8) & 0xf0);
	sd->base_high    = (base >> 24) & 0xff;
	return;
}

void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar)
{
	gd->offset_low   = offset & 0xffff;
	gd->selector     = selector;
	gd->dw_count     = (ar >> 8) & 0xff;
	gd->access_right = ar & 0xff;
	gd->offset_high  = (offset >> 16) & 0xffff;
	return;
}

unsigned int memtest(unsigned int start, unsigned int end)
{
	char flg486 = 0;
	unsigned int eflg, cr0, i;

	
	eflg = io_load_eflags();
	eflg |= EFLAGS_AC_BIT; 
	io_store_eflags(eflg);
	eflg = io_load_eflags();
	if ((eflg & EFLAGS_AC_BIT) != 0) { 
		flg486 = 1;
	}
	eflg &= ~EFLAGS_AC_BIT; 
	io_store_eflags(eflg);

	if (flg486 != 0) {
		cr0 = load_cr0();
		cr0 |= CR0_CACHE_DISABLE; 
		store_cr0(cr0);
	}

	i = memtest_sub(start, end);

	if (flg486 != 0) {
		cr0 = load_cr0();
		cr0 &= ~CR0_CACHE_DISABLE; 
		store_cr0(cr0);
	}

	return i;
}
/*
unsigned int memtest_sub(unsigned int start, unsigned int end)
{
	struct dubug
	{

	};
	unsigned int i,old,*op,k=0xfffffff0,add1=0x0000000f;
	char c[50];
	int y=30;
	for(i=start;i<=end-0x1000;i+=0x1000)
	{

		op=(unsigned int *) (i);
		old=*op;
		k=k+add1;
		*op=0xff00ff00+0x00ff00ff;
		sprintf(c, "%x %x", add1,k);

		if(*op!=k)
		{

			*op=old;
			break;

		}
		*op=old;
		add1+=0x00000001;
		k=0xffffffff-add1;
		sprintf(c, "%x %x", add1,k);
	}
	return i;
}

*/



void init_memtable(struct mem_table *table,unsigned int add,unsigned int size)
{
	table->flag=0;
	table->size=size;
	table->size_good=size;
	table->node[0].size=size;
	table->node[0].add=add;
	int i;
	for(i=1;i<4000;i++)
	{
		table->node[i].size=0;
		table->node[i].add=0;
		
	}
	return;
}
unsigned int mem_alloc(struct mem_table *table,unsigned int size)
{
	unsigned int i,balance=0xffffffff;
	int now=-1;
	for(i=0;i<4000;i++)
	{
		int k=table->node[i].size-size;
		if(k>=0&&k<balance)
		{
			now=i;
			if(k==0) break;
		}
	}
	if(now==-1) return 0x0;
	table->node[now].size-=size;
	table->node[now].add+=size;
	table->size-=size;
	table->size_good-=size;
	return table->node[now].add-size;
}

int mem_free(struct mem_table *table,unsigned int add,unsigned int size)
{
	unsigned int i,balance=0xffffffff,now=0;
	unsigned int k1=add+size;
	int flag=1;
	for(i=0;i<4000;i++)
	{
		unsigned int k2=table->node[i].add+size;
		if(k2==add)
		{
			table->node[i].size+=size;
			return 1;
		}
		else if(k1==table->node[i].add)
		{
			table->node[i].size+=size;
			table->node[i].add=add;
			return 2;
		}
	}
	for(i=0;i<4000;i++)
	{
		if(table->node[i].size<balance)
		{
			now=i;
			if(table->node[i].size==0)
			{
				flag=0;
				break;
			}
		}
	}
	if(flag==1)  
	{
		table->flag=1;
		table->size_good-=table->node[now].size;
	}
	else
	{
		table->size_good+=size;
	}
	table->size+=size;
	table->node[now].size=size;
	table->node[now].add=add;
	return now;
}


unsigned int mem_alloc_4k(struct mem_table *table,unsigned int size)
{
	size=(size+0xfff)&0xfffff000;
	return mem_alloc(table,size);
}

int mem_free_4k(struct mem_table *table,unsigned int add,unsigned int size)
{
	size=(size+0xfff)&0xfffff000;
	return mem_free(table,add,size);
}



