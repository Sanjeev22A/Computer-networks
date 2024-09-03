#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#define MAX_INPUT_LENGTH 1024

void handle_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {
    char url[MAX_INPUT_LENGTH];
    char filename[MAX_INPUT_LENGTH];
    char command[MAX_INPUT_LENGTH * 2 + 50]; // Sufficient size to hold the command string

    // Get URL from the user
    printf("Enter the URL/resource: ");
    if (fgets(url, sizeof(url), stdin) == NULL) {
        handle_error("fgets");
    }
    // Remove newline character from input
    url[strcspn(url, "\n")] = '\0';

    // Get filename from the user
    printf("Enter the filename to save the resource: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        handle_error("fgets");
    }
    // Remove newline character from input
    filename[strcspn(filename, "\n")] = '\0';

    // Construct the command string
    snprintf(command, sizeof(command), "lwp-request \"%s\" > \"%s\"", url, filename);

    // Execute the command
    int ret = system(command);
    if (ret == -1) {
        handle_error("system");
    }

    printf("Resource from %s has been saved to %s\n", url, filename);

    return 0;
}
