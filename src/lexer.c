#include "lexer.h"

#define PASS 1
#define NOTPASS 0

int generate_token(char *cmd, char *tokens[]) {
    int count = 0;
    char *worker = cmd;

    while (worker != '\0') {
        if (isBlank(*worker) == PASS) {
            worker++;
            continue;
        }
        if (isOperator(*worker) == PASS) {

        }
    }
}

int isBlank(char c) {
    if (c == ' ') {
        return PASS;
    }
    return NOTPASS;
}

int isOperator(char c) {

}

void cleanTokens(char *tokens[]) {

}