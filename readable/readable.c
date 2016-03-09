/*
Overview:
---------
Takes one or zero command line arguments.
Argument, if present, is a pathname (relative or absolute).  If no argument is present, the pathname of the present working directory is assumed (getcwd).
Readable recursively traverses the directories and their files, starting with the pathname, and lists every regular file that is readable by the current process.
Each regular file is listed to stdout as a full pathname, one pathname per line.
No order is defined for the pathnames that are output.

Other stuff:
------------
Ignore symbolic links (Why?)
Submit one source file via email: readable.c
Design your solution in advance of coding.
You will need: access, opendir, readdir, closedir, lstat
*/

#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/param.h>

void isReadable(char * pathName){
	if(pathName == NULL) return;
	struct stat area,  *s=&area;
	DIR *currDir;
	struct dirent *dir;
	char* currPath = NULL;

	if(access(pathName, R_OK) == 0){
		if(lstat(pathName,s) == 0) {
			if(S_ISREG(s->st_mode)){	
				printf("%s\n", pathName);
				free(pathName);
			}
			else if(S_ISDIR(s->st_mode)){
				currDir = opendir(pathName);			
				while((dir = readdir(currDir)) != NULL){
					if(strcmp(dir->d_name, ".") != 0 
					&& strcmp(dir->d_name, "..") != 0){
						currPath = malloc(sizeof(char)*(
								strlen(pathName)+strlen(dir->d_name)+2));
						strcpy(currPath, pathName);
						strcat(currPath, "/");		
						strcat(currPath, dir->d_name);						
						isReadable(currPath);
					}
				}
				closedir(currDir);
			}
		}
	}
	else{
		printf("... File doesn't exist or can't get to it ...\n");
	}
}

int main(int argc, char *argv[]){
	char *pathName = NULL;	
	if(argc == 2){
		chdir(argv[1]);
	}
	pathName  = getcwd(pathName, 511);
	isReadable(pathName);
	free(pathName);
	return 0;
}
