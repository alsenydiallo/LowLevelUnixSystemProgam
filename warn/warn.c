/*
Overview
---------
Design and implement program “warn”, as described below:

 The program will spawn a child process with a pipe shared between the parent and child. The parent will write to the pipe and the child will read from it. There are no required command line arguments.

  The parent process will intercept SIGINT and when a SIGINT is detected py the parent, it will prompt the user for a single line text message.  Once having received the message from the user, the parent will send (write) the message to the child via the shared pipe and send a SIGFPE signal to the child to get its attention. The parent process will repeat this behavior for any SIGINT it receives, until an "exit" message is received from the user (see below).

  The child process must ignore SIGINT.  When the child process detects a SIGFPE, it will read a single line message from the shared pipe.  The first token of the message may optionally be an integer representing a time in seconds (DELAY).  If the first token is not an integer, assume that the DELAY value is 5 seconds and the non-integer token is then part of the message.  The child will print the message (minus the leading integer, if any) to standard output.  It will repeat printing the message every DELAY seconds until the next SIGFPE is intercepted and a new message is read from the parent.  Implementation of the repetition interval shall be done using signal(), pause() and alarm().  Use of the library function sleep() and busy wait loops are not permitted.

  If the parent receives a message of “exit” from the user, the parent forwards the message to the child and waits for it to exit (terminate) and then performs its own exit.  Upon receipt of the “exit” message, the child prints a message to standard out indicating its intention to exit and then exits.
*/

#define _POSIX_SOURCE

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>         // for fork()
#include <sys/types.h>      // for pid_t
#include <sys/wait.h>       // for waitpid()
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <math.h>

char message[512];
int fd[2]; 			/* pipe file descriptor */
int delay = 5;		/* default value : 5 second */
int rdr, wtr; 		/* fd[0] and fd[1] */
char buffer[512];
int pid;

void printMessage(){
	printf("%s", message);
}

void my_sleep(){
	signal(SIGALRM, printMessage);
	alarm(delay);
	pause();
	alarm(0);
}

void parent_handler(){
	fprintf(stdout, "Enter a message: \n");
	fgets(buffer, 512, stdin);
}

int indexMsg(char str[]){
	int i = 0;
	while(isdigit(str[i]) || isspace(str[i]))
			++i;
	return i;
}
void child_handler(){
	char msg[512];
	msg[511] = '\0';
	read(fd[0], msg, 512);

	if(sscanf(&msg[0], "%i", &delay) == 1){
		int index = indexMsg(msg);
		strcpy(&message[0], &msg[index]);
	}else{
		strcpy(&message[0], &msg[0]);
	}
	message[511] = '\0';
}

int main(int argc, char *argv[]){

	if(pipe(fd) < 0) {fprintf(stderr, "Piping failled\n"); exit(1);}

	pid = fork();
	if (pid < 0){
			printf("fork error %d\n", pid);
	}
	else if(pid != 0){
		// parent process
		//close(fd[0]);	
		while(1){
			signal(SIGINT, parent_handler);
			pause();

		 	if(strcmp(buffer, "exit\n") == 0){
				printf("Child is diying\n");
				kill(pid, SIGKILL);
				wait(NULL);
				exit(1);
			}else{
				write(fd[1], buffer, 512);
				kill(pid, SIGFPE);
			}
		}
		//close(wtr);
	}
	else{
		// child process
		//close(wtr);
		signal(SIGINT, SIG_IGN);
		int flag = 1;
		while(flag == 1){
			signal(SIGFPE, child_handler);
			my_sleep();
		}
		//close(rdr);
	}
	return 0;
}
