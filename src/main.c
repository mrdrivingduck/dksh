#include <stdio.h>
#include <unistd.h>
#include "lexer.h"

int main() {

	char cmd[80] = "python3 src/main/a.py -a 999 | grep \"pux>\" log";
	char *tokens[80] = { NULL };

	generate_token(cmd, tokens, sizeof(tokens) / sizeof(tokens[0]));
	for (int i = 0; i < sizeof(tokens) / sizeof(tokens[0]); i++) {
		if (tokens[i] == NULL) {
			break;
		}
		printf("%s\n", tokens[i]);
	}
	clean_up_tokens(tokens);

	return 0;
}