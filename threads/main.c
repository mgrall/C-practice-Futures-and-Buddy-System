#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <lib.h>

char* longRunningTask(void* data) {
    char* result = (char*) calloc(sizeof(char), 16);

    if (snprintf(result, 16, "sleep result %u", sleep(8)) < 0) {
        perror("snprintf");
    }
    return result;
}

int main(int argc, char** argv) {
    printf("Hello World\n");

    BsFuture fut = runAsyncTask(longRunningTask, NULL);

    printf("It started\n");

    char* result = awaitTask(&fut);

    printf("Result: %s, Future State is %s\n", result, FUTURE_STATE_STRING[fut.state]);

    return 0;
}
