#ifndef _LEXER_
#define _LEXER_

int generate_token(char *cmd, char *tokens[], int max_token_count);
void clean_tokens(char *tokens[]);

#endif