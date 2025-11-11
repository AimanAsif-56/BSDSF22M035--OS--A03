include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>

#define MAX_LEN 1024
#define MAXARGS 64
#define ARGLEN 128

// Function to read a command line from user
char* read_cmd(char* prompt, FILE* fp) {
    printf("%s", prompt);
    char* cmdline = (char*) malloc(sizeof(char) * MAX_LEN);
    int c, pos = 0;

    while ((c = getc(fp)) != EOF) {
        if (c == '\n') break;
        cmdline[pos++] = c;
    }

    if (c == EOF && pos == 0) {
        free(cmdline);
        return NULL; // Ctrl+D
    }

    cmdline[pos] = '\0';
    return cmdline;
}

// Function to split command line into tokens
char** tokenize(char* cmdline) {
    if (cmdline == NULL || cmdline[0] == '\0' || cmdline[0] == '\n') {
        return NULL;
    }

    char** arglist = (char**)malloc(sizeof(char*) * (MAXARGS + 1));
    for (int i = 0; i < MAXARGS + 1; i++) {
        arglist[i] = (char*)malloc(sizeof(char) * ARGLEN);
        bzero(arglist[i], ARGLEN);
    }

    int argc = 0;
    char* token = strtok(cmdline, " \t");
    while (token != NULL && argc < MAXARGS) {
        strcpy(arglist[argc++], token);
        token = strtok(NULL, " \t");
    }
    arglist[argc][0] = '\0'; // null terminate last arg
    return arglist;
}

// Function to handle built-in commands
int handle_builtin(char** args) {
    if (args == NULL || args[0] == NULL) return 0;

    // exit command
    if (strcmp(args[0], "exit") == 0) {
        printf("Exiting shell...\n");
        exit(0);
    }

    // cd command
    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "cd: expected argument\n");
        } else {
            if (chdir(args[1]) != 0) {
                perror("cd");
            }
        }
        return 1; // handled
    }

    return 0; // not built-in
}

// Function to execute commands (external or built-in)
void execute_command(char** args) {
    if (args == NULL || args[0][0] == '\0') return;

    // Check for built-in commands first
    if (handle_builtin(args)) return;

    // External commands (placeholder)
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return;
    } else if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("Command not found");
        }
        exit(1);
    } else {
        // Parent process waits
        wait(NULL);
    }
}

// Main loop
int main() {
    char* cmdline;
    char** args;

    while (1) {
        cmdline = read_cmd("FCIT> ", stdin);
        if (cmdline == NULL) {
            printf("\n");
            break; // Ctrl+D
        }
        args = tokenize(cmdline);
        execute_command(args);

        // Free allocated memory
        free(cmdline);
        if (args != NULL) {
            for (int i = 0; i < MAXARGS + 1; i++) free(args[i]);
            free(args);
        }
    }

    return 0;
}

