#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>


void mod2div(int ret[], int str1[], int len, int str2[])							//function to do modulo 2 division
{
	int i, j=0;
	
	for(i=1; i<len; i++)
	{
		if(str1[i] == str2[i])
		{
			ret[j] = 0;
		}
		else
		{
			ret[j]= 1;
		}
		j++;
	}
}

void crc_cal(int remainder[], int data[], int data_len, int crc[], int len_crc)		//function to find parity pits
{
	int temp, diff_len, i;
	
	temp = len_crc;
	diff_len=0;
	
	int diff[temp+1];
	
	for(i=0; i<temp; i++)														//storing first bits for mod 2 division
	{
		diff[diff_len] = data[i];
		diff_len++;
	}
	
	int diff_temp[temp+1], zeroes[temp+1];
	
	for(i=0; i<temp; i++)														//Zeroes array is usedwhen remainder is less than divisior
	{
		zeroes[i] = 0;
	}
	
	//running till there is still one more iteration is left because at last iteration we will get remainder as less than 1 bit of length of crc_generator. But in this while loop we will store 		remainder with same bits as length of crc_generator so to avoid garbage value we will stop before one iteration
	while(temp<data_len)														
	{
		if(diff[0]==1)																
		{
			mod2div(diff_temp, diff, diff_len, crc);							//remainder after current division we will be storing in diff_temp
			diff_temp[diff_len-1]=data[temp];									//droping down the next bit for division
			for(i=0; i<diff_len; i++)											//combining the data into diff variable for next iteration
			{
				diff[i] = diff_temp[i];
			}
		}
		else																	//if remiander is less than divisior then we must pull a bit down so we will do division with all 0's
		{
			mod2div(diff_temp, diff, diff_len, zeroes);
			diff_temp[diff_len-1]=data[temp];
			for(i=0; i<diff_len; i++)
			{
				diff[i] = diff_temp[i];
			}
		}
		temp++;
		/*for(i=0; i<diff_len; i++)
		{
			printf("%d ", diff[i]);
		}
		printf("\n");*/
	}
	
	//Doing the last iteration and we will get the final remainder in this iteration
	if(diff[0]==1)
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
		printf("%d\n", diff[i]);
	}*/
	//printf("%s\n", diff);
	
	for(i=0; i<diff_len-1; i++)													//Storing remainder from diff_temp
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
		
		int org_data, len_org_data, crc_generator, len_crc, len_data_modification;
		
		char buff[256], org_data_s[256], crc_generator_s[256], data_sent[256];
		int modified_data[256], crc_int[256], remainder[256];
		
		
		printf("Enter CRC generator in form of binary numbers: ");
		scanf("%d", &crc_generator);
		
		sprintf(crc_generator_s, "%d", crc_generator);
		len_crc = strlen(crc_generator_s);
		
		printf("Enter data: ");
		scanf("%d", &org_data);
		
		sprintf(org_data_s, "%d", org_data);
		len_org_data = strlen(org_data_s);
		
		sprintf(buff, "%d", len_org_data);
		send(send_sock, buff, sizeof(buff), 0);
		
		send(send_sock, crc_generator_s, sizeof(crc_generator_s), 0);
		
		len_data_modification = len_org_data+len_crc-1;
		
		for(int i=0; i<len_data_modification; i++)								//appending zeroes to data storing it in integer arrray for easy computation
		{
			if(i<len_org_data)
			{
				modified_data[i] = org_data_s[i]-48;
			}
			else
			{
				modified_data[i] = 0;
			}
		}
		
		for(int i=0; i<len_crc; i++)											//Storing crc_generator in integer arrray for easy computation
		{
			crc_int[i] = crc_generator_s[i]-48;
		}

		
		crc_cal(remainder, modified_data, len_data_modification,  crc_int, len_crc);		//calling function to get remainder
		
		
		int j=0;
		for(int i=0; i<len_data_modification; i++)
		{
			if(i<len_org_data)
			{
				data_sent[i] = org_data_s[i];
			}
			else
			{
				data_sent[i] = remainder[j]+48;
				j++;
			}
		}
		
		
		printf("Data sent:%s\n", data_sent);
		send(send_sock, data_sent, sizeof(data_sent), 0);
		
		close(send_sock);
		
		
	}
}
