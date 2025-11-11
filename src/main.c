#include "shell.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    char* cmdline;
    char** args;

    while (1) {
        cmdline = read_cmd("FCIT> ");
        if (cmdline == NULL) break; // Ctrl+D

        args = tokenize(cmdline);
        if (args == NULL) {
            free(cmdline);
            continue;
        }

        execute_command(args);

        // Free memory safely
        free(args);      // free array of pointers
        free(cmdline);   // free the command line string
    }

    printf("\n");
    return 0;
}

