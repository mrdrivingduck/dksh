/********************************************************************
 * 
 * @author Mr Dk.
 * @version 2020/01/22
 * 
 * Split input command into tokens.
 * 
 * 
 ********************************************************************/


#ifndef _LEXER_
#define _LEXER_

#define PASS 1
#define NOTPASS 0

#define OVERFLOW -1
#define UNRECOGNIZED_CHAR -2

int generate_token(char *command, char *tokens[], int max_token_count);
void clean_up_tokens(char *tokens[]);

int consume_blank(char *worker);
int consume_operator(char *worker);
int consume_string(char *worker);
int consume_token(char *worker);

#endif