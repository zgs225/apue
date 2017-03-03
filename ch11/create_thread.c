#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_t ntid;

void printids(const char *str)
{
    pid_t pid;
    pthread_t tid;

    pid = getpid();
    tid = pthread_self();
    printf("%s pid %lu tid %lu (0x%lx)\n", str, (unsigned long) pid, (unsigned long) tid, (unsigned long) tid);
}

void *
thr_fn(void *arg)
{
    printids("new thread: ");
    return ((void *) 0);
}

int main(void)
{
    int err;

    if ((pthread_create(&ntid, NULL, thr_fn, NULL)) != 0)
    {
        printf("can't create thread");
        exit(err);

    }

    printids("main thread: ");
    sleep(1);

    return 0;
}
