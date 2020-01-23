/********************************************************************
 * 
 * @author Mr Dk.
 * @version 2020/01/22
 * 
 * Core function of the shell.
 * 
 ********************************************************************/


#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "shell.h"
#include "lexer.h"


char SHELL_HINT[] = "dksh->> ";

char LOG_MISSING_ARG[] = "dksh: Missing argument.\n";
char LOG_MISSING_PIPE_TARGET[] = "dksh: Missing pipe target.\n";
char LOG_UNRECOGNIZED_CHAR[] = "dksh: Unrecognized character.\n";
char LOG_TOKEN_OVERFLOW[] = "dksh: Memory overflow.\n";
char LOG_TOO_MANY_ARGS[] = "cd: Too many arguments.\n";
char LOG_NO_SUCH_PATH[] = "cd: No such path.\n";


int child_process_handler(char *tokens[], int last_token) {
    /* index of the first token to be executed in this process */
    /* if there is no piping, it will be 0 */
    int start_token = 0;
    
    /* terminate the argument list */
    tokens[last_token + 1] = NULL;
    /* search the token from the tail */
    for (int i = last_token; i >= 0; i--) {
        if (0 == strcmp(tokens[i], "|")) {
            /* there is a pipe */
            if (i + 1 > last_token || i - 1 < 0) {
                /* pipe target should exist */
                write(STDERR_FILENO, LOG_MISSING_PIPE_TARGET,
                      strlen(LOG_MISSING_PIPE_TARGET) + 1);
            }
            /* create the pipe */
            int fd[2];
            pipe(fd);
            /* create another process to execute the former command */
            /* <<<<<<<< another process >>>>>>>> */
            if (0 == fork()) {
                /* the pipe is copied by the fork() */
                /* override the STDOUT with the pipe */
                /* close the pipe */
                /* now output to STDOUT will be injected into the pipe */
                dup2(fd[1], STDOUT_FILENO);
                close(fd[0]);
                close(fd[1]);
                /* execute the former command */
                child_process_handler(tokens, i - 1);
                exit(0);
            }
            /* <<<<<<<< another process >>>>>>>> */
            /* override the STDIN with the pipe */
            /* close the pipe */
            /* the pipe will inject to this process from STDIN */
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);
            close(fd[1]);
            /* the first token after the '|' */
            start_token = i + 1;
            break;
        } else if (0 == strcmp(tokens[i], ">")) {
            if (i + 1 > last_token || i - 1 < 0) {
                /* redirecting target should exist */
                write(STDERR_FILENO, LOG_MISSING_ARG,
                      strlen(LOG_MISSING_ARG) + 1);
                exit(0);
            }
            /* open the redirecting target */
            /* override the STDOUT with the redrecting target */
            /* close the redirecting target */
            int redirect_fd = open(tokens[i + 1], O_CREAT | O_RDWR, 0666);
            dup2(redirect_fd, STDOUT_FILENO);
            close(redirect_fd);
            /* skipping the redirecting target */
            /* terminate its parameters */
            tokens[i] = NULL;
            i--;
        } else if (0 == strcmp(tokens[i], "<")) {
            if (i + 1 > last_token || i - 1 < 0) {
                /* redirecting target should exist */
                write(STDERR_FILENO, LOG_MISSING_ARG,
                      strlen(LOG_MISSING_ARG) + 1);
                exit(0);
            }
            /* open the redirecting target */
            /* override the STDIN with the redrecting target */
            /* close the redirecting target */
            int redirect_fd = open(tokens[i + 1], O_CREAT | O_RDWR, 0666);
            dup2(redirect_fd, STDIN_FILENO);
            close(redirect_fd);
            /* skipping the redirecting target */
            /* terminate its parameters */
            tokens[i] = NULL;
            i--;
        }
    }

    if (0 == strcmp(tokens[start_token], "cd")) {
        /* the command 'cd' should use 'chdir()' */
        if (last_token - start_token > 1) {
            /* too many arguments for 'cd' */
            write(STDERR_FILENO, LOG_TOO_MANY_ARGS, 1 + strlen(LOG_TOO_MANY_ARGS));
        } else if (last_token - start_token == 1 &&
                   -1 == chdir(tokens[start_token + 1])) {
            /* 'chdir()' failed */
            write(STDERR_FILENO, LOG_NO_SUCH_PATH, 1 + strlen(LOG_NO_SUCH_PATH));
        }
    } else {
        /* other command should be 'execvp()' */
        execvp(tokens[start_token], tokens + start_token);
    }

    return 0;
}

void shell_core_loop() {
	char cmd[80] = { 0 };
	char *tokens[80] = { NULL };
    int max_token = sizeof(tokens) / sizeof(tokens[0]);

	write(STDOUT_FILENO, SHELL_HINT, strlen(SHELL_HINT) + 1);
	while (NULL != fgets(cmd, sizeof(cmd), stdin)) {
        /* remove the ending '\n' */
        /* use 'fgets()' instead of 'gets()' */
		cmd[strlen(cmd) - 1] = '\0';

        /* empty command */
		if (strlen(cmd) == 0) {
            write(STDOUT_FILENO, SHELL_HINT, strlen(SHELL_HINT) + 1);
            continue;
        }

        /* split the tokens */
        int token_count = generate_token(cmd, tokens, sizeof(tokens) / sizeof(tokens[0]));
        if (token_count > 0 && token_count < max_token) {
            /* the number of tokens is reasonable */
            if (0 == strcmp("cd", tokens[0]) && !need_fork(tokens, token_count)) {
                /* for 'cd' command, operate on the main process's PCB */
                /* no need to fork a new process */
                /* piping is no support for 'cd' */
                child_process_handler(tokens, token_count - 1);
            } else {
                /* for other command, fork a new process to execute */
                /* wait until the execution terminated */
                int pid = fork();
                if (0 == pid) {
                    child_process_handler(tokens, token_count - 1);
                    exit(0);
                }
                waitpid(pid, NULL, 0);
            }
        } else if (token_count == UNRECOGNIZED_CHAR) {
            /* there exists unrecognized character */
            write(STDERR_FILENO, LOG_UNRECOGNIZED_CHAR, strlen(LOG_UNRECOGNIZED_CHAR) + 1);
        } else if (token_count == OVERFLOW) {
            /* the number of tokens exceeds to limit */
            write(STDERR_FILENO, LOG_TOKEN_OVERFLOW, strlen(LOG_TOKEN_OVERFLOW) + 1);
        }

        /* free the memory for all tokens */
        clean_up_tokens(tokens);
		
        /* ready to accept the next command */
		write(STDOUT_FILENO, SHELL_HINT, strlen(SHELL_HINT) + 1);
	}
}

int need_fork(char *tokens[], int token_count) {
    for (int i = 0; i < token_count; i++) {
        /* if there is a pipe, then a fork is necessary */
        if (0 == strcmp(tokens[i], "|")) {
            return 1;
        }
    }
    return 0;
}