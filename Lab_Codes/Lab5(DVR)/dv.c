#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

struct dvr{
	int dest[100], dist[100], hop[100];
};

int main()
{
	struct dvr route[100], routeini[100];
	int nodes, edges, dist[100][100], i, j, k, u, v, temp=0;
	
	printf("Enter no of nodes: ");
	scanf("%d", &nodes);
	
	printf("The names of nodes will be given starting from 1 to %d\n", nodes);
	/*printf("Enter no of edges: ");
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
	
	//Making non-direct edges as infinite
//	for(i=0; i<nodes; i++)
//	{
//		for(j=0; j<nodes; j++)
//		{
//			if(i!=j && dist[i][j]==0)
//			{
//				dist[i][j]=INT_MAX;
//			}
//		}
//	}
	
	for(i=0; i<nodes; i++)
	{
		printf("\t%d", i+1);
	}
	printf("\n");
	for(i=0; i<nodes; i++)
	{
		printf("%d\t", i+1);
		for(j=0; j<nodes; j++)
		{
			printf("%d\t", dist[i][j]);
		}
		printf("\n");
	}*/
	
	printf("Enter edge weights for nodes shown below(If both source and dest is same put 0\n");
	for(i=0; i<nodes; i++)
	{
		for(j=0; j<nodes; j++)
		{
			printf("Weight between %d and %d: ", i+1, j+1);
			scanf("%d", &dist[i][j]);
			route[i].dist[j]=dist[i][j];
			route[i].hop[j]=j;
		}
	}
	//Making non-direct edges as infinite
	for(i=0; i<nodes; i++)
	{
		for(j=0; j<nodes; j++)
		{
			if(i!=j && dist[i][j]==0)
			{
				route[i].dist[j]=1000000;
				dist[i][j]=1000000;
			}
		}
	}
	
	do
	{
		temp=0;
		for(i=0; i<nodes; i++)
		{
			for(j=0; j<nodes; j++)
			{
				for(k=0; k<nodes; k++)
				{
					if((route[i].dist[j])>(route[i].dist[k]+route[k].dist[j]) && dist[i][j]==1000000)
					{
						printf("S=%d D=%d h=%d dis=%d hp1=%d hp2=%d\n", i, j, k, route[i].dist[j], dist[i][k], route[k].dist[j]);
						route[i].dist[j]=dist[i][k]+route[k].dist[j];
						printf("%d, %d, %d\n",i, j, route[i].dist[j]);
						route[i].hop[j]=k;
						temp=1;
					}
				}
			}
		}
	}while(temp!=0);
	
	
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
		printf("\t%d", i+1);
	}
	printf("\n");
	for(i=0; i<nodes; i++)
	{
		printf("%d\t", i+1);
		for(j=0; j<nodes; j++)
		{
			printf("%d\t", dist[i][j]);
		}
		printf("\n");
	}*/
	
}
