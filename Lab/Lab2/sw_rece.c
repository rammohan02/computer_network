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
	char msg[1024];
	
	timer1 t;
	unsigned long seconds = 5;
	
	
	struct sockaddr_in receiver, sender;
	
	int count = 0;
	Frame frame_recv, frame_send;
	
	
	rec_sock = socket(AF_INET, SOCK_DGRAM, 0);
	memset(&receiver, 0, sizeof(receiver));
	
	receiver.sin_family = AF_INET;
	receiver.sin_port = htons(9001);
	receiver.sin_addr.s_addr = INADDR_ANY;
	
	socklen_t addr;
	bind(rec_sock, (struct sockaddr *) &receiver, sizeof(receiver));
	
	addr = sizeof(sender);
	
	do
	{
		int f_recv_size = recvfrom(rec_sock, &frame_recv, sizeof(Frame), 0, (struct sockaddr*) &sender, &addr);
		
		if(f_recv_size>0 && frame_recv.frame_kind==1 && frame_recv.seq_no==count){
			printf("Frame Received: %s\n", frame_recv.packet.msg);
			
			frame_send.seq_no = 0;
			frame_send.frame_kind = 0;
			frame_send.ack = frame_recv.seq_no + 1;
			
			sendto(rec_sock, &frame_send, sizeof(Frame), 0, (struct sockaddr *) &sender, addr);
			printf("Acknowledgement: %d\n", frame_recv.seq_no+1);
			count++;
		}
		else{
			printf("No Frame received\n");
			
			frame_send.seq_no = 0;
			frame_send.frame_kind = 0;
			frame_send.ack = count;
			sendto(rec_sock, &frame_send, sizeof(Frame), 0, (struct sockaddr *) &sender, addr);
		}
	}while(count!=10);
	
	
	
	/*recvfrom(rec_sock, msg, sizeof(msg), 0, (struct sockaddr*) &sender, &addr);
	
	printf("Receiver: %s\n", msg);*/
	
	close(rec_sock);
	return 0;
}

