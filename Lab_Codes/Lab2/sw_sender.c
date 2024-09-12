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
	int frames[] = {1,2,3,4,5,6,7,8,9,10};
	int count=0;
	
	timer1 t;
	//t.begTime = time(NULL);
	unsigned long seconds = 5;
	
	//printf("%ld\n", t.begTime-seconds);
	//printf("%ld\n", t.begTime);
	
	int send_sock;
	char msg[1024];
	
	struct sockaddr_in sender, receiver;
	
	int frame_id = 0;
	Frame frame_send, frame_recv;
	int ack_recv = 1;
	
	
	send_sock = socket(AF_INET, SOCK_DGRAM, 0);
	memset(&sender, 0, sizeof(sender));
	
	sender.sin_family = AF_INET;
	sender.sin_port = htons(9001);
	sender.sin_addr.s_addr = INADDR_ANY;
	
	
	do
	{
//		bool timeout= false;
		printf("Sending Frame: %d\n", frames[count]);
		char buffer[1024];
		sprintf(buffer, "%d", frames[count]);
		strcpy(frame_send.packet.msg, buffer);
		frame_send.seq_no = count;
		frame_send.frame_kind = 1;
		frame_send.ack = 0;

		sendto(send_sock, &frame_send, sizeof(Frame), 0, (struct sockaddr *) &sender, sizeof(sender));
		count++;
		
		t.begTime = time(NULL);
		if(rand()%2)
		{
			int to = 24600 + rand()%(64000 - 24600)  + 1;
	        for(int i=0;i<64000;i++)
    	    	for(int j=0;j<to;j++) {}
		}
		
		int addr_size = sizeof(receiver);
		
		int f_rec_size = recvfrom(send_sock, &frame_recv, sizeof(frame_recv), 0, (struct sockaddr *) &receiver, &addr_size);
		
		if(elapsed_time(t.begTime) > seconds)
		{
			printf("Delayed Ack\n");
			count--;
		}else if(f_rec_size>0 && frame_recv.seq_no==0 && frame_recv.ack==count){
			printf("Received Ack:%d\n", frame_recv.ack);
		}else{
			printf("-ve Ack\n");
			count--;
		}
	}while(count!=10);

	
	/*socklen_t ad;
	
	ad = sizeof(receiver);
	strcpy(msg, "Hi");
	sendto(send_sock, msg, sizeof(msg), 0, (struct sockaddr*) &sender, sizeof(sender));
	
	printf("message sent: %s\n", msg);*/
	close(send_sock);
	
	return 0;
}
