#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<time.h>

int count = 0;

int main()
{
	int rec_sock;
	char buff[1024];
	char msg[1024];
	
	struct sockaddr_in receiver, sender;
	
	rec_sock = socket(AF_INET, SOCK_DGRAM, 0);
	memset(&receiver, 0, sizeof(receiver));
	
	receiver.sin_family = AF_INET;
	receiver.sin_port = htons(9001);
	receiver.sin_addr.s_addr = INADDR_ANY;
	
	socklen_t addr;
	bind(rec_sock, (struct sockaddr *) &receiver, sizeof(receiver));
	
	addr = sizeof(sender);
	
	recvfrom(rec_sock, msg, sizeof(msg), 0, (struct sockaddr*)&sender, &addr);
	
	printf("Server msg: %s\n", msg);
	
	printf("Sending initial count as 0 to first client\n");
	sprintf(buff, "%d", count);
	sendto(rec_sock, buff, sizeof(buff), 0, (struct sockaddr*)&sender, addr);
	
	int x;
	//scanf("%d", &x);
	
	recvfrom(rec_sock, buff, sizeof(buff), 0, (struct sockaddr*)&sender, &addr);
	int final_count = atoi(buff);
	
	printf("Count received from last client is %d\n", final_count);
	
	//recvfrom(rec_sock, msg, sizeof(msg), 0, (struct sockaddr*)&sender, &addr);
	
	//printf("Server msg: %s\n", msg);
	
	return 0;
}

