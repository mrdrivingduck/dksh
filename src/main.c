#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "lexer.h"
#include "shell.h"

int main() {

	char cmd[80] = "cat <input.txt | sort | uniq | cat >output.txt";
	char *tokens[80] = { NULL };

	int token_count = generate_token(cmd, tokens, sizeof(tokens) / sizeof(tokens[0]));
	if (0 == fork()) {
		child_process_handler(tokens, token_count, 0);
		exit(0);
	}
	
	clean_up_tokens(tokens);

	return 0;
}