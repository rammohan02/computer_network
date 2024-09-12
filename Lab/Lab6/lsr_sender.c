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
		
		printf("Enter no of nodes: ");
		scanf("%d", &nodes);
		
		sprintf(buff, "%d", nodes);
		send(send_sock, buff, sizeof(buff), 0);
		
		printf("The names of nodes will be given starting from 1 to %d\n", nodes);
		printf("Enter no of edges: ");
		scanf("%d", &edges);
		
		sprintf(buff, "%d", edges);
		send(send_sock, buff, sizeof(buff), 0);
		
		for(i=0; i<nodes; i++)
		{
			for(j=0; j<nodes; j++)
			{
				if(i==j)
				{
					route[i].hop[j]=i;
				}
				arr[0].dist[i][j]=0;
				route[i].dist[j]=0;
			}
		}
		
		//taking edges information
		printf("Enter details of nodes which are directly connected only\n");
		for(i=0; i<edges; i++)
		{
			printf("Edge %d :Enter the nodes and the weight(eg:1 2 4): ", i+1);
			scanf("%d %d %d", &u, &v, &k);
			
			if(u==v)
			{
				printf("No edge should be there between %d and %d\n", u, v);
				i--;	
			}
			else if(arr[0].dist[u-1][v-1]==0 && arr[0].dist[v-1][u-1]==0)
			{
				arr[0].dist[u-1][v-1]=arr[0].dist[v-1][u-1]=k;
				route[u-1].dist[v-1]=route[v-1].dist[u-1]=k;
				route[u-1].hop[v-1]=v-1;
				route[v-1].hop[u-1]=u-1;
			}	
			else
			{
				printf("Edge details already given.Enter a different edge\n");
				i--;
			}
		}
		
		//Making non-direct edges as infinite
		for(i=0; i<nodes; i++)
		{
			for(j=0; j<nodes; j++)
			{
				if(i!=j && arr[0].dist[i][j]==0)
				{
					route[i].dist[j]=1000000;
					route[i].hop[j]=-1;
					arr[0].dist[i][j]=1000000;
				}
			}
		}
		
		/*for(i=0; i<nodes; i++)
		{
			for(j=0; j<nodes; j++)
			{
				printf("%d ", arr[0].dist[i][j]);
			}
			printf("\n");
		}*/
		
		printf("Sending information...\n");
		
		
		send(send_sock, route, sizeof(route), 0);
		
		//send(send_sock, arr, sizeof(arr), 0);
		
		recv(send_sock, route1, sizeof(route1), 0);
		
		printf("\nFinal routing tables received\n");
		for(i=0; i<nodes; i++)
		{
			printf("Table for router %d: \n", i+1);
			printf("Destination\tDistance\tNext Hop\n");
			for(j=0; j<nodes; j++)
			{
				printf("%d\t\t%d\t\t%d\n", j+1, route1[i].dist[j], route1[i].hop[j]+1);
			}
		}
		
	}
}
