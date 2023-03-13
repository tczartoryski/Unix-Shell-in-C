#ifndef _VECTOR_H
#define _VECTOR_H

/**
 *  * Takes in a buffer input and turns it into an array of tokens
 *  *
 *  * @param buffer input which is the line passed through shell
 *  * @return an array of tokens
 *  */
char **create_tokens(const char *buffer);

/**
 *  * String is duplicated
 *  *
 *  * @param str is string to duplicate
 *  * @return an string
 *  */
char *strdup(const char *str);

/**
 *  * Array of tokens is freed
 *  *
 *  * @param tokens an array of tokens
 *  */
void free_data(char **tokens);

#endif
