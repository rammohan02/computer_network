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
	int frames[] = {1,2,3,4,5,6,7,8,9,10};
	int nack_recv[100], size_nack_recv=0; 
	int count=0, current_frame=0;
	
	int window_size = 4;
	
	timer1 t;
	
	unsigned long seconds = 10;
	
	int send_sock, i, j, k, l, m, n, len=0;
	char msg[1024], tmsg[1024];
	char buffer[1024];
	
	int frame_id = 0;
	Frame frame_send, frame_recv;
	int ack_recv = 1;
	
	struct sockaddr_in sender, receiver;
	
	send_sock = socket(AF_INET, SOCK_DGRAM, 0);
	memset(&sender, 0, sizeof(sender));
	
	sender.sin_family = AF_INET;
	sender.sin_port = htons(9001);
	sender.sin_addr.s_addr = INADDR_ANY;
	
	int addr_size = sizeof(receiver);
	
	while(count!=10)
	{
		current_frame=0;
		j=0;
		for(m=0; m<size_nack_recv; m++)
		{
			
			sprintf(buffer, "%d", nack_recv[m]);
			strcpy(frame_send.packet.msg, buffer);
			frame_send.seq_no = nack_recv[m];
			frame_send.frame_kind = 1;
			frame_send.ack = 0;
			
			sendto(send_sock, &frame_send, sizeof(Frame), 0, (struct sockaddr *) &sender, sizeof(sender));
		
			printf("Sending Packet1: %d\n", nack_recv[m]);
			count++;
			j++;
		}
		for(i=0; i<window_size; i++)
		{
			if(count<10)
			{
				sprintf(buffer, "%d", frames[len]);
				strcpy(frame_send.packet.msg, buffer);
				frame_send.seq_no = count;
				frame_send.frame_kind = 1;
				frame_send.ack = 0;
			
				sendto(send_sock, &frame_send, sizeof(Frame), 0, (struct sockaddr *) &sender, sizeof(sender));
			
				printf("Sending Packet: %d\n", frames[count]);
				count++;
				len++;
				j++;
			}
		}
		
		i=0;
//		t.begTime = time(NULL);
		
		k=0;
		l=0;
		m=0;
		n=count-j;
		while(m<window_size && n<10)
		{
			int f_recv_size = recvfrom(send_sock, &frame_recv, sizeof(frame_recv), 0, (struct sockaddr *) &receiver, &addr_size);
			
			if(frame_recv.ack == -1)
			{
				printf("Negative acknowledgement received for packet: %s\n",frame_recv.packet.msg);
				nack_recv[l]=atoi(frame_recv.packet.msg);
				k=1;
				l++;
				count--;
			}
			m++;
			n++;
		}
		
		if(k==0)
		{
			printf("Positive acknowledgement received for all packets within the window\n");
		}
	}
	
	close(send_sock);
	return 0;
}
