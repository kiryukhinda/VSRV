#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COUNT 200

void ChildProcess(void);
void ParentProcess(void);
void CreateProcesses(void);

int main() {
    CreateProcesses();
    return 0;
}

void CreateProcesses(void) {
    pid_t pid;
    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } 
    else if (pid == 0) {
        ChildProcess();
    } 
    else {
        ParentProcess();
    }
}

void ChildProcess(void) {
    for (int i = 1; i <= MAX_COUNT; i++) {
        printf("   This line is from child, value = %d\n", i);
    }
    exit(0);
}

void ParentProcess(void) {
    for (int i = 1; i <= MAX_COUNT; i++) {
        printf("This line is from parent, value = %d\n", i);
    }
}
