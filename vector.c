#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// largest vector size
#define VECTOR_MAX 260

static char **vector = NULL;
static int vector_capacity;
static int vector_size; 
static void create_vector();
static int parse_string(const char *buffer, char *holder);
static void add_token(const char *token);


// duplicates string
char *strdup(const char *str)
{
	int n = strlen(str) + 1;
	char *dup = malloc(n);
	if(dup)
	{
		strcpy(dup, str);
	}
	return dup;
}


//Free vector memory
void free_data(char **tokens) {
	char **holder = tokens;
	while (*holder != NULL) {
		free(*holder);
		++holder;
	}
	free(vector);
}


// parses the string and returns number of characters in between quotation mark
int parse_string(const char *buffer, char *holder) {

     	unsigned int size = 0;

	while (*buffer && *buffer != '"') {
		*holder = *buffer;
		++size;
		++holder;
		++buffer;

	}

	*holder = '\0';
	return size;                                                                                               }


// turns a buffer line input into tokens in the form of a vector
char **create_tokens(const char *buffer) {
    create_vector();
    int i = 0; // holder index
    int j = 0; // buffer index
    char holder[260];
    
    // parses through each char in buffer
    while (buffer[j] != 0) {
	    switch (buffer[j]) {
		    case ' ':
	  	    case '\n':	
  		    case '\t':			
		  	    if (i > 0) {
	  			    holder[i] = '\0';
  				    add_token(holder);
				    i = 0;
			    }

			    break;
		    case '>': 		                     
		    case ')': 
		    case '<': 
		    case '(': 
		    case '|': 
		    case ';':
			    if (i > 0) {
				    holder[i] = '\0';	
				    add_token(holder);
				    i = 0;	  
			    }
		  		    holder[0] = buffer[j];  // rearrange
			  	    holder[1] = '\0';
			  	    add_token(holder);
			  	    break;
		    case '"':    
		  		    ++j;
		  		    int size = parse_string(&buffer[j], &holder[i]);
		  		    i += size;
		  		    j += size;
				    break; 
		    default:
				    holder[i] = buffer[j];
				    ++i;                                                                                                                                                                                                      
	    }                                                                                                                                                                                           
	    ++j; 
    }
    

     // Handles last token
    if (i > 0) {
	    holder[i] = 0;
	    add_token(holder);
    }
     
    return vector;
}

  


// grows the vector
void grow_vector() {
	vector_capacity += VECTOR_MAX;	
	vector = realloc(vector, sizeof(char *) * vector_capacity);
}

// returns whether or not vector is full             
int vector_full() {						
	return vector_size + 1 == vector_capacity;
}

// adds a token to the vector grows it if it is full
void add_token(const char* token) {
	
	if (vector_full())
		grow_vector();
	
	char *hold = strdup(token);
	vector[vector_size] = hold;
	++vector_size;
	vector[vector_size] = NULL;
}
                
// creates the vecotr
void create_vector() {
	vector_size = 0;
	vector_capacity = VECTOR_MAX;
	vector = malloc(sizeof(char *) * vector_capacity);
	vector[0] = NULL;
}




