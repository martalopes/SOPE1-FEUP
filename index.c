#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


#define ERROR 1
#define OK 0

int main(int argc, char * argv[]){

	char * path_to_dir = argv[1];
	int status;

	//-----------creates the path to words.txt------------
	char path_to_words[strlen(path_to_dir)+10];	
	strncpy(path_to_words, path_to_dir, strlen(path_to_dir));
	strcat(path_to_words, "/words.txt");


	if(access(path_to_words, F_OK) != 0) //checks if the file exists
		exit(ERROR);

	int file_counter = 1;
	


	while(1){
		char fname[33] = "";
		sprintf(fname, "%d", file_counter); 	//int to string, to get the name of the next file
		char * path_to_file = malloc(strlen(path_to_dir) * sizeof(char));
		
		//-----------creates the path to the file------------
		strncpy(path_to_file, path_to_dir, strlen(path_to_dir));
		strcat(path_to_file, "/");
		strcat(path_to_file, fname);
		strcat(path_to_file, ".txt" );

		if(access(path_to_file, F_OK) != 0) //checks if the file exists
			break;

		pid_t pid = fork();

		if(pid == 0){	
			execlp("./swexec", "./swexec", path_to_words, path_to_file, NULL);		//executes sw with the file given

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

	
		execlp("./cscexec", "./cscexec", path_to_dir, NULL);		//executes csc with the file given
	


	exit(OK);
}