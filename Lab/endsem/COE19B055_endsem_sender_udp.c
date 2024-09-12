#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<pthread.h>
#include<semaphore.h>

sem_t x;

int count;


void *create_client(void *id)
{
	
	int c_id = *((int *) id);
	//printf("%d", c_id);
	int c_sock;
	
	c_sock = socket(AF_INET, SOCK_DGRAM, 0);
	
	struct sockaddr_in sender, receiver;
	
	sender.sin_family = AF_INET;
	sender.sin_port = htons(9001);
	sender.sin_addr.s_addr = INADDR_ANY;
	sem_wait(&x);
	
	printf("Client %d - Count received from client %d is %d\n", c_id, c_id-1, count);
	
	count = count + 1;
	
	
	sem_post(&x);
	//close(c_sock);
	pthread_exit(NULL);
	
}

int main()
{
	sem_init(&x, 0, 1);
	int clients;
	int send_sock;
	char buff[1024];
	char msg[1024] = "I am client";
	pthread_t pid[100];
	
	printf("Enter no:of clients: ");
	scanf("%d", &clients);
	
	struct sockaddr_in sender, receiver;
	

	
	send_sock = socket(AF_INET, SOCK_DGRAM, 0);
	memset(&sender, 0, sizeof(sender));
	
	sender.sin_family = AF_INET;
	sender.sin_port = htons(9001);
	sender.sin_addr.s_addr = INADDR_ANY;

	socklen_t addr;
	
	sendto(send_sock, msg, sizeof(msg), 0, (struct sockaddr*)&sender, sizeof(sender));
	
	addr = sizeof(receiver);
	
	recvfrom(send_sock, buff, sizeof(buff), 0, (struct sockaddr *)&receiver, &addr);
	
	count = atoi(buff);
	
	printf("Client 1 - Count received from server is %d\n", count);
	
	count = count + 1;
	
	for(int i=1; i<clients; i++)
	{
		pthread_create(&pid[i], NULL, create_client, (void *)&i);
	}
	
	for(int i=1; i<clients; i++)
	{
		pthread_join(pid[i], NULL);
	}
	
	//printf("FInal count = %d\n", count);
	sprintf(buff, "%d", count);
	
	
	printf("Sending final count to server\n");
	
	sendto(send_sock, buff, sizeof(buff), 0, (struct sockaddr*)&sender, sizeof(sender));
	
	//close(send_sock);
	
	//sendto(send_sock, msg, sizeof(msg), 0, (struct sockaddr*)&sender, sizeof(sender));
	
	return 0;
}
