#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>

struct dvr{
	int dest[100], dist[100], hop[100], flag[100];
};

struct array{
	int dist[100][100];
};


int main()
{
	int recv_sock;
	struct sockaddr_in receiver, sender;
	
	recv_sock = socket(AF_INET, SOCK_STREAM, 0);
	
	memset(&receiver, 0, sizeof(receiver));
	memset(&sender, 0, sizeof(sender));
	
	receiver.sin_family = AF_INET;
	receiver.sin_port = htons(9001);
	receiver.sin_addr.s_addr = INADDR_ANY;
	
	bind(recv_sock, (struct sockaddr *) &receiver, sizeof(receiver));
	
	if(listen(recv_sock, 10)!=0)
	{
		printf("Listen failed\n");
	}
	else
	{
		printf("Listening\n");
	}
	
	socklen_t add;
	add = sizeof(sender);
	int connect = accept(recv_sock, (struct sockaddr *)&sender, &add);
	
	if(connect<0)
	{
		printf("Connection denied\n");
		return 0;
	}
	else
	{
		printf("Connection accepted\n");
		
		int dist[100][100],  nodes, edges, i, j, k, u, v, temp=1;
		struct dvr route[100], route1[100];
		struct array arr[100];
		char buff[256];
		
		
	}
}
