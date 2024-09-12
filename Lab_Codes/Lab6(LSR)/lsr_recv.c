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


int min_dist(int dist[], int nodes, int src, struct dvr route[])
{
	int min=1000000, min_index;
	
	for(int i=0; i<nodes; i++)
	{
		if(route[src].flag[i]==0 && dist[i]<=min)
		{
			min = dist[i];
			min_index=i;
		}
	}
	return min_index;
}


void dijkstra(struct dvr route[], int src, int nodes)
{
	int dist[100];
	
	
	for(int i=0; i<nodes; i++)
	{
		dist[i]=1000000;
		route[src].flag[i]=0;
	}
	
	dist[src]=0;
	
	
	for(int i=0; i<nodes-1; i++)
	{
		int min = min_dist(dist, nodes, src, route);
		
		route[src].flag[min]=1;
		
		//printf("a i=%d, j=%d, min=%d\n", src, i, min);
			
		/*for(int l=0; l<nodes; l++)
		{
			printf("%d ", route[src].flag[l]);
		}
		printf("\n");*/
		
		for(int j=0; j<nodes; j++)
		{
			if(!route[src].flag[j] && route[min].dist[j]!=0 && dist[min]!=1000000 && dist[min]+route[min].dist[j]<dist[j])
			{
				//printf("i=%d j=%d in_dist=%d dist=%d\n", src, j, dist[j], dist[min]+route[min].dist[j]);
				dist[j] = dist[min]+route[min].dist[j];
				//route[src].dist[j]=dist[j];
				if(route[src].dist[j]!=1000000 && src!=j)
				{
					route[src].hop[j]=j;
				}
				else
				{
					route[src].hop[j]=min;
				}
			}
		}
		
	}
	
	for(int k=0; k<nodes; k++)
	{
		route[src].dist[k] = dist[k];
		//printf("%d ", route[src].dist[k]);
	}
	//printf("\n");
	
}



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
		
		for(i=0; i<nodes; i++)
		{
			dijkstra(route, i, nodes);
		}
		
		send(connect, route, sizeof(route), 0);
		
		
		
		
	}
}
