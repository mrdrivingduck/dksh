/********************************************************************
 * 
 * @author Mr Dk.
 * @version 2020/01/22
 * 
 * Core function of the shell.
 * 
 * 
 ********************************************************************/

#ifndef _SHELL_
#define _SHELL_

int child_process_handler(char *tokens[], int last_token);

void shell_core_loop();

int need_fork(char *tokens[], int token_count);

#endif
