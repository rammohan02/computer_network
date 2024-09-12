#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<time.h>

typedef struct timer{
	time_t begTime;
}timer1;

unsigned long elapsed_time(time_t tim)
{
	return (time(NULL)-tim);
}

int main()
{
	timer1 t;
	t.begTime = time(NULL);
	unsigned long seconds = 5;
	int to = 24600 + rand()%(64000 - 24600)  + 1;
	        for(int i=0;i<64000;i++)
    	    	for(int j=0;j<to;j++) {}
	printf("%ld\n", elapsed_time(t.begTime));
}
