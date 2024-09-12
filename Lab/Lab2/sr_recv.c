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

int main()
{
	int frames_recv[100], frames_recv_size=0, f_wind[100], f_wind_size=0;
	int nack_recv[100], size_nack_recv=0;
	int count=0;
	
	int window_size = 4;
	
	timer1 t;
	
	unsigned long seconds = 10;
	
	int rec_sock, i, j, k, l, m, n;
	char msg[1024], tmsg[1024];
	char buffer[1024];
	
	int frame_id = 0;
	Frame frame_send, frame_recv;
	int ack_recv = 1;
	
	struct sockaddr_in sender, receiver;
	
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
		m=0;
		f_wind_size=0;
		size_nack_recv = 0;
		for(i=0; i<window_size; i++)
		{
			if(count<10)
			{
				int f_recv_size = recvfrom(rec_sock, &frame_recv, sizeof(Frame), 0, (struct sockaddr*) &sender, &addr);
				
				if(f_recv_size>0)
				{
					j++;
					count++;
					frames_recv[frames_recv_size++] = count;
					f_wind[f_wind_size++] = count;
					printf("Packet Received: %s\n", frame_recv.packet.msg);
				}
				else
				{
					printf("Packet %d is not received\n", count+1);
					nack_recv[m] = count+1;
					m++;
					size_nack_recv++;
					//count++;
				}
			}
		}
		
		/*for(j=0; j<f_wind_size; j++)
		{
			printf("%d\t", f_wind[j]);
		}
		printf("\n");
		for(j=0; j<size_nack_recv; j++)
		{
			printf("%d\t", nack_recv[j]);
		}
		printf("\n");*/
		
		j=0;
		m=0;
		n=l;
		for(j=0; j<f_wind_size; j++)
		{
			printf("Acknowledgement sent for frame %d\n", f_wind[j]);
			sprintf(buffer, "%d", f_wind[j]);
			strcpy(frame_send.packet.msg, buffer);
			frame_send.seq_no = 0;
			frame_send.frame_kind = 0;
			frame_send.ack = f_wind[j];
			
			sendto(rec_sock, &frame_send, sizeof(Frame), 0, (struct sockaddr *) &sender, addr);
		}
		
		for(m=0; m<size_nack_recv; m++)
		{
			printf("Neg Acknowledgement sent for frame1 %d\n", nack_recv[m]);
			sprintf(buffer, "%d", nack_recv[m]);
			strcpy(frame_send.packet.msg, buffer);
			frame_send.seq_no = 0;
			frame_send.frame_kind = 0;
			frame_send.ack = -1;
			
			sendto(rec_sock, &frame_send, sizeof(Frame), 0, (struct sockaddr *) &sender, addr);
		}
	}
	
	close(rec_sock);
	return 0;
}


















