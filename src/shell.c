/********************************************************************
 * 
 * @author Mr Dk.
 * @version 2020/01/21
 * 
 * Core function of the shell.
 * 
 * 
 ********************************************************************/


#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


char LOG_MISSING_ARG[] = "Missing argument\n";


int child_process_handler(char *tokens[], int token_count, int start_token) {
    
    for (int i = start_token; i < token_count; i++) {
        if (0 == strcmp(tokens[i], "|")) {
            if (i + 1 < token_count) {
                int fd[2];
                pipe(fd);
                if (0 == fork()) {
                    dup2(fd[0], 0);
                    close(fd[0]);
                    close(fd[1]);
                    child_process_handler(tokens, token_count, i + 1);
                    exit(0);
                }
                /* current process */
                dup2(fd[1], 1);
                close(fd[0]);
                close(fd[1]);
                tokens[i] = NULL;
                break;
            }
        } else if (0 == strcmp(tokens[i], ">")) {
            if (i + 1 >= token_count) {
                write(STDERR_FILENO, LOG_MISSING_ARG, strlen(LOG_MISSING_ARG) + 1);
                exit(0);
            }
            int redirect_fd = open(tokens[i + 1], O_CREAT | O_RDWR, 0666);
            dup2(redirect_fd, 1);
            close(redirect_fd);
            tokens[i] = NULL;
            i++;
        } else if (0 == strcmp(tokens[i], "<")) {
            if (i + 1 >= token_count) {
                write(STDERR_FILENO, LOG_MISSING_ARG, strlen(LOG_MISSING_ARG) + 1);
                exit(0);
            }
            int redirect_fd = open(tokens[i + 1], O_CREAT | O_RDWR, 0666);
            dup2(redirect_fd, 0);
            close(redirect_fd);
            tokens[i] = NULL;
            i++;
        }
    }
    tokens[token_count] = NULL;
    
    execvp(tokens[start_token], tokens + start_token);

    return 0;
}

