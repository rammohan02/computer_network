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
	int send_sock;
	
	struct sockaddr_in sender, receiver;
	
	//AF_INET: basic tranfer protocol
	send_sock = socket(AF_INET, SOCK_STREAM, 0);
	
	memset(&sender, 0, sizeof(sender));
	memset(&receiver, 0, sizeof(receiver));
	
	sender.sin_family = AF_INET;
	sender.sin_port = htons(9001);
	sender.sin_addr.s_addr = INADDR_ANY;
	
	int connection = connect(send_sock, (struct sockaddr *) &sender, sizeof(sender));
	
	if(connection == -1)
	{
		printf("Connection failed\n");
		return 0;
	}
	else
	{
		printf("Connection established\n");
		
		int nodes, edges, dist[100][100], flag[100], i, j, k, u, v, temp=0;
		struct dvr route[100], route1[100];
		struct array arr[100];
		char buff[256];
		
		
		
	}
}
