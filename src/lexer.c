#include <stdlib.h>
#include <string.h>
#include "lexer.h"

#define PASS 1
#define NOTPASS 0

int generate_token(char *cmd, char *tokens[], int max_token_count) {
    int count = 0;
    char *worker = cmd;
    char *base;
    int token_len = 0;

    while (worker != '\0') {
        while (is_blank(*worker)) {
            worker++;
        }
        if (token_len = is_operator(*worker)) {
            if (count >= max_token_count || !tokens[count] = (char *) kalloc(token_len + 1)) {
                return -1;
            }
            strcpy(tokens[count], worker, token_len);
            count++;
            worker += token_len;
            continue;
        }
        if (token_len = is_str(*worker)) {
            if (count >= max_token_count || !tokens[count] = (char *) kalloc(token_len + 1)) {
                return -1;
            }
            strcpy(tokens[count], worker, token_len);
            count++;
            worker += token_len;
            continue;
        }
    }
}

int is_blank(char *worker) {
    if (*worker == ' ') {
        return 1;
    }
    return 0;
}

int is_operator(char *worker) {
    if (*worker == '|') {
        return 1;
    }
    if (*worker == '>') {
        return 1;
    }
    if (*worker == '<') {
        return 1;
    }
    return 0;
}

int is_str(char *worker) {
    if (*worker == '\'' || *worker == '"') {
        char *search = worker + 1;
        while (*search) {
            if (*search == *worker) {
                return search - worker
            }
            search++;
        }
        return search - worker;
    }
    return 0;
}

void cleanTokens(char *tokens[]) {

}