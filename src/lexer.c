/********************************************************************
 * 
 * @author Mr Dk.
 * @version 2020/01/16
 * 
 * Split input command into tokens.
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

    /* not reaching the end of the command */
    while (*worker != '\0') {
        while (find_blank(worker)) {
            /* skip the blank */
            worker++;
        }
        if (token_len = find_operator(worker)) {
            /* valid operator */
            if (count >= max_token_count ||
                (tokens[count] = (char *) malloc(token_len + 1)) == NULL) {
                /* failed to allocate memory for token */
                return -1;
            }
            /* copy the operator to the token set */
            strncat(tokens[count], worker, token_len);
            count++;
            worker += token_len;
            continue;
        }
        if (token_len = find_str(worker)) {
            /* a valid string */
            if (count >= max_token_count ||
                (tokens[count] = (char *) malloc(token_len + 1)) == NULL)
            {
                return -1;
            }
            /* copy the string to the token set */
            /* skip the quote */
            strncat(tokens[count], worker + 1, token_len - 2);
            count++;
            worker += token_len;
            continue;
        }
        if (token_len = find_token(worker)) {
            /* a valid token */
            if (count >= max_token_count ||
                (tokens[count] = (char *) malloc(token_len + 1)) == NULL)
            {
                return -1;
            }
            /* copy the token to the token set */
            strncat(tokens[count], worker, token_len);
            count++;
            worker += token_len;
            continue;
        }
    }

    return count;
}

int find_blank(char *worker) {
    if (*worker == ' ') {
        return 1;
    }
    return 0;
}

int find_operator(char *worker) {
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

int find_str(char *worker) {
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

int find_token(char *worker) {
    if (*worker > 32 && *worker < 127) {
        char *search = worker + 1;
        while (*search) {
            if (*search > 32 && *search < 127 && !find_operator(search)) {
                /* 33-126 are ASCII code for characters can be displayed */
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