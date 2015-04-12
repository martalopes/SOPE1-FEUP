#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>


#define ERRORPIPE 1
#define ERRORFORK 1
#define OK 0
#define SIZE 255

int main(int argc, char* argv[])
{

	char *path_to_file = argv[2];
	char *path_to_wordsfile = argv[1];
	int filedes[2];
	int status;

	if (pipe(filedes) < 0) {  // checks pipe error
		fprintf(stderr, "pipe error\n"); 
		exit(ERRORPIPE); 
	} 

	pid_t pid = fork();
	if (pid < 0) { 		// checks fork error

		fprintf(stderr, "fork error\n"); 
		exit(ERRORFORK); 
	} 
	else if(pid == 0){

		close(filedes[0]); // close read  
		dup2(filedes[1], STDOUT_FILENO);
		execlp("grep", "grep", "-n", "-o", "-w", "-f", path_to_wordsfile, path_to_file, NULL);
	}
	else {
		wait(&status);

		close(filedes[1]); // close write
		FILE* stream = fdopen(filedes[0], "r");
		char line[SIZE];

		//--CREATING A FILE---
		//finds file name
		char * p_bar;
		//char * p_period;
		int last_bar = 0;
		int last_period = 0;
		p_bar = strrchr(path_to_file, '/'); //finds last occurrence of "/"
		if (p_bar != NULL)
			last_bar = p_bar - path_to_file; 	// last "/" position
		
		/*p_period = strrchr(path_to_file, '.');	//finds last occurence of "."
		if(p_period != NULL)
			last_period = p_period - path_to_file; 		// last "." position
			*/

		int size_of_name = strlen(path_to_file) - last_bar - 5;
		char filename[SIZE];
		strncpy(filename, path_to_file+last_bar+1, size_of_name);
		strcat(filename, "_index.txt");

		FILE * pFile;
		pFile = fopen(filename, "w");

		while (fgets(line, 40, stream) != NULL){
			
			//WRITES THE WORD
			char * pch;  
			char dest[SIZE]; 
			int nr_colon;
			
			pch = strchr(line,':');	// finds first occurrence of ":"
			nr_colon = pch-line;	// ":" position

			char num[SIZE];			
			strncpy(num, line, nr_colon); //gets the number of the line where the word is
			
			strncpy(dest, pch+1, strlen(line) ); //copies to dest everything after ":"
			dest[strlen(dest) - 1] = '\0';

			strcat(dest, " : "); // writes in dest ":"


			//WRITES FILE NAME
			char newdest[SIZE] = "";

			strncpy(newdest, path_to_file+last_bar+1, size_of_name); //writes in dest the name of the file
			strcat(dest, newdest);

			//WRITES NUMBER OF LINE WHERE THE WORD IS
			strcat(dest, "-");
			
			strcat(dest, num);

			if (pFile!=NULL){
				strcat(dest, "\n");
				fputs(dest, pFile);
			}
		}

		fclose(pFile);
		fclose(stream);
		close(filedes[0]);
	}

	exit(OK);
}