/**************************************************************
rdt-part3.h
Student name: Xu Bing
Student No. :3035027710
Date and version:2015/04/14
Development platform:Mac OS X
Development language:C
Compilation: 
	Can be compiled with gcc 
*****************************************************************/

#ifndef RDT3_H
#define RDT3_H

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>

#define PAYLOAD 1000		//size of data payload of the RDT layer
#define TIMEOUT 50000		//50 milliseconds
#define TWAIT 10*TIMEOUT	//Each peer keeps an eye on the receiving  
							//end for TWAIT time units before closing
							//For retransmission of missing last ACK
#define W 5					//For Extended S&W - define pipeline window size


//----- Type defines ----------------------------------------------------------
typedef unsigned char		u8b_t;    	// a char
typedef unsigned short		u16b_t;  	// 16-bit word
typedef unsigned int		u32b_t;		// 32-bit word 

extern float LOSS_RATE, ERR_RATE;

typedef struct _packet
	{
		u16b_t packet_type;//0 for data, 1 for ACK
		u16b_t sequenceNum;
		u16b_t checksum;
		char payload[PAYLOAD];
	} Packet;

int nextseqnum = 0;//0~(W-1)
int last_acknum = 0;//to record the ACKed as a sender
int expectedseqnum = 0;//for the process as a receiver
/* this function is for simulating packet loss or corruption in an unreliable channel */
/***
Assume we have registered the target peer address with the UDP socket by the connect()
function, udt_send() uses send() function (instead of sendto() function) to send 
a UDP datagram.
***/
int udt_send(int fd, void * pkt, int pktLen, unsigned int flags) {
	double randomNum = 0.0;

	/* simulate packet loss */
	//randomly generate a number between 0 and 1
	randomNum = (double)rand() / RAND_MAX;
	if (randomNum < LOSS_RATE){
		//simulate packet loss of unreliable send
		printf("WARNING: udt_send: Packet lost in unreliable layer!!!!!!\n");
		return pktLen;
	}

	/* simulate packet corruption */
	//randomly generate a number between 0 and 1
	randomNum = (double)rand() / RAND_MAX;
	if (randomNum < ERR_RATE){
		//clone the packet
		u8b_t errmsg[pktLen];
		memcpy(errmsg, pkt, pktLen);
		//change a char of the packet
		int position = rand() % pktLen;
		if (errmsg[position] > 1) errmsg[position] -= 2;
		else errmsg[position] = 254;
		printf("WARNING: udt_send: Packet corrupted in unreliable layer!!!!!!\n");
		return send(fd, errmsg, pktLen, 0);
	} else 	// transmit original packet
		return send(fd, pkt, pktLen, 0);
}

/* this function is for calculating the 16-bit checksum of a message */
/***
Source: UNIX Network Programming, Vol 1 (by W.R. Stevens et. al)
***/
u16b_t checksum(u8b_t *msg, u16b_t bytecount)
{
	u32b_t sum = 0;
	u16b_t * addr = (u16b_t *)msg;
	u16b_t word = 0;
	
	// add 16-bit by 16-bit
	while(bytecount > 1)
	{
		sum += *addr++;
		bytecount -= 2;
	}
	
	// Add left-over byte, if any
	if (bytecount > 0) {
		*(u8b_t *)(&word) = *(u8b_t *)addr;
		sum += word;
	}
	
	// Fold 32-bit sum to 16 bits
	while (sum>>16) 
		sum = (sum & 0xFFFF) + (sum >> 16);
	
	word = ~sum;
	
	return word;
}

//----- Type defines ----------------------------------------------------------

// define your data structures and global variables in here




int rdt_socket();
int rdt_bind(int fd, u16b_t port);
int rdt_target(int fd, char * peer_name, u16b_t peer_port);
int rdt_send(int fd, char * msg, int length);
int rdt_recv(int fd, char * msg, int length);
int rdt_close(int fd);

/* Application process calls this function to create the RDT socket.
   return	-> the socket descriptor on success, -1 on error 
*/
int rdt_socket() {
//same as part 1
	int sockfd;
   if((sockfd=socket(AF_INET,SOCK_DGRAM,0))==-1){
     // perror("socket");
      return -1;
   }
   return sockfd;
}

/* Application process calls this function to specify the IP address
   and port number used by itself and assigns them to the RDT socket.
   return	-> 0 on success, -1 on error
*/
int rdt_bind(int fd, u16b_t port){
//same as part 1
	 struct sockaddr_in my_addr;  
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);     // short, network byte order
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY); // automatically fill with my IP address
    memset(&(my_addr.sin_zero), '\0', 8); // zero the rest of the struct
   
   if((bind(fd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr)))==-1){
      return -1;
   }
   return 0;
}

/* Application process calls this function to specify the IP address
   and port number used by remote process and associates them to the 
   RDT socket.
   return	-> 0 on success, -1 on error
*/
int rdt_target(int fd, char * peer_name, u16b_t peer_port){
//same as part 1
	struct hostent *he;
   // get the host info
    if ((he=gethostbyname(peer_name))== NULL) {  
        fprintf(stderr,"gethostbyname fail\n");
        return -1;
    }

    struct sockaddr_in peer_addr;
    peer_addr.sin_family = AF_INET;    // host byte order 
    peer_addr.sin_port = htons(peer_port);  // short, network byte order 
    peer_addr.sin_addr = *((struct in_addr *)he->h_addr); // already in network byte order
    memset(&(peer_addr.sin_zero), '\0', 8);  // zero the rest of the struct 
    connect(fd,(struct sockaddr *)& peer_addr,sizeof(struct sockaddr_in));
    return 0;
}

/* Application process calls this function to transmit a message to
   target (rdt_target) remote process through RDT socket; this call will
   not return until the whole message has been successfully transmitted
   or when encountered errors.
   msg		-> pointer to the application's send buffer
   length	-> length of application message
   return	-> size of data sent on success, -1 on error
*/
  
int rdt_send(int fd, char * msg, int length){
//implement the Extended Stop-and-Wait ARQ logic
	Packet packet_recv; 
	int numSent = 0;
	int N = 0;
	int q = length/PAYLOAD;
	int remain = length%PAYLOAD;//if not 0, need a new packet
	//printf("Here the q = %d,remain = %d\n",q,remain );
	//printf("Here the nextseqnum = %d\n",nextseqnum );
	//int last_ACKed = 0;
	if(remain==0){
		N=q;
	}else{
		N=q+1;
	}
	//printf("Here the N= %d\n",N );

	if(N>W){//assum that N<=W always true
		perror("rdt_send");
		printf("Too much data passed,payload overflow\n");
		return -1;
	} 
	//make the packets 
	Packet packets_send[N];
	int S = nextseqnum;//to record the starting sequenceNum of this rdt_send()
	bool ACKed[N];

	for(int j=0;j<N;j++){
		ACKed[j]=false;
	}
	for(int i=0;i<q;i++){
		
		packets_send[i].packet_type = 0;
		packets_send[i].sequenceNum = nextseqnum;
		packets_send[i].checksum = 0;
		//memcpy(packets_send[i].payload,msg+PAYLOAD*nextseqnum,PAYLOAD);
		memcpy(packets_send[i].payload,msg+PAYLOAD*i,PAYLOAD);

		u8b_t tmp[sizeof(packets_send[i])];
		memcpy(tmp,&packets_send[i],sizeof(packets_send[i]));
		u16b_t checksum_tmp = checksum(tmp,sizeof(tmp));
		packets_send[i].checksum = checksum_tmp;
		
		//nextseqnum = (nextseqnum+1)%W;
		nextseqnum++;
	}
	if(remain!=0){
		packets_send[N-1].packet_type = 0;
		packets_send[N-1].sequenceNum = nextseqnum;
		packets_send[N-1].checksum = 0;
		memcpy(packets_send[N-1].payload,msg+PAYLOAD*q,remain);
		u8b_t tmp[6+remain];
		memcpy(tmp,&packets_send[N-1],6+remain);
		u16b_t checksum_tmp = checksum(tmp,sizeof(tmp));
		packets_send[N-1].checksum = checksum_tmp;

		//nextseqnum = (nextseqnum+1)%W;
		nextseqnum++;
	}
	//send the packets
	for(int i=0;i<q;i++){
		if((numSent += udt_send(fd,&packets_send[i],6+PAYLOAD,0))==-1){
		perror("udt_send");
		return -1;
		}
		printf("rdt_send:Sent one message of size %lu with sequence num = %d\n",sizeof(packets_send[i]),packets_send[i].sequenceNum);//change later to make it faster
	}
	if(remain!=0){
		if((numSent += udt_send(fd,&packets_send[N-1],6+remain,0))==-1){
		perror("udt_send");
		return -1;
		}
		printf("rdt_send:Sent one message of size %lu with sequence num = %d\n",sizeof(packets_send[N-1]),packets_send[N-1].sequenceNum);//change later to make it faster
	
	}

	//start timer,wait for ACK or timeout until received all ACKs;
	int status;
	fd_set read_fds;
	FD_ZERO(&read_fds);
	FD_SET(fd,&read_fds);
	struct  timeval timer;
	timer.tv_sec = 0;
	timer.tv_usec = TIMEOUT;

	for(;;){
		FD_ZERO(&read_fds);
		FD_SET(fd,&read_fds);

		if((status=select(fd+1,&read_fds,NULL,NULL,&timer))==-1){
			perror("select");
			return -1;
		} else if (status == 0) {  
		// timeout, retransmit all unACKed packets and restart the timer
			printf("rdt_send:Timeout!! Retransmit all unACKed packets again\n");
			
			for(int i=0;i<q;i++){
				if(ACKed[i]==false){
				if((udt_send(fd,&packets_send[i],6+PAYLOAD,0))==-1){
				perror("udt_send");
				return -1;
				}
				printf("rdt_send:Retransmit one message of size %lu with sequence num = %d\n",sizeof(packets_send[i]),packets_send[i].sequenceNum);//change later to make it faster
				}
			}
			if(remain!=0&&ACKed[N-1]==false){
				if((udt_send(fd,&packets_send[N-1],6+remain,0))==-1){
				perror("udt_send");
				return -1;
			}
			printf("rdt_send:Retransmit one message of size %lu with sequence num = %d\n",sizeof(packets_send[N-1]),packets_send[N-1].sequenceNum);//change later to make it faster
	
			}
			
			continue;
		}//end of timeout action
		//packet arrives

		int numReceived =0;
		if((numReceived=recv(fd,&packet_recv,sizeof(packet_recv),0))==-1){
			perror("recv");
			return -1;
		}
		//check if the received packet is corrupted one
		u8b_t packet_check[numReceived];
		memcpy(packet_check, &packet_recv, numReceived);
		u16b_t check_tmp = checksum(packet_check,sizeof(packet_check));

		if(check_tmp!=0){  
			printf("rdt_send:Received a corrupted packet : Type = %d,Length = %d\n",packet_recv.packet_type,numReceived);
			printf("rdt_send:Drop the packet\n" );

            memset(&packet_recv,'\0',sizeof(packet_recv));
			continue;
		}

		//not corrupted
		if(packet_recv.packet_type==0){//received unexpectd DATA packet,maybe it used to be a receiver
			printf("rdt_send:Received an unexpected DATA packet\n" );
			//to test is it new or a retransmitted one
			//the expectedseqnum actually will stay in the state when the sender is a receiver.

			if(packet_recv.sequenceNum==expectedseqnum){//it is a new one,just drop it.
				memset(&packet_recv,'\0',sizeof(packet_recv));
			}else{//it is a retransmitted one,need to send the old ACK
				Packet ACK_makeup;
				ACK_makeup.packet_type=1;
				if(expectedseqnum==0){
					ACK_makeup.sequenceNum = W-1;
				}else{
					ACK_makeup.sequenceNum = expectedseqnum-1;
				}
				
				ACK_makeup.checksum = 0;
				int len_ACK=6;
				u8b_t tmp1[len_ACK];
				memcpy(tmp1, &ACK_makeup, len_ACK);
				ACK_makeup.checksum = checksum(tmp1,sizeof(tmp1));
				printf("rdt_send: Retransmit the old ACK...............................\n");
				if((udt_send(fd,&ACK_makeup,len_ACK,0))==-1){
					perror("udt_send");
					return -1;
				}
				memset(&packet_recv,'\0',sizeof(packet_recv));//then drop the packet
			}
			
			continue;

		}else{//got an ACK


			if((S+N-1)>=S){//didnt wrap around
				if(packet_recv.sequenceNum<S||packet_recv.sequenceNum>(S+N-1)){//not in window sequenceNum
					printf("rdt_send:Received an unexpected ACK with sequence number%d\n",packet_recv.sequenceNum);
					memset(&packet_recv,'\0',sizeof(packet_recv));
					continue;
				}else if(packet_recv.sequenceNum>=S&&packet_recv.sequenceNum<(S+N-1)){//received correct sequenceNum, cumulative acknowledgment approach
					printf("rdt_send:Received ACK : sequnceNum = %d--(%d-%d)\n",packet_recv.sequenceNum,S,S+N-1);
					//then up to it all aknowleded
					for(int i=0;i<packet_recv.sequenceNum-S+1;i++){
						ACKed[i] = true;
					}
					last_acknum = packet_recv.sequenceNum;
					continue;
				}else if(packet_recv.sequenceNum==(S+N-1)){
					printf("rdt_send:Received ACK : sequnceNum = %d--(%d-%d),then all packets acknowledged\n",packet_recv.sequenceNum,S,S+N-1);
					ACKed[N-1] = true;
					last_acknum = packet_recv.sequenceNum;
					return numSent-6*N;//minus N packet header bytes
				}
			}else{//did wraparound
				if(packet_recv.sequenceNum>(S+N-1)&&packet_recv.sequenceNum<S){
					printf("rdt_send:Received an unexpected ACK with sequence number%d\n",packet_recv.sequenceNum);
					memset(&packet_recv,'\0',sizeof(packet_recv));
					continue;
				}else if(packet_recv.sequenceNum<(S+N-1)&&packet_recv.sequenceNum>=S){//received correct sequenceNum, cumulative acknowledgment approach
					printf("rdt_send:Received ACK : sequnceNum = %d--(%d-%d)\n",packet_recv.sequenceNum,S,S+N-1);
					//then up to it all aknowleded
					for(int i=0;i<packet_recv.sequenceNum-S+1;i++){
						ACKed[i] = true;
					}
					last_acknum = packet_recv.sequenceNum;
					continue;
				}else if(packet_recv.sequenceNum==(S+N-1)){
					printf("rdt_send:Received ACK : sequnceNum = %d--(%d-%d),then all packets acknowledged\n",packet_recv.sequenceNum,S,S+N-1);
					ACKed[N-1] = true;
					last_acknum = packet_recv.sequenceNum;
					return numSent-6*N;//minus N packet header bytes
				}


			}//end of sequenceNum check 
		}//end of packet_type check
	}//end for loop
	
}//end of rdt_send()

/* Application process calls this function to wait for a message of any
   length from the remote process; the caller will be blocked waiting for
   the arrival of the message. 
   msg		-> pointer to the receiving buffer
   length	-> length of receiving buffer
   return	-> size of data received on success, -1 on error
*/
int rdt_recv(int fd, char * msg, int length){
//implement the Extended Stop-and-Wait ARQ logic
	int numReceived = 0;
	int len_ACK = 6;
	Packet packet_recv,packet_send;
	memset(packet_send.payload,'\0',PAYLOAD);
    memset(&packet_recv,'\0',sizeof(packet_recv));

    while(true){
    	if((numReceived=recv(fd,&packet_recv,sizeof(packet_recv),0))==-1){
			perror("recv");
			return -1;
	}
	//check if corrupted
	
	u8b_t packet_check[numReceived];
	memcpy(packet_check, &packet_recv, numReceived);
	u16b_t checksum_tmp = checksum(packet_check,sizeof(packet_check));

	if(checksum_tmp!=0){//corruption happened
		printf("rdt_recv:Received a corrupted packet : Type = %d,Length = %d\n",packet_recv.packet_type,numReceived);
		printf("rdt_recv:Drop the packet\n" );
		
		memset(&packet_recv,'\0',sizeof(packet_recv));
		
		continue; 
	}
	//no corruption happened
	if(packet_recv.packet_type==0){//DATA

		if(packet_recv.sequenceNum==expectedseqnum){
		//	printf("rdt_recv:Got an expected packet,now the expectedseqnum = %d the last_acknum = %d \n",expectedseqnum,last_acknum);
			printf("rdt_recv:Got an expected packet - SeqNo = %d\n",packet_recv.sequenceNum);
			
			memcpy(msg,packet_recv.payload,length);
			
			packet_send.packet_type = 1;//ACK
			packet_send.sequenceNum = expectedseqnum;
	
			packet_send.checksum = 0;
			
			u8b_t tmp2[len_ACK];
			memcpy(tmp2, &packet_send, len_ACK);
			packet_send.checksum = checksum(tmp2,sizeof(tmp2));


			//change the expectedseqnum
			//expectedseqnum = (expectedseqnum+1)%W;
			expectedseqnum ++;
		//send back ACK
			if((udt_send(fd,&packet_send,len_ACK,0))==-1){
				perror("udt_send");
				return -1;

			}
			
			return numReceived-6;
		}else{//not expectedseqnum
			//printf("rdt_recv:Received a retransmission DATA packet from peer!! sequence num = %d now the expectedseqnum =%d\n",packet_recv.sequenceNum,expectedseqnum);
			printf("rdt_recv:Received out-of-order DATA packet - SeqNo = %d (expected : %d)\n",packet_recv.sequenceNum,expectedseqnum);
			//retransmit last ACK
			
			packet_send.packet_type = 1;//ACK
			if(expectedseqnum==0){
				packet_send.sequenceNum = W-1;
			}else{
				packet_send.sequenceNum = expectedseqnum-1;
			}

			//packet_send.sequenceNum = last_acknum;
			packet_send.checksum = 0;
			
			u8b_t tmp3[len_ACK];
			memcpy(tmp3, &packet_send, len_ACK);
			packet_send.checksum = checksum(tmp3,sizeof(tmp3));

			if((udt_send(fd,&packet_send,len_ACK,0))==-1){
				perror("udt_send");
				return -1;
			}
			memset(&packet_recv,'\0',sizeof(packet_recv));
			continue;
		}


	}else{//ACK
		printf("rdt_recv:Received an unexpected ACK, just ignore it\n");
		memset(&packet_recv,'\0',sizeof(packet_recv));
		continue;
	}//end of packet_type check

    }//end While
}//end of rdt_recv()

/* Application process calls this function to close the RDT socket.
*/
int rdt_close(int fd){
	Packet packet_recv,packet_send;
	memset(packet_recv.payload,'\0',PAYLOAD);

	int status;
	fd_set read_fds;
	FD_ZERO(&read_fds);
	FD_SET(fd,&read_fds);
	struct timeval timer;
	timer.tv_sec = 0;
	timer.tv_usec = TWAIT;

	for(;;){
		FD_ZERO(&read_fds);
		FD_SET(fd,&read_fds);

	if ((status = select(fd+1, &read_fds, NULL, NULL, &timer)) == -1) {
		perror("close");
		return -1;
	}else if(status==0){//timeout close the connection
		printf("rdt_close:Nothing hapepend for 500.00 milliseconds.\n");
		printf("rdt_close:Release the socket.\n");
		close(fd);
		return 0;
	}
	//if received a packet 
	if((recv(fd,&packet_recv,sizeof(packet_recv),0))==-1){
			perror("recv");
			return -1;
	}
	//check if corrupted first
	
		u8b_t packet_check[sizeof(packet_recv)];
		memcpy(packet_check, &packet_recv, sizeof(packet_recv));
		u16b_t checksum_tmp = checksum(packet_check,sizeof(packet_check));
		//if(checksum_tmp==packet_recv.checksum){
		if(checksum_tmp==0){
			//not corrupted
			if(packet_recv.packet_type==0&&packet_recv.sequenceNum==expectedseqnum-1){
					packet_send.packet_type = 1;//ACK

					packet_send.sequenceNum = expectedseqnum-1;
					packet_send.checksum = 0;
					
					u8b_t tmp[sizeof(packet_send)];
					memcpy(tmp, &packet_send, sizeof(packet_send));
					packet_send.checksum = checksum(tmp,sizeof(tmp));

					if((udt_send(fd,&packet_send,sizeof(packet_send),0))==-1){
					perror("udt_send");
					return -1;
					}
					printf("rdt_close:Received a out-of-order DATA packet,resend the ACK again-SeqNo = %d\n",packet_recv.sequenceNum);
					continue;
			}
		}//end of the only one action situation
		continue;
	}//end of for


}

#endif
