#include <stdio.h>
#include "vector.h"



int main(int argc, char **argv) {
  //Create a variable to take in stdin
    char buf[256];

    fgets(buf, 256, stdin);

    
    char **tokens = create_tokens(buf);
    char **token_pointer = tokens;

    while (*token_pointer != NULL) {

	    printf("%s\n", *token_pointer);
	    ++token_pointer;

    }
    free_data(tokens);
    return 0;

}

