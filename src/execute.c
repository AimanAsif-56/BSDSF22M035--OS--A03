#include "shell.h"
#include <stdio.h>      // perror
#include <stdlib.h>     // exit
#include <unistd.h>     // fork, execvp
#include <sys/types.h>  // pid_t
#include <sys/wait.h>   // waitpid

void execute(char** arglist) {
    if (arglist == NULL || arglist[0] == NULL) return;

    pid_t cpid = fork();
    if (cpid < 0) {
        perror("fork failed");
        exit(1);
    } else if (cpid == 0) {
        // child process
        if (execvp(arglist[0], arglist) == -1) {
            perror("Command not found");
            exit(1);
        }
    } else {
        // parent waits for child
        int status;
        waitpid(cpid, &status, 0);
    }
}

