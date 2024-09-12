#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>				//For size_t variable
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<time.h>

typedef struct timer{
	time_t begTime;
}timer1;

typedef struct packet{
	char msg[1024];
}Packet;

typedef struct frame{
	int frame_kind;			//0-Ack, 1-pack
	int seq_no;
	int ack;
	Packet packet;
}Frame;

unsigned long elapsed_time(time_t tim)
{
	return (time(NULL)-tim);
}


int main()
{
	int frames[] = {1,2,3,4,5,6,7,8,9,10};
	int count=0;
	
	int window_size = 4, w1;
	
	timer1 t;
	
	unsigned long seconds = 10;
	
	int send_sock, i, j;
	char msg[1024], tmsg[1024];
	char buffer[1024];
	
	strcpy(tmsg, "Time out");
		
	struct sockaddr_in sender, receiver;
	
	int frame_id = 0;
	Frame frame_send, frame_recv;
	int ack_recv = 1;
	
	
	send_sock = socket(AF_INET, SOCK_DGRAM, 0);
	memset(&sender, 0, sizeof(sender));
	
	sender.sin_family = AF_INET;
	sender.sin_port = htons(9001);
	sender.sin_addr.s_addr = INADDR_ANY; 
	
	int addr_size = sizeof(receiver);
	
	while(count!=10)
	{
		j=0;
		for(i=0; i<window_size; i++)
		{
			sprintf(buffer, "%d", frames[count]);
			strcpy(frame_send.packet.msg, buffer);
			frame_send.seq_no = count;
			frame_send.frame_kind = 1;
			frame_send.ack = 0;
			
			sendto(send_sock, &frame_send, sizeof(Frame), 0, (struct sockaddr *) &sender, sizeof(sender));
			if(count<10)
			{
				printf("Sending Frame: %d\n", frames[count]);
				count++;
				j++;
			}
		}
		i=0;
		w1=window_size;
		
		/*while(i<window_size)
		{
			t.begTime = time(NULL);
			if(rand()%2)
			{
				int to = 24600 + rand()%(64000 - 24600)  + 1;
			    for(int i=0;i<64000;i++)
			    	for(int j=0;j<to;j++) {}
			}
			
			
			int f_rec_size = recvfrom(send_sock, &frame_recv, sizeof(frame_recv), 0, (struct sockaddr *) &receiver, &addr_size);
			
			if(strcmp(frame_recv.packet.msg, tmsg)==0)
			{
				int last = count - w1;
				if(last<10)
				{
					printf("Delayed Ack. Resend frames from %d onwards", last);
				}
				break;
			}
			else
			{
				if(frame_recv.ack<=10)
				{
					printf("Received Ack:%d\n", frame_recv.ack);
					w1--;
				}
				else
				{
					break;
				}
			}
			if(frame_recv.ack>10)
			{
				break;
			}
			i++;
		}
		
		if(w1==0 && count>=9)
		{
			strcpy(frame_send.packet.msg, tmsg);
			frame_send.seq_no = 0;
			frame_send.frame_kind = 1;
			frame_send.ack = 0;
			sendto(send_sock, &frame_send, sizeof(Frame), 0, (struct sockaddr *) &sender, sizeof(sender));
			break;
		}
		else if(w1!=0 && count<10)
		{
			count = count - w1;
			w1=window_size;
		}
		//printf("\n%d\n", count);*/
		
		if(j>0)
		{
			t.begTime = time(NULL);
			if(rand()%2)
			{
				int to = 24600 + rand()%(64000 - 24600)  + 1;
			    for(int i=0;i<64000;i++)
			    	for(int j=0;j<to;j++) {}
			}
			
			
			if(elapsed_time(t.begTime) > seconds)
			{
				printf("Delayed Ack\n");
				count = count - j;
			}
			else
			{
				recvfrom(send_sock, &frame_recv, sizeof(frame_recv), 0, (struct sockaddr *) &receiver, &addr_size);
			
				if(strcmp(frame_recv.packet.msg, tmsg)==0)
				{
					int last = count - w1;
					if(last<=10)
					{
						printf("Delayed Ack. Resend frames from %d onwards", last);
					}
					//break;
				}
				else
				{
					if(frame_recv.ack<=10)
					{
						printf("Received Ack:%d\n", frame_recv.ack);
						//w1--;
					}
				}
			}
			
		}
	}
	
	
	close(send_sock);
	
	return 0;
}


























