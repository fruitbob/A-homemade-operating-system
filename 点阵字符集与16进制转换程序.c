#include<cstdio>
#include<iostream>
#include <stdlib.h>
#include <string.h>
using namespace std;
int main()
{
	char buf[10];
	FILE *p;
	p=fopen("123.txt","r");
	FILE *out;
	int cou=0;
	out=fopen("out1.txt","w+"); 
	while(fgets(buf,10,p) != NULL)
	{
		if(buf[0]=='.'||buf[0]=='*')
		{
			cou++;
			int num=0;
			int k=1;
			for(int i=7;i>=0;i--)
			{
				if(buf[i]=='*') num=num+(1<<(7-i));
			}
			if(num<16)
			 fprintf(out,"0x0%X,",num); 
			else
			fprintf(out,"0x%X,",num);
			
		}
		//fprintf(out,"\n");
		
		/*else
		{
			fprintf(out,"%s\n",buf);
			
		}*/
		
	}
	cout<<cou;
 } 
