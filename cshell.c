#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>

char *userInput = NULL;
size_t inputSize = 0;

char *WORKING_DIR = NULL;
char *USER = NULL;
char HOSTNAME[HOST_NAME_MAX];
char *TEXT = NULL;

int main() {
    const char *PATH = "/home/milind/dev/c/tmp/";
    WORKING_DIR = getcwd(NULL, 0);

    if (WORKING_DIR == NULL) {
        perror("Error getting current working directory");
        return 1; // Exit with an error code
    }

    USER = getenv("USER");

    if (USER == NULL) {
        perror("Error getting the current user's name");
        return 1; // Exit with an error code
    }

    if (gethostname(HOSTNAME, HOST_NAME_MAX) == -1) {
        perror("Error getting hostname");
        return 1; // Exit with an error code
    }

    int textBufferSize = snprintf(NULL, 0, "%s@%s:%s$", USER, HOSTNAME, WORKING_DIR);
    if (textBufferSize < 0) {
        perror("Error calculating textBufferSize");
        return 1;
    }

    TEXT = (char *)malloc(textBufferSize + 1); // +1 for the null terminator
    if (TEXT == NULL) {
        perror("Error allocating memory for TEXT");
        return 1;
    }

    if (sprintf(TEXT, "%s@%s:%s$", USER, HOSTNAME, WORKING_DIR) < 0) {
        perror("Error constructing TEXT");
        return 1;
    }

    while (1) {
        printf("%s ", TEXT);

        if (getline(&userInput, &inputSize, stdin) == -1) {
            perror("Error reading user input");
            return 1; 
        }

        userInput[strcspn(userInput, "\n")] = 0;

        if (strcmp(userInput, "exit") == 0) {
            break; 
        }
        else {
            char fullBinaryPath[PATH_MAX];
            if (snprintf(fullBinaryPath, PATH_MAX, "%s%s", PATH, userInput) < 0) {
                perror("Error constructing fullBinaryPath");
                return 1;
            }

            int status;
            pid_t pid = fork();

            if (pid == -1) {
                perror("Fork failed");
                return 1; 
            } 
            else if (pid == 0) {
                execl(fullBinaryPath, userInput, (char *)0);
                perror("Execution failed");
                exit(1);
            } 
            else {
                int childStatus;
                if (waitpid(pid, &childStatus, 0) == -1) {
                    perror("Wait for child failed");
                    return 1; 
                }
                if (WIFEXITED(childStatus)) {
                    int exitStatus = WEXITSTATUS(childStatus);
                    if (exitStatus != 0) {
                        fprintf(stderr, "Child process exited with a non-zero status: %d\n", exitStatus);
                    }
                }
            }
        }
    }

    // Free memory when you're done
    free(userInput);
    free(WORKING_DIR);
    free(TEXT);

    return 0;
}
