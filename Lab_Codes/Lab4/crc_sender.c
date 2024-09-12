#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>


void mod2div(char ret[], char str1[], int len, char str2[])
{
	//ret = "";
	int i, j=0;
	
	for(i=1; i<len; i++)
	{
		if(str1[i] == str2[i])
		{
			ret[j] = '0';
		}
		else
		{
			ret[j]= '1';
		}
		j++;
	}
	//printf("%d: %s\n", len, ret);
}

void crc_cal(char remainder[], char data[], char crc[])
{
	int temp, diff_len, data_len, i;
	
	data_len = strlen(data);
	temp = strlen(crc);
	diff_len=0;
	
	char diff[temp+1];
	
	for(int i=0; i<temp; i++)
	{
		diff[diff_len] = data[i];
		diff_len++;
	}
	
	char diff_temp[temp+1], zeroes[temp+1];
	
	for(i=0; i<temp; i++)
	{
		zeroes[i] = '0';
	}
	
	while(temp<data_len)
	{
		if(diff[0]=='1')
		{
			mod2div(diff_temp, diff, diff_len, crc);
			diff_temp[diff_len-1]=data[temp];
			for(i=0; i<diff_len; i++)
			{
				diff[i] = diff_temp[i];
			}
		}
		else
		{
			mod2div(diff_temp, diff, diff_len, zeroes);
			diff_temp[diff_len-1]=data[temp];
			for(i=0; i<diff_len; i++)
			{
				diff[i] = diff_temp[i];
			}
		}
		temp++;
	}
	
	if(diff[0]=='1')
	{
		mod2div(diff_temp, diff, diff_len, crc);
	}
	else
	{
		mod2div(diff_temp, diff, diff_len, zeroes);
	}
	
	//printf("diff_temp: %s\n", diff_temp);
	
	/*for(i=0; i<diff_len; i++)
	{
		printf("%c\n", diff[i]);
	}
	printf("%s\n", diff);*/
	for(i=0; i<diff_len-1; i++)
	{
		remainder[i] = diff_temp[i];
	}
	
}

int main()
{
	int send_sock;
	
	struct sockaddr_in sender, receiver;
	
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
		
		int org_data, len_org_data, crc_generator, len_crc;
		
		char buff[256], org_data_s[256], crc_generator_s[256], modified_data[256], remainder[256], data_sent[256];
		
		//printf("Enter length of data: ");
		//scanf("%d", &len_org_data);
		
		//sprintf(buff, "%d", len_org_data);
		//send(send_sock, buff, sizeof(buff), 0);
		
		printf("Enter CRC generator in form of binary numbers: ");
		scanf("%d", &crc_generator);
		
		sprintf(crc_generator_s, "%d", crc_generator);
		len_crc = strlen(crc_generator_s);
		printf("%d\n", len_crc);
		//printf("%s\n", crc_generator_s);
		
		printf("Enter data: ");
		scanf("%d", &org_data);
		
		sprintf(org_data_s, "%d", org_data);
		//printf("%s\n", org_data_s);
		len_org_data = strlen(org_data_s);
		
		sprintf(buff, "%d", len_org_data);
		printf("%d\n", len_org_data);
		send(send_sock, buff, sizeof(buff), 0);
		
		for(int i=0; i<len_org_data+len_crc-1; i++)
		{
			if(i<len_org_data)
			{
				modified_data[i] = org_data_s[i];
			}
			else
			{
				modified_data[i] = '0';
			}
		}
		
		printf("Modified data(zeroes): %s\n", modified_data);
		
		crc_cal(remainder, modified_data, crc_generator_s);
		
		printf("Remainder: %s\n", remainder);
		
		int j=0;
		for(int i=0; i<len_org_data+len_crc-1; i++)
		{
			if(i<len_org_data)
			{
				data_sent[i] = org_data_s[i];
			}
			else
			{
				data_sent[i] = remainder[j];
				j++;
			}
		}
		
		printf("Data sent: %s\n", data_sent);
		
		close(send_sock);
		
		
	}
}
