/*
Overview:
---------
Use binary search via lseek and read
if search range empty (bot >= top), return 0
set mid = (bot+top)/2; read that line into 'have' (don't read newline)
compare 'want' vs. 'have' (using strcmp)
if they are equal, return 1
if 'want' smaller than 'have', set top = mid; otherwise, set bot = mid+1
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

/*
 *	print out the word in the file
 */
int  okay(int fd, char* word){		
/*	
 *	find number of word inside the file 
 */	
	char buffer[16];
	for(int i=0; i<16; i++)
		   buffer[i] = ' ';	

	int ret = lseek(fd, 0, SEEK_END);
	int nbrWord = (ret/16);
	int top = nbrWord + 1;
	int bot=0;
	int mid=0;
	int test;
	
	while(top > bot){
		mid = (top+bot)/2;

		printf("mid: %d ", mid);
		printf("top: %d ", top);
		printf("bot: %d\n", bot);

		if((ret = (lseek(fd, mid*16, SEEK_SET) < 0)))
			printf("Error reading word");

	ret = read(fd, &buffer, 16);
			buffer[15] = '\0';
			test = strcmp(buffer, word);
			if(test == 0){
				return 1;
			}
			else if(test > 0){
				top = mid;
			}
			else if (test < 0){
				bot = mid + 1;
			}	
	}	
	return 0;	
}



int main(int argc, char* argv[]){

	int fd;
//	char fname ="/encs_share/cs/class/cs360/lib/webster";
	char fname[] ="webster";
/*
 *	fill space in the name to be search
 */
	char* word = (char*) malloc(sizeof(char) * (strlen(argv[1]) +1));
	strcpy(word, argv[1]);
	for(int i=strlen(argv[1]); i < 16; i++){
		word[i] = ' ';
	}
	word[15] = '\0';

	if(argc == 3){
		if((fd = open(argv[2], O_RDONLY, 0)) < 0){
			printf("file not open\n");
			exit(1);
		}
	}
	else if(argc == 2){	
		if((fd = open(fname, O_RDONLY, 0)) < 0){
			printf("file not open\n");
			exit(1);
		}
	}

	int ret = okay(fd, word);
	if(ret == 1)
		printf("Yes\n");
	else
		printf("No\n");
	
	close(fd);
	return 0;
}
