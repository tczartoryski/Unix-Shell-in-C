#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "vector.h"
#include <stdlib.h>
#include <fcntl.h>

#define MAX_INPUT 260
static char *previous = NULL; // represents the previous command
			 
void create_pipe(char** tokens);
void process_tokens(char **tokens);
void clear(char **tokens);
void redirect(char** tokens, char* file, int directory); // rearrange
void execute_pipe(char** left, char** right);    //run
void create_redirect(char** tokens);
void execute_tokens(char** tokens);


// calculates number of tokens
int num_tokens(char** tokens) {
	int size = 0;
	char** holder = tokens;
	while (*holder) {
		size++;
		++holder;
	}
	return size;
}


// replaces the previous command with new previous command 
void new_previous(char *buffer) {
	int num_char = strlen(buffer);  // number of char in the bugger
   	buffer[num_char-1] = '\0'; 
   	char *previous_pointer = strstr(buffer, "prev"); // pointer for previous command
   if (previous_pointer == NULL) {  
        free(previous);           // frees memory at previous
        previous = strdup(buffer);   //sets previous equal to the buffer
    }
}

// pass in an array of tokens and clears the array
void clear(char **tokens) {
	int i;
	for (i = 0; i < MAX_INPUT; i++) {
		tokens[i] = NULL;
	}
}

// processes a line of tokens inputted to the shell
void process(char *buffer) {
	
	char *input[MAX_INPUT];  // represents input
	clear(input);            // removes anything that might be in there
	int current = 0;        // current char we are on
	char **tokens = create_tokens(buffer); 
      	char **holder;
        holder = tokens;
        while (*holder) { 
               if (strcmp(*holder,";") == 0) { // checks if semicolon is hit
                          current++;
	                  input[current] = NULL;
	                  process_tokens(input); 
	                  clear(input);
	                  current = 0;
                	  ++holder;      
			  continue;
	       }


	       input[current] = *holder;
	       current++;
	       ++holder;

	}
	process_tokens(input); // execute on remaining args
	free_data(tokens);
}

// returns true if one of the tokens is a pipe
int has_pipe(char** tokens) {
	int i;
	for (i = 0; tokens[i] != NULL; i++) {
     		if (strcmp(tokens[i], "|") == 0) {
			return 1;
		}
	}
	return 0;
}

// returns true if one of the tokens is a redirect
int has_redirect(char** tokens) {
	int i;
	for (i = 0; tokens[i]!= NULL; i++) {
		if (strcmp(tokens[i], "<") == 0 || strcmp(tokens[i], ">") == 0) {
			return 1;
		}
	}
	return 0;
}
                                                                                                            

// creates a pipe and then runs it
void create_pipe(char** tokens) {
	int i;
	int j;
	int x;
	for (i = 0; tokens[i] != NULL; i++) { 

		// splits array into right and left sections if there is a pipe
		if (strcmp(tokens[i], "|") == 0) {

			char *left[MAX_INPUT];
			clear(left);
			for (j = 0; j < i; j++) {
				left[j] = tokens[j];
			}
			// ^^ left side put in left array

			char *right[MAX_INPUT];
	   		clear(right);
	
			for(x = 0; tokens[i+x+1]!= NULL; x++) {

				right[x] = tokens[i+x+1];

			}
			// ^^ right side put in right array
			execute_pipe(left, right);
			return;
		} 
	}
}


// pipe is executed using the left and right tokens
void execute_pipe(char** left, char** right) {
	// child creates pipe
	if (fork() == 0) {


		int io_pipe[2];

		pipe(io_pipe);
		
		int io_write = io_pipe[1];
		int io_read = io_pipe[0];



		
		// another child creates a pipe

		if (fork() == 0) {

			
			if (close(io_read) == -1) {

				perror("Cannot close read end of pipe");
				exit(1);
			}


			
			

			if (close(1) == -1) {

				perror("Cannot close stdout");
				exit(1);
			}

			dup(io_write);

			// at this point stdout is closed and read end of pipe takes its place
			

			process_tokens(left);

			exit(0); // takeout

		} 
		else {    

			if (close(io_write) == -1) {

				perror("Pipe output closed");
				exit(1);

			}


		
			if (close(0) == -1) {
	
				perror("Stdin closed");

				exit(1);

			}

			dup(io_read);
			// at this point stdin is closed and write end of pipe takes its place

			process_tokens(right);
			wait(NULL);
			exit(0);
		}
	} 
	else {
		wait(NULL);
	}

} 
                                                                                                                                                                                                                                                                                                                                                               


// creates redirect 
void create_redirect(char** tokens) {
	int i;
	int j;
	char *redirect_tokens[MAX_INPUT];
	
	
	for (i = 0; tokens[i] != NULL; i++) {
		// checks to see where redirect happens
		if (strcmp(tokens[i], "<") == 0) {
			for (j = 0; j < i; j++) {
				redirect_tokens[j] = tokens[j];
			}

			redirect_tokens[i] = NULL;

			redirect(redirect_tokens, tokens[i+1], 0);

		} else if (strcmp(tokens[i], ">") == 0) { // if > is hit, then dir = 1   SWITCH THESE

			for (j = 0; j < i; j++) {
				redirect_tokens[j] = tokens[j];
			}

			redirect_tokens[i] = NULL;

			redirect(redirect_tokens, tokens[i+1], 1);
		}
	}
}                         

// using directory does redirect using tokens and file
void redirect(char** tokens, char* file, int directory) {

	
	if (fork() == 0) {

		if (directory == 0) { 
			// closes stdin
			if (close(0) == -1) {

				perror("Cannot close stdin");
				_exit(1);
			}

			int new_file = open(file, O_RDONLY); 
		} else {

			if (close(1) == -1) {
				perror("Cannot close stdout");
				_exit(1);
			}

			int new_file = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		}

		execvp(tokens[0], tokens);

		perror("Could not execute");
		_exit(1);
  
	} else {
		wait(NULL);
	}
}





// executes tokens without redirect or pipe
void execute_tokens(char** tokens) {

	if (!tokens[0]) {
		return;
	}

        	
	if (strcmp(tokens[0], "exit") == 0 && num_tokens(tokens) == 1) { 
		printf("Bye bye.\n");
		free(previous); 
		exit(0);

	} else if (strcmp(tokens[0], "cd") == 0 && num_tokens(tokens) == 2) { 
		chdir(tokens[1]); 
	} else if (strcmp(tokens[0], "source") == 0 && num_tokens(tokens) == 2) { 
		FILE *file = fopen(tokens[1], "r"); 
		if (file == NULL) {
			printf("Cannot open :  %s\n", tokens[1]);
		}

		char input[MAX_INPUT];

		while (fgets(input, MAX_INPUT, file)) {
			process(input);
		}

	} else if (strcmp(tokens[0], "prev") == 0 && num_tokens(tokens) == 1) { 

		if (previous != NULL) { 

			printf("%s\n", previous);
			process(previous);
		} else {
			printf("There are no previous commands\n");
		}

	} else if (strcmp(tokens[0], "help") == 0 && num_tokens(tokens) == 1) { 

		printf("Help Menu:\n");
		printf("help : prints all possible in the shell\n");
		printf("cd : change the directory\n");
		printf("previous : executes the previous command\n");
		printf("source [file] : turns each line in the file into a command and executes it\n");
		printf("exit : exits the shell\n");
		
	} else { 
	
		if (fork() == 0) {
	 		
                        
			execvp(tokens[0], tokens);
			_exit(1);
		} else {
			wait(NULL);
		}
	}
}


// decides whether to pipe, redirect, or just execute
void process_tokens(char **tokens) {

	if (has_pipe(tokens)) {
		create_pipe(tokens);
	} else if (has_redirect(tokens)) {
		create_redirect(tokens);
	} else {
		execute_tokens(tokens);
	}
}


int main(int argc, char **argv) {


	printf("Welcome to mini-shell.\n");

	while(1) {
		printf("shell $ ");
		char buffer[MAX_INPUT]; 

		if (fgets(buffer, MAX_INPUT, stdin) == NULL) {
			printf("Bye bye.\n");
			free(previous);
			return 0;
		} 

		process(buffer); 
		new_previous(buffer); 

	}

	free(previous); 
	return 0;

}


