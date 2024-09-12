#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>

int main()
{
	int s_sock;
	char msg[256] = "You'r connected to server";
	
	s_sock = socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in server, other;
	
	memset(&server, 0, sizeof(server));
	memset(&other, 0, sizeof(other));
	
	server.sin_family = AF_INET;
	server.sin_port = htons(9009);
	server.sin_addr.s_addr = INADDR_ANY;
	
	bind(s_sock, (struct sockaddr *) &server, sizeof(server));
	
	listen(s_sock, 5);
	
	socklen_t add;
	add = sizeof(other);
	int client_socket = accept(s_sock, (struct sockaddr *) &other, &add);
	
	send(client_socket, msg, sizeof(msg), 0);
	
	close(s_sock);
}
