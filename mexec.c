#include "mexec.h"

char** extract_arguments(char *buffer, int *arg){

	char **arguments;
	arguments = NULL;
    char *command = strtok(buffer, " ");
    (*arg) = 0;

    while(command){
        arguments = realloc(arguments, sizeof(char *) * (++(*arg)));
        arguments[(*arg) - 1] = command;
        command = strtok(NULL, " ");
    }
    arguments = realloc(arguments, sizeof(char *) * (++(*arg)));
    arguments[(*arg) - 1] = NULL;
    return arguments;
}

void run_pipe(char **arguments, int in, int out){

	/* --- Open pipes --- */
	int pid = fork();
	if (pid < 0){
		perror("Error pipe()");
		exit(1);
	}
	else if (pid == 0){
		if (in != 0){
			dup2(in, 0);
			close (in);
		}
		if (out != 1){
			dup2(out, 1);
			close (out);
		}
	    int status_code = execvp(arguments[0], arguments);
        if(status_code == -1){
            perror(arguments[0]);
            exit(1);
        }
	}
	/* --- Wait for parent process to be finished --- */
	else if (pid > 0)
		wait(NULL);
}

int pip_counter(char buffer[]){

	int counter =0;
	for(int i=0; buffer[i] != '\0' ;i++){
	  if(buffer[i] == '|')
	  	counter++;
	}
	return counter;
}

void separation(char buff[]){

 char buffer2[100];
 int total_pipes = pip_counter(buff);
 //Now Total_Pipe contains the pipe present in the input
  int index = 0;
  //The first process should get its input from the original file descriptor 0.
  int z = 0;
  // Below loop will run for first n-1 command.
	int read = 0 ,fd[2];
	for (int i=0; i <total_pipes; i++){
		 for(z = 0; buff[index] != '|' && buff[index] != '\0'  ; index++, z++){
		   buffer2[z] = buff[index];
		   }
		buffer2[z] = '\0';

		index = index + 1;
		if (pipe(fd) == -1){
			perror("Error: Could not create pipe.");
			exit(1);
		}
		int num_of_arguments;
		char **proc_argc = extract_arguments(buffer2, &num_of_arguments);
		run_pipe(proc_argc, read, fd[1]);
		close(fd[1]);
		read = fd[0];
		free(proc_argc[num_of_arguments-1]);
		free(proc_argc);
	}
	// This is the last command being run now.
	for(z = 0; buff[index] != '|' && buff[index] != '\0'  ; index++, z++){
	  buffer2[z] = buff[index];
	}
	buffer2[z] = '\0';
	index = index +1;
	int num_of_arguments = 0;
	char **proc_argc = extract_arguments(buffer2, &num_of_arguments);
	run_pipe(proc_argc, read, 1);
	free(proc_argc[num_of_arguments-1]);
	free(proc_argc);
}

int main(int argc, char **argv){

	char fulltext[1024];
	fulltext[0]  = '\0';
	//2-Cases
	if(argc > 1){
		//Case - 1
		//read from file
		FILE *file_pointer = fopen(argv[1],"r");
		if(file_pointer){
			char line[1024];
	        while (fgets (line, 1024, file_pointer) != NULL) {
	        	line[strcspn(line, "\r\n")] = 0;
	            strcat(fulltext, line);
	            strcat(fulltext, "|");
	        }
	        int len = strlen(fulltext);
	        fulltext[len-1] = '\0';			
	        fclose(file_pointer);
	        separation(fulltext);
		}
		else if(!file_pointer){
			fprintf(stderr, "%s: ", argv[1]);
            perror("");
            exit(EXIT_FAILURE);
		}
		else{
			fprintf(stderr, "usage: ./mexec [FILE]\n");
			exit(EXIT_FAILURE);
		}
	}
	else{
		//Case - 2
		//Reading Data from Consolse
		//Continue to take data from user untill user press CTRL+D
		int count = 0;
		size_t len = 0;
		char *buffer= NULL;
		while(1){
			ssize_t read = 0;
			read = getline(&buffer, &len, stdin);
			if(read == EOF)
				break;
			int i;
			for(i = 0; buffer[i] != '\n'; i++, count++)
				fulltext[count] = buffer[i];
			fulltext[count] = '|';
			count++;
		}
		count--;
		fulltext[count] = '\0';
		//Remove any extra blank or space entered by the user
		char updated_text[1024];
		int c = 0, d = 0;
		while (fulltext[c] != '\0') {
		    if (!(fulltext[c] == ' ' && fulltext[c + 1] == ' ')) {
		      updated_text[d] = fulltext[c];
		      d++;
		    }
		    c++;
		}

  		updated_text[d] = '\0';

		separation(updated_text);
		free(buffer);
	}
	return 0;
}
