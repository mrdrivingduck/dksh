/********************************************************************
 * 
 * @author Mr Dk.
 * @version 2020/01/10
 * 
 * 
 * 
 * 
 ********************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lexer.h"

#define PASS 1
#define NOTPASS 0

int generate_token(char cmd[], char *tokens[], int max_token_count) {
    int count = 0;
    char *worker = cmd;
    int token_len = 0;

    printf("%s\n", cmd);

    while (*worker != '\0') {
        while (is_blank(worker)) {
            worker++;
        }
        if (token_len = is_operator(worker)) {
            if (count >= max_token_count ||
                (tokens[count] = (char *) malloc(token_len + 1)) == NULL) {
                return -1;
            }
            printf("operator\n");
            strncat(tokens[count], worker, token_len);
            count++;
            worker += token_len;
            continue;
        }
        if (token_len = is_str(worker)) {
            if (count >= max_token_count ||
                (tokens[count] = (char *) malloc(token_len + 1)) == NULL)
            {
                return -1;
            }
            strncat(tokens[count], worker + 1, token_len - 2);
            count++;
            worker += token_len;
            continue;
        }
        if (token_len = is_token(worker)) {
            if (count >= max_token_count ||
                (tokens[count] = (char *) malloc(token_len + 1)) == NULL)
            {
                return -1;
            }
            strncat(tokens[count], worker, token_len);
            count++;
            worker += token_len;
            continue;
        }
    }

    return count;
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
            if (*search++ == *worker) {
                break;
            }
        }
        return search - worker;
    }
    return 0;
}

int is_token(char *worker) {
    if (*worker > 32 && *worker < 127) {
        char *search = worker + 1;
        while (*search) {
            if (*search > 32 && *search < 127 && !is_operator(search)) {
                search++;
                continue;
            }
            break;
        }
        return search - worker;
    }
    return 0;
}

void clean_tokens(char *tokens[]) {
    int i = 0;
    while (tokens[i] != NULL) {
        free(tokens[i]);
        tokens[i] = NULL;
    }
}