#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    pid_t pid;
    int fds[2];
    int n;
    char line[2048];

    if (pipe(fds) < 0) {
        fprintf(stderr, "pipe failed\n");
        exit(1);
    }

    if ((pid = fork()) < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (pid > 0) { // parent
        close(fds[0]);
        write(fds[1], "hello world\n", 12);
    } else if (pid == 0) {
        close(fds[1]);
        n = read(fds[0], line, 2048);
        write(STDOUT_FILENO, line, n);
    }

    exit(0);
}
