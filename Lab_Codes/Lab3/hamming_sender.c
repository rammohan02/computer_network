#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include<math.h>

void to_array(int array[], int org, int len)				//function to convert num to array
{
	int i, num;
	
	num = org;
	len=len-1;
	while(num!=0)
	{
		array[len] = num%10;
		num = num/10;
		len--;
	}
	
}

void to_char(char final_data[], int array[], int len)		//function to convert array to string
{
	int i, temp;
	char buff1[256];
	
	for(i=0; i<len; i++)
	{
		temp = array[i];
		sprintf(buff1, "%d", temp);
		//printf("%s", buff1);
		strcat(final_data, buff1);
	}
	
	//printf("%s\n", data);

}

int r_bit_cal(int data[], int len, int pos)					//to calculate the value of r-bits according to their position
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


int ham_code(int final_data[], int org[], int len)			//to encode the data
{
	int i, j, k, r, pos, r_bit;
	
	r=0;
	while((int) pow(2, r) < len+1+r)						//to find min R Value(i.e., min no:of parity bits)
	{
		r++;
	}
	
	int data[len+r];
	
	j=len-1, k=0;
	for(i=0; i<len+r; i++)									//to get the combined array with data and parity bits before finding parity bits
	{
		if(i== (int)pow(2, k)-1)
		{
			data[i]=0;
			k++;
		}
		else
		{
			data[i] = org[j];
			j--;
		}
	}
	
	for(i=0; i<r; i++)										//finding parity bits and filling in the data array
	{
		pos = (int)pow(2, i);
		
		r_bit = r_bit_cal(data, len+r, pos);
		
		data[pos-1]=r_bit;
	}
	
	/*for(int i=0; i<len+r; i++)
	{
		printf("%d", data[i]);
	}
	printf("\n");*/
	i=len+r-1;
	j=0;
	while(i>=0)												//to reverse the encoded value
	{
		final_data[j] = data[i];
		j++;
		i--;
	}
	return (len+r);
}

int main()
{
	int sock;
	struct sockaddr_in sender, receiver;
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	
	memset(&sender, 0, sizeof(sender));
	memset(&receiver, 0, sizeof(receiver));
	
	sender.sin_family = AF_INET;
	sender.sin_port = htons(9001);
	sender.sin_addr.s_addr = INADDR_ANY;
	
	
	int connection = connect(sock, (struct sockaddr *) &sender, sizeof(sender));
	
	if(connection==-1)
	{
		printf("Connection failed\n");
		return 0;
	}
	else
	{
		printf("connection established\n");
		
		int data[256], len_data, org, org_arr[100], final_length;
		char buff[256], final_data[256];
		
		printf("Enter length of data: ");
		scanf("%d", &len_data);
		
		sprintf(buff, "%d", len_data);
		send(sock, buff, sizeof(buff), 0);
		
		printf("Enter data: ");
		scanf("%d", &org);
		
		to_array(org_arr, org, len_data);
		
		/*for(int i=0; i<len_data; i++)
		{
			printf("%d", org_arr[i]);
		}
		printf("\n");*/
		final_length = ham_code(data, org_arr, len_data);
		
		/*for(int i=0; i<final_length; i++)
		{
			printf("%d", data[i]);
		}
		printf("\n");*/
		
		to_char(final_data, data, final_length);
		send(sock, final_data, sizeof(final_data), 0);
		printf("Data sent : %s\n", final_data);
		
		//printf("Pow: %d\n", (int) pow(2, 2));
		
		close(sock);
		return 0;
		
	}
	
	
	
}
