#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>

void mod2div(int ret[], int str1[], int len, int str2[])
{
	int i, j;
	
	j=0;
	for(i=1; i<len; i++)
	{
		if(str1[i] == str2[i])
		{
			ret[j] = 0;
		}
		else
		{
			ret[j] = 1;
		}
		j++;
	}
}

int crc_cal(int remainder[], int crc[], int data[], int len_crc, int  len_data)
{
	int i, temp, diff[256], diff_len;
	
	temp = len_crc;
	
	diff_len=0;
	for(i=0; i<temp; i++)
	{
		diff[diff_len] = data[i];
		diff_len++;
	}
	
	int diff_temp[temp+1], zeroes[temp+1];
	
	for(i=0; i<temp; i++)														//Zeroes array is usedwhen remainder is less than divisior
	{
		zeroes[i] = 0;
	}
	
	while(temp<len_data)
	{
		if(diff[0] == 1)
		{
			mod2div(diff_temp, diff, diff_len, crc);
			diff_temp[diff_len-1] = data[temp];
			for(i=0; i<diff_len; i++)
			{
				diff[i] = diff_temp[i];
			}
		}
		else
		{
			mod2div(diff_temp, diff, diff_len, zeroes);
			diff_temp[diff_len-1] = data[temp];
			for(i=0; i<diff_len; i++)
			{
				diff[i] = diff_temp[i];
			}
		}
		temp++;
	}
	
	if(diff[0] == 1)
	{
		mod2div(diff_temp, diff, diff_len, crc);
	}
	else
	{
		mod2div(diff_temp, diff, diff_len, zeroes);
	}
	
	for(i=0; i<diff_len-1; i++)
	{
		if(diff_temp[i]==1)
		{
			return 1;
		}
		//printf("%d ", diff_temp[i]);
	}
	//printf("\n");
	
	return 0;
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
	
	if(connect<=0)
	{
		printf("Connection denied\n");
		return 0;
	}
	else
	{
		printf("Connection accepted\n");
		
		int org, len_org_data, data[256], crc[256], remainder[256], i, len_data, len_crc;
		char buff[256], crc_generator_s[256], data_s[256];
		
		recv(connect, buff, sizeof(buff), 0);
		len_org_data = atoi(buff);
		printf("Length of data sender want to send is: %d\n", len_org_data);
		
		recv(connect, crc_generator_s, sizeof(crc_generator_s), 0);
		printf("CRC generator is: %s\n", crc_generator_s);
		
		recv(connect, data_s, sizeof(data_s), 0);
		printf("Data received: %s\n", data_s);
		
		len_crc=0;
		for(i=0; i<strlen(crc_generator_s); i++)
		{
			crc[len_crc] = crc_generator_s[i]-48;
			len_crc++;
		}
		
		len_data=0;
		for(i=0; i<strlen(data_s); i++)
		{
			data[len_data] = data_s[i]-48;
			len_data++;
		}
		
		/*for(i=0; i<len_crc; i++)
		{
			printf("%d", crc[i]);
		}
		printf("\n");
		
		for(i=0; i<len_data; i++)
		{
			printf("%d", data[i]);
		}
		printf("\n");*/
		
		int error = crc_cal(remainder, crc, data, len_crc, len_data);
		
		if(error==1)
		{
			printf("There is error in received data\n");
			return 0;
		}
		else
		{
			printf("No error detected\n");
			printf("Data after decoding is: ");
			for(i=0; i<len_org_data; i++)
			{
				printf("%d", data[i]);
			}
			printf("\n");
		}
		
		close(recv_sock);
		
	}
}
