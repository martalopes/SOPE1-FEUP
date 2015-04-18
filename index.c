#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


#define ERROR 1
#define OK 0

int main(int argc, char * argv[]){

	int status;
	char * path_to_dir = malloc(sizeof(char) * (strlen(argv[1]) + 10));

	if (argv[1][strlen(argv[1])-1] == '/')
		strncpy(path_to_dir, argv[1], strlen(argv[1]) - 1);
	else
		strcpy(path_to_dir, argv[1]);

	

	//-----------creates the path to words.txt------------
	char * path_to_words = malloc(sizeof(char) * (strlen(argv[1]) + 10));

	strncpy(path_to_words, path_to_dir, strlen(path_to_dir));
	strcat(path_to_words, "/words.txt");

	if(access(path_to_words, F_OK) != 0) //checks if the file exists
		exit(ERROR);

	int file_counter = 1;	

	while(1){
		
		char fname[50] = "";
		sprintf(fname, "%d", file_counter); 	//int to string, to get the name of the next file
		char * path_to_file = malloc((40 + strlen(path_to_dir)) * sizeof(char));
		
		//-----------creates the path to the file------------
		strncpy(path_to_file, path_to_dir, strlen(path_to_dir));
		strcat(path_to_file, "/");
		strcat(path_to_file, fname);
		strcat(path_to_file, ".txt" );

		if(access(path_to_file, F_OK) != 0) //checks if the file exists
			break;

		pid_t pid = fork();

		if(pid == 0){	
			execlp("./bin/swexec", "./bin/swexec", path_to_words, path_to_file, NULL);		//executes sw with the file given

		}
		else if (pid > 0){
			file_counter++;
			wait(&status);
		}
		else{
			fprintf(stderr, "INDEX: sw fork error\n"); 
			exit(ERROR);
		}


		
		wait(&status);
	}

		execlp("./bin/cscexec", "./bin/cscexec", path_to_dir, NULL);		//executes csc with the file given
	


	exit(OK);
}