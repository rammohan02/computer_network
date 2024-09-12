#include<stdio.h>
#include<stdlib.h>

struct dvr{
	int dest[100], dist[100], hop[100], flag[100];
};

//void dijkstra(int dist[100][100]

/*int min_dist(int src, int nodes, int dist[100][100], struct dvr route[])
{
	int ans = 1000000, min_index;
	
	for(int i=0; i<nodes; i++)
	{
		if(route[src].flag[i]==0 && route[src].dist[i]<=ans)
		{
			ans = route[src].dist[i];
			min_index = i;
		}
	}
	
	return min_index;
}*/

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
		
		printf("a i=%d, j=%d, min=%d\n", src, i, min);
			
		for(int l=0; l<nodes; l++)
		{
			printf("%d ", route[src].flag[l]);
		}
		printf("\n");
		
		for(int j=0; j<nodes; j++)
		{
			if(!route[src].flag[j] && route[min].dist[j]!=0 && dist[min]!=1000000 && dist[min]+route[min].dist[j]<dist[j])
			{
				printf("i=%d j=%d in_dist=%d dist=%d\n", src, j, dist[j], dist[min]+route[min].dist[j]);
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
		printf("%d ", route[src].dist[k]);
	}
	printf("\n");
	
}

int main()
{
	struct dvr route[100], routeini[100];
	int nodes, edges, dist[100][100], flag[100], i, j, k, u, v, temp=0;
	
	printf("Enter no of nodes: ");
	scanf("%d", &nodes);
	
	printf("The names of nodes will be given starting from 1 to %d\n", nodes);
	printf("Enter no of edges: ");
	scanf("%d", &edges);
	
	for(i=0; i<nodes; i++)
	{
		for(j=0; j<nodes; j++)
		{
			if(i==j)
			{
				route[i].hop[j]=i;
			}
			dist[i][j]=0;
			route[i].dist[j]=0;
		}
	}
	
	/*for(i=0; i<nodes; i++)
	{
		for(j=0; j<nodes; j++)
		{
			route[i].flag[j]=0;
		}
	}*/
	
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
		else if(dist[u-1][v-1]==0 && dist[v-1][u-1]==0)
		{
			dist[u-1][v-1]=dist[v-1][u-1]=k;
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
	
	for(i=0; i<nodes; i++)
	{
		for(j=0; j<nodes; j++)
		{
			if(i!=j && dist[i][j]==0)
			{
				route[i].dist[j]=1000000;
				route[i].hop[j]=-1;
				dist[i][j]=1000000;
			}
		}
	}
	
	printf("\nInitial routing tables\n");
	for(i=0; i<nodes; i++)
	{
		printf("Table for router %d: \n", i+1);
		printf("Destination\tDistance\tNext Hop\n");
		for(j=0; j<nodes; j++)
		{
			printf("%d\t\t%d\t\t%d\n", j+1, route[i].dist[j], route[i].hop[j]+1);
		}
	}
	
	/*for(i=0; i<nodes; i++)
	{
		
		for(j=0; j<nodes; j++)
		{
			int min = min_dist(i, nodes, dist, route);
			
			route[i].flag[min]=1;
			
			printf("a i=%d, j=%d, min=%d\n", i, j, min);
			
			for(int l=0; l<nodes; l++)
			{
				printf("%d ", route[i].flag[l]);
			}
			printf("\n");
			
			if(!route[i].flag[j] && route[min].dist[j]!=0 && route[i].dist[min]!=1000000 && route[i].dist[min]+route[min].dist[j] < route[i].dist[j])
			{
				printf("i=%d j=%d in_dist=%d dist=%d\n", i, j, route[i].dist[j], route[i].dist[min]+dist[min][j]);
				route[i].dist[j] = route[i].dist[min]+route[min].dist[j];
				route[i].hop[j] = min;
			}
		}
	}*/
	
	for(i=0; i<nodes; i++)
	{
		dijkstra(route, i, nodes);
	}
	
	printf("\nFinal routing tables\n");
	for(i=0; i<nodes; i++)
	{
		printf("Table for router %d: \n", i+1);
		printf("Destination\tDistance\tNext Hop\n");
		for(j=0; j<nodes; j++)
		{
			printf("%d\t\t%d\t\t%d\n", j+1, route[i].dist[j], route[i].hop[j]+1);
		}
	}
}
