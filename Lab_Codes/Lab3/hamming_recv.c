#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include<math.h>

void to_array(int data[], char buff[], int len)
{
	int i, temp, j=0;
	
	for(i=len-1; i>=0; i--)
	{
		data[j] = buff[i]-48;
		j++;
	}
}

int data_error(int data[], int len, int pos)										//returns 1 if error or 0
{
	int i, j, count=0;
	
	i=pos-1;
	
	while(i<len)
	{
		for(j=i; j<pos+i; j++)
		{
			if(data[j]==1)
			{
				count++;
			}
		}
		i=i+2*pos;
	}
	
	if(count%2==0){
		return 0;
	}else{
		return 1;
	}
	
}

int cal_error_pos(int pos_error[], int len)									//calculating position of error by converting binary to decimal
{
	int pos=0, i=0;
	
	for(i=0; i<len; i++)
	{
		pos = pos + (pos_error[i] * (int)pow(2, i));
	}
	
	return pos;
}

void ham_decode(int final_data[], int data[], int len_data, int r)
{
	int i, pos, pos_error[r], j=0, k, error=0, error_pos ;
	
	for(i=0; i<r; i++)														//to check whether error is there or not
	{
		pos = (int) pow(2, i);
		
		pos_error[i] = data_error(data, len_data+r, pos);
	}
	
	for(j=0; j<r; j++)
	{
		//printf("%d ", pos_error[j]);
		if(pos_error[j] == 1)
		{
			error=1;
			break;
		}
	}
	
	if(error==1)
	{
		error_pos = cal_error_pos(pos_error, r);												//getting position from error bits
	
		//printf("error ->%d\n", error_pos);
		j=0, k=0;
		for(i=0; i<len_data+r; i++)
		{
			if(i == (int)pow(2, k)-1)
			{
				k++;
			}
			else
			{
				if(error_pos == i)
				{
					final_data[j] = (data[i]==1)? 0 : 1;										//if it is the position of error bit then replcaing with its converse
					j++;
				}
				else
				{
					final_data[j] = data[i];
					j++;
				}
			}
		}
	}
	else
	{
		j=0, k=0;
		for(i=0; i<len_data+r; i++)
		{
			if(i == (int)pow(2, k)-1)
			{
				k++;
			}
			else
			{
				final_data[j] = data[i];
				j++;
			}
		}
	}
	
	if(error==1)
	{
		printf("Found error in data at position %d\n", error_pos);
	}
	else
	{
		printf("No error detected\n");
	}
}

int main()
{
	int r_sock;
	
	struct sockaddr_in receiver, sender;
	
	r_sock = socket(AF_INET, SOCK_STREAM, 0);
	
	memset(&receiver, 0, sizeof(receiver));
	memset(&sender, 0, sizeof(sender));
	
	receiver.sin_family = AF_INET;
	receiver.sin_port = htons(9001);
	receiver.sin_addr.s_addr = INADDR_ANY;
	
	bind(r_sock, (struct sockaddr*) &receiver, sizeof(receiver));
	
	if(listen(r_sock, 10)!=0)
	{
		printf("Listen failed\n");
	}
	else
	{
		printf("listening\n");
	}
	
	socklen_t add;
	add = sizeof(sender);
	int sender_connect = accept(r_sock, (struct sockaddr *) &sender, &add);
	
	if(sender_connect <= 0)
	{
		printf("connection denied\n");
		return 0;
	}
	else
	{
		printf("connection accepted\n");
		
		char buff[256];
		int data[256], final_data[256], len_data, r, i;
		
		recv(sender_connect, buff, sizeof(buff), 0);
		
		len_data = atoi(buff);
		printf("Received length of data is: %d\n", len_data);
		
		recv(sender_connect, buff, sizeof(buff), 0);
		//data = atoi(buff);
		printf("Received data is: %s\n", buff);

		r=0;
		while((int) pow(2, r) < len_data+1+r)						//to find min R Value(i.e., min no:of parity bits)
		{
			r++;
		}
		
		to_array(data, buff, len_data+r);
		
		/*for(i=0; i<len_data+r; i++)
		{
			printf("%d ", data[i]);
		}
		printf("\n");*/
		
		ham_decode(final_data, data, len_data, r);
		
		printf("Decoded data: ");
		for(i=len_data-1; i>=0; i--)
		{
			printf("%d", final_data[i]);
		}
		printf("\n");
		
		close(r_sock);
		return 0;
		
		
	}
	
	
	
}
