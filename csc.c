#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <sys/wait.h>

#define ERRORPIPE 1
#define ERRORFORK 1
#define OK 0
#define SIZE 255

int main(int argc, char * argv[]){
	
	int status;

	int filedes[2];

	if(pipe(filedes) < 0){

		fprintf(stderr, "\n CSC: pipe error\n");
		exit(ERRORPIPE);
	}


	char *path_to_dir = argv[1];
	char path_to_index[strlen(path_to_dir) + strlen("/index.txt")];
	strncpy(path_to_index, path_to_dir, strlen(path_to_dir));
	strcat(path_to_index, "/index.txt");

//INSERTS THE CONTENT OF THE FILE ON PIPE AND REMOVES THE FILE
	char filename[SIZE] = "";
	int file_counter = 1;

	while(1)
	{
		sprintf(filename, "%d", file_counter);
		strcat(filename, "_index.txt");

		if(access(filename, F_OK) != 0)
			break;


	pid_t pid = fork();

	if(pid < 0){
		fprintf(stderr, "CSC: first fork error\n"); 
		exit(ERRORFORK); 
	}
	else if(pid == 0){
		close(filedes[0]); // close read  
		dup2(filedes[1], STDOUT_FILENO);
		execlp("cat", "cat", filename, NULL);
	}
	else
		wait(&status);



	//**REMOVES FILE**
	pid_t pidd = fork();

	if(pidd < 0){
		fprintf(stderr, "CSC: second fork error\n"); 
		exit(ERRORFORK); 
	}
	else if(pidd == 0){
		execlp("rm", "rm", filename, NULL);
	}
	else
		wait(&status);
	//****************

	file_counter++;
	}	

//******************************************************************



	
	close(filedes[1]); // close write

	FILE* stream = fdopen(filedes[0], "r"); // reads everything that is on the pipe
	char line[SIZE];

	//WRITES ON THE OUTPUT FILE UNSORTED
	FILE* output = fopen(path_to_index, "w");

	if(output == NULL)
		fclose(output);

	while(fgets(line, SIZE, stream) != NULL)
		fputs(line, output);
	
	fclose(output);
	//***************************

	//SORTS
	char * temp_index_path = malloc((strlen(path_to_index) + 4) * sizeof(char));
	strncpy(temp_index_path,  path_to_index, strlen(path_to_index) - 4);
	strcat(temp_index_path, "_temp.txt");
	
	pid_t pid = fork();

	if(pid < 0){
		fprintf(stderr, "CSC: third fork error\n"); 
		exit(ERRORFORK); 
	}
	else if(pid == 0){
		execlp("sort", "sort", "-V", "-f", path_to_index, "-o", temp_index_path, NULL);
	}
	else
		wait(&status);
	//*******


	//WRITES ON THE OUTPUT FILE SORTED
	FILE* sorted_input = fopen(temp_index_path, "r");
	FILE* sorted_output = fopen(path_to_index, "w");

	if(sorted_output  == NULL)
		fclose(sorted_output);

	if(sorted_input == NULL)
		fclose(sorted_input);

	char * name = malloc((SIZE+1) * sizeof(char));
	
	fputs("INDEX", sorted_output);

	while(fgets(line, SIZE, sorted_input) != NULL){
		

		char name2[SIZE] = "";
		int i = 0;

		while(line[i] != ' '){
			if(i == SIZE){
				break;
			}
			name2[i] = line[i];
			i++;
		}

		int size_n2 = i;
		
		name2[size_n2] = '\0';

		if(strncmp(name, name2, strlen(name2)) != 0)
		{
			strcat(name, "\n\n");
			fputs(name, sorted_output);

			name = malloc((SIZE+1) * sizeof(char));
		
			strncpy(name, name2, (strlen(name2)+1));
			strcat(name, ": ");

			char tmp[25] = "";
			strncpy(tmp, line + strlen(name2) + 3 , strlen(line) - 4 - strlen(name2));
			strcat(name, tmp);
		}
		else{

			char tmp[25] = "";
			
			strncpy(tmp, line + strlen(name2) + 3, strlen(line) - 4 - strlen(name2));
			strcat(name, ", ");
			strcat(name, tmp);
		}
	}

	char lastn[10] = "\n";
	fputs(name, sorted_output);
	fputs(lastn, sorted_output);

	//REMOVES TEMPORARY INDEX
	pid_t pidf = fork();

	if(pidf < 0){
		fprintf(stderr, "CSC: second fork error\n"); 
		exit(ERRORFORK); 
	}
	else if(pidf == 0){
		execlp("rm", "rm", temp_index_path, NULL);
	}
	else
		wait(&status);
	
	//**********************

	fclose(sorted_output);
	fclose(sorted_input);
	




	exit(OK);

	}
