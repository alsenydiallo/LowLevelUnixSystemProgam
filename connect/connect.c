/*
Overview:
---------
% connect    friday   :   wc  -l
Write a program "connect" works like a shell pipeline
The colon argument breaks argv into a left portion and a right portion

parent: 	left portion   runs with stdout = pipe write end
child:	right portion runs with stdin   = pipe read end
Consider swapping these parent/child to avoid parent to died and rip the child 

*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>         // for fork()
#include <sys/types.h>      // for pid_t
#include <sys/wait.h>       // for waitpid()
#include <assert.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]){

	int fd[2];			/* pipe file descriptors */
	int rdr, wtr;		/* fd[0] and fd[1] */
	int pid;
	
	if(argc <= 3){ 
		printf("Too few argument !!! \n");	
		printf("./connect arg1 : arg2\n");
		exit(0); 
	};

	int i=1;
	for(; i < argc; i++){
		if(strcmp(argv[i], ":") == 0){
			argv[i] = NULL;
			break;
		}
	}
	//check if right command provided
	if(i == (argc - 1)){ 
		printf("No right argument provided after the : \n");
		printf("./connect arg1 : arg2\n");
		exit(1);
	}
	//check if left command provided
	if(i == 1){ 
		printf("No left argument provided after the : \n");
		printf("./connect arg1 : arg2\n");
		exit(1);
	}


	if(pipe(fd) < 0){ fprintf(stderr, " Piping failled\n"); exit(0);};
	rdr = fd[0];
	wtr = fd[1];

	pid = fork();
    if (pid < 0) {
        printf("fork error %d\n", pid);
	return -1;
    }
    else if (pid != 0) {
        // code that runs in the parent process
        i++;
        close(wtr);
		close(0); dup(rdr); close(rdr);
		if(execvp(argv[i], &argv[i]) == -1)
			fprintf(stderr, "%s: exec of failed\n", argv[1]);
		close(rdr);
    }
    else {
        // code that runs in the child process
		//i++;
        close(rdr);   
		close(1); dup(wtr); close(wtr);
		if(execvp(argv[1], &argv[1]) == -1)
			fprintf(stderr, "%s: exec of failed\n", argv[1]);		
		close(wtr);
    }	
	return 0;
}
