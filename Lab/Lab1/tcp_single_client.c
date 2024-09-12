#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>

int main()
{
	int c_sock;
	char msg[256];
	
	c_sock = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in client;
	memset(&client, 0, sizeof(client));
	client.sin_family = AF_INET;
	client.sin_port = htons(9009);
	client.sin_addr.s_addr = INADDR_ANY;
	
	int connection = connect(c_sock, (struct sockaddr *) &client, sizeof(client));
	
	if(connection == -1)
	{
		printf("Not connected");
		return 0;
	}
	
	recv(c_sock, msg, sizeof(msg), 0);
	printf("server sent: %s\n", msg);
	close(c_sock);

	return 0;
}
