/********************************************************************
 * 
 * @author Mr Dk.
 * @version 2020/01/22
 * 
 * Split input command into tokens.
 * 
 * 
 ********************************************************************/


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lexer.h"


int generate_token(char command[], char *tokens[], int max_token_count) {
    int count = 0;
    char *worker = command;
    int token_len = 0;

    /* not reaching the end of the command */
    while (*worker != '\0') {
        while (consume_blank(worker)) {
            /* skip the blank */
            worker++;
        }
        if ((token_len = consume_operator(worker))) {
            /* valid operator */
            if (count >= max_token_count ||
                (tokens[count] = (char *) malloc(token_len + 1)) == NULL) {
                /* failed to allocate memory for token */
                return OVERFLOW;
            }
            /* copy the operator to the token set */
            strncat(tokens[count], worker, token_len);
            count++;
            worker += token_len;
            continue;
        }
        if ((token_len = consume_string(worker))) {
            /* a valid string */
            if (count >= max_token_count ||
                (tokens[count] = (char *) malloc(token_len + 1)) == NULL)
            {
                return OVERFLOW;
            }
            /* copy the string to the token set */
            /* skip the quote */
            strncat(tokens[count], worker + 1, token_len - 2);
            count++;
            worker += token_len;
            continue;
        }
        if ((token_len = consume_token(worker))) {
            /* a valid token */
            if (count >= max_token_count ||
                (tokens[count] = (char *) malloc(token_len + 1)) == NULL)
            {
                return OVERFLOW;
            }
            /* copy the token to the token set */
            strncat(tokens[count], worker, token_len);
            count++;
            worker += token_len;
            continue;
        }
        if (*worker == '\0') {
            break;
        }

        return UNRECOGNIZED_CHAR;
    }

    return count;
}

int consume_blank(char *worker) {
    if (*worker == ' ') {
        return 1;
    }
    return 0;
}

int consume_operator(char *worker) {
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

int consume_string(char *worker) {
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

int consume_token(char *worker) {
    if (*worker > 32 && *worker < 127) {
        char *search = worker + 1;
        while (*search) {
            if (*search > 32 && *search < 127 && !consume_operator(search)) {
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

void clean_up_tokens(char *tokens[]) {
    int i = 0;
    while (tokens[i] != NULL) {
        free(tokens[i]);
        tokens[i] = NULL;
    }
}