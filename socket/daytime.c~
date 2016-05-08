#define _BSD_SOURCE
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

#define MY_PORT_NUM 49999

int makeConnection();
int setUpServerAddr(int socketfd, char* host);
int connectToServer(int socketfd, struct sockaddr_in* servAddr);

int main(int argc, char** argv){
	char buffer[512];

	if(argc < 2){
		printf("Too few argument !!\n");
		exit(1);
	}	

	int socketfd = makeConnection();
	int connectfd = setUpServerAddr(socketfd, argv[1]);
	int n =0;
	while((n = read(socketfd, buffer, 512)) > 0){
		printf("%s", buffer);
	}

	return 0;
} 

int makeConnection(){
	return socket(AF_INET, SOCK_STREAM, 0);
}

int setUpServerAddr(int socketfd, char* host){
	struct sockaddr_in servAddr;
	struct hostent* hostEntry;
	struct in_addr **pptr;

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(MY_PORT_NUM);

	hostEntry = gethostbyname(host);
	if(hostEntry == NULL){herror("host name"); exit(1);}

	// this is magic
	pptr = (struct in_addr **) hostEntry->h_addr_list;
	memcpy(&servAddr.sin_addr, *pptr, sizeof(struct in_addr));

	int connectfd =  connect(socketfd, (struct sockaddr *) &servAddr, sizeof(servAddr));
	if(connectfd < 0) {perror("connect"); exit(1);}	
	printf("client is connected to %s\n", hostEntry->h_name);
	
	return connectfd;
}

