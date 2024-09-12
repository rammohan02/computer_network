#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>

int main()
{
	int s_sock;
	//char msg[256] = "You'r connected to server";
	char msg[256];
	int n=0;
	char buf[100];

	s_sock = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server, other;

	memset(&server, 0, sizeof(server));
	memset(&other, 0, sizeof(other));

	server.sin_family = AF_INET;
	server.sin_port = htons(9009);
	server.sin_addr.s_addr = INADDR_ANY;

	bind(s_sock, (struct sockaddr *) &server, sizeof(server));


	if(listen(s_sock, 5)!=0) {
		printf("Listen failed\n");
		return 0;
	} else {
		printf("Listening\n");
	}

	socklen_t add;
	add = sizeof(other);
	int client_socket = accept(s_sock, (struct sockaddr *) &other, &add);

	if(client_socket <=0 ) {
		printf("Connction denied\n");
		return 0;
	} else {
		printf("Connection accepted\n");
	}


	recv(client_socket, buf, sizeof(buf), 0);
	printf("From Client : %s", buf);

	printf("Enter message: ");
	while((msg[n++] = getchar())!='\n')
	{}
	send(client_socket, msg, sizeof(msg), 0);

	close(s_sock);
}
