#ifndef SHELL_H
#define SHELL_H

#define MAX_LEN 1024
#define MAXARGS 64

char* read_cmd(char* prompt);
char** tokenize(char* cmdline);
int handle_builtin(char** args);
void execute_command(char** args);

#endif

