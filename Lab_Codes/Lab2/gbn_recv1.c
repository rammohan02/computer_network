#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
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
	int rec_sock;
	char msg[1024], tmsg[1024], buffer[1024];
	
	strcpy(tmsg, "Time out");
	
	int window_size = 4, w1;
	
	timer1 t;
	unsigned long seconds = 5, acktimer=5, temp;
	
	int count = 0, i, j, ack=0;
	
	struct sockaddr_in receiver, sender;
	Frame frame_recv, frame_send;
	
	rec_sock = socket(AF_INET, SOCK_DGRAM, 0);
	memset(&receiver, 0, sizeof(receiver));
	
	receiver.sin_family = AF_INET;
	receiver.sin_port = htons(9001);
	receiver.sin_addr.s_addr = INADDR_ANY;
	
	socklen_t addr;
	bind(rec_sock, (struct sockaddr *) &receiver, sizeof(receiver));
	addr = sizeof(sender);
	
	while(count!=10)
	{
		j=0;
		for(i=0; i<window_size; i++)
		{
			
			if(count<10)
			{
				int f_recv_size = recvfrom(rec_sock, &frame_recv, sizeof(Frame), 0, (struct sockaddr*) &sender, &addr);
				j++;
				count++;
				printf("Frame Received: %s\n", frame_recv.packet.msg);
			}
			if(strcmp(frame_recv.packet.msg, tmsg)==0)
			{
				break;
			}
		}
		i=0;
		//while(i<window_size)
		if(j>0)
		{
			
			//recvfrom(rec_sock, &frame_recv, sizeof(Frame), 0, (struct sockaddr*) &sender, &addr);
			
			/*if(strcmp(frame_recv.packet.msg, tmsg)==0)
			{
				break;
			}*/
			
			temp = rand()%50;
			if(temp<acktimer)
			{
				strcpy(frame_send.packet.msg, tmsg);
				frame_send.seq_no = 0;
				frame_send.frame_kind = 0;
				frame_send.ack = count;
				
				sendto(rec_sock, &frame_send, sizeof(Frame), 0, (struct sockaddr *) &sender, addr);
				//break;
			}
			else
			{
				if(count<=10 && ack+j==count && frame_recv.seq_no==count-1)
				{
					//printf("Frame Received: %s\n", frame_recv.packet.msg);
					//sprintf(buffer, "%d", frames[count]);
					printf("Acknowledgement sent for frame %d\n", count);
					strcpy(frame_send.packet.msg, frame_recv.packet.msg);
					frame_send.seq_no = 0;
					frame_send.frame_kind = 0;
					frame_send.ack = frame_recv.seq_no + 1;
					
					sendto(rec_sock, &frame_send, sizeof(Frame), 0, (struct sockaddr *) &sender, addr);
					ack = count;
				}
				else
				{
					//break;
					
					printf("Acknowledgement sent for frame1 %d\n", ack);
					strcpy(frame_send.packet.msg, frame_recv.packet.msg);
					frame_send.seq_no = 0;
					frame_send.frame_kind = 0;
					frame_send.ack = ack + 1;
					
					sendto(rec_sock, &frame_send, sizeof(Frame), 0, (struct sockaddr *) &sender, addr);
					count = count-j;
					ack = count;
					//count=10;
				}
				//count++;
			}
			
			/*if(count>10)
			{
				break;
			}*/
			i++;
		}
	}
	
	close(rec_sock);
	return 0;
}
