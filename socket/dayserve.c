#define _BSD_SOURCE
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define MY_PORT_NUM 49999

void bindSocketToPort(int listenfd);
int makeSocket();
int listen_accept_connect(int listenfd);
char* getHostName(const struct sockaddr_in clientAddr);


int main(int argc, char** argv){
	int listenfd = makeSocket();
	bindSocketToPort(listenfd);
	int n=0;

	pid_t pid;
	int status;
	int connectfd;
	listen(listenfd, 1);
	while((connectfd = listen_accept_connect(listenfd)) > 0){
		
		pid = fork();
		if(pid < 0){ printf("fork failed\n"); exit(1);}
		if(pid){ // parent process
			close(connectfd);
		}
		else{// child process
			time_t rawtime;
			time(&rawtime);
			char *msg = ctime(&rawtime);
			printf("writing ... \n");
			if((n = write(connectfd, msg, (strlen(msg) + 1))) < 0)
				perror("write");
			printf("Finished writing\n");
			exit(0);
		}
		
	}

	return 0;
}

int makeSocket(){
	return socket(AF_INET, SOCK_STREAM, 0);
}

void bindSocketToPort(int listenfd){
	struct sockaddr_in servAddr;
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(MY_PORT_NUM);
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(listenfd, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0) {
		perror("bind");
		exit(1);
	}
}

int listen_accept_connect(int listenfd){
//	listen(listenfd, 1);
	socklen_t length = sizeof(struct sockaddr_in);
	struct sockaddr_in clientAddr;
	int connectfd = accept(listenfd, (struct sockaddr*) &clientAddr, &length);

	if(connect < 0) {
		perror("connect"); exit(1);
	}
	printf("connected to host name: %s\n", getHostName(clientAddr));
	return connectfd;	
}

char* getHostName(const struct sockaddr_in clientAddr){
	struct hostent* hostEntry;
	char* hostname;

	hostEntry = gethostbyaddr(&(clientAddr.sin_addr), sizeof(struct in_addr), AF_INET);
	hostname = hostEntry->h_name;
	if(hostname == NULL) {
		herror("host name"); exit(1);
	}
	return hostname;
}


