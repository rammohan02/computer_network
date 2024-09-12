#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>

struct dvr{
	int dest[100], dist[100], hop[100];
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

		recv(connect, buff, sizeof(buff), 0);
		nodes = atoi(buff);
		printf("Received no:of nodes is %d\n", nodes);

		recv(connect, buff, sizeof(buff), 0);
		edges = atoi(buff);
		printf("Received no:of edges is %d\n", edges);

		recv(connect, route, sizeof(route), 0);

		//recv(connect, arr, sizeof(arr), 0);

		/*for(i=0; i<nodes; i++)
		{
			for(j=0; j<nodes; j++)
			{
				printf("%d ", arr[0].dist[i][j]);
			}
			printf("\n");
		}*/

		printf("\nInformation Received\n");
		for(i=0; i<nodes; i++)
		{
			printf("Table for router %d: \n", i+1);
			printf("Destination\tDistance\tNext Hop\n");
			for(j=0; j<nodes; j++)
			{
				route1[i].dist[j] = route[i].dist[j];
				route1[i].hop[j] = route[i].dist[j];
				printf("%d\t\t%d\t\t%d\n", j+1, route[i].dist[j], route[i].hop[j]+1);
			}
		}
		printf("\n");

		while(temp!=0)
		{
			temp=0;
			for(i=0; i<nodes; i++)
			{
				for(j=0; j<nodes; j++)
				{
					//printf("a\n");
					for(k=0; k<nodes; k++)
					{
						//printf("b\n");
						if((route[i].dist[j])>(route[i].dist[k]+route[k].dist[j]) )
						{
							//printf("S=%d D=%d h=%d dis=%d hp1=%d hp2=%d\n", i, j, k, route[i].dist[j], dist[i][k], route[k].dist[j]);
							route[i].dist[j]=route[i].dist[k]+route[k].dist[j];
							//printf("%d, %d, %d\n",i, j, route[i].dist[j]);
							if(route[i].hop[k]!=0)
							{
								route[i].hop[j]=route[i].hop[k];
							}
							else
							{
								route[i].hop[j]=k;
							}
							temp=1;
							//printf("c\n");
						}
					}
				}
			}
		}

		send(connect, route, sizeof(route), 0);




	}
}
