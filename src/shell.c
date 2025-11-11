#include <stdio.h>      // printf, fprintf, stderr, perror
#include <stdlib.h>     // malloc, free, exit
#include <string.h>     // strtok, strlen, strcmp
#include <unistd.h>     // chdir, fork, execvp
#include <sys/types.h>  // pid_t
#include <sys/wait.h>   // wait

#define MAX_LEN 1024
#define MAXARGS 64

// Read a command line from the user
char* read_cmd(char* prompt) {
    printf("%s", prompt);
    char* cmdline = malloc(MAX_LEN);
    if (!fgets(cmdline, MAX_LEN, stdin)) {
        free(cmdline);
        return NULL; // Ctrl+D pressed
    }

    size_t len = strlen(cmdline);
    if (len > 0 && cmdline[len - 1] == '\n') {
        cmdline[len - 1] = '\0';
    }

    return cmdline;
}

// Split command line into arguments
char** tokenize(char* cmdline) {
    if (cmdline == NULL) return NULL;

    char** args = malloc(sizeof(char*) * (MAXARGS + 1));
    if (!args) return NULL;

    int i = 0;
    char* token = strtok(cmdline, " \t");
    while (token != NULL && i < MAXARGS) {
        args[i++] = token; // store pointers into cmdline
        token = strtok(NULL, " \t");
    }
    args[i] = NULL;
    return args;
}

// Handle built-in commands: exit, cd
int handle_builtin(char** args) {
    if (args == NULL || args[0] == NULL) return 0;

    if (strcmp(args[0], "exit") == 0) {
        printf("Exiting shell...\n");
        exit(0);
    }

    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "cd: expected argument\n");
        } else {
            if (chdir(args[1]) != 0) {
                perror("cd");
            }
        }
        return 1; // handled internally
    }

    return 0; // not a built-in
}

// Execute external commands
void execute_command(char** args) {
    if (args == NULL || args[0] == NULL) return;

    // handle built-ins first
    if (handle_builtin(args)) return;

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
    } else if (pid == 0) {
        // child process
        if (execvp(args[0], args) == -1) {
            perror("Command not found");
        }
        exit(1);
    } else {
        // parent waits
        wait(NULL);
    }
}

