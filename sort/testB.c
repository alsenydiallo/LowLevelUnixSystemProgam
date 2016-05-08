#include "getWord.h"
#include "sort.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sort.h"


#define ARRAY_SZ 5000000
#define NBR_OF_THREAD 5
char *array[ARRAY_SZ];
int count = 0;

void makeArray(char* fileName){
	FILE *fp = fopen(fileName, "r");
	char *temp = NULL;

	if(fp == NULL) {printf("file is not open\n"); exit(1);}

	while((count < ARRAY_SZ) && (temp = getNextWord(fp)) != NULL){
		array[count] = (char*) malloc(sizeof(char) * (strlen(temp)+1));
		strcpy(array[count], temp);
		free(temp);
		count++;
	}
	free(fp);
}

void printArray(char* ar[]){
	for(int i = 0; i < count; i++)
		printf("%d : %s\n",i, ar[i]);
}

void destroyArray(char *ar[]){
	for(int i = 0; i < count; i++)
		free(array[i]);
}

int main(int argc, char **argv){

	if(argc == 1) {printf("No fileName or Path provided\n"); exit(1);}
	
	makeArray(argv[1]);
	setSortThreads(NBR_OF_THREAD);
	sortThreaded(array, count);
	printArray(array);
//	destroyArray(array);

	return 0;
}
