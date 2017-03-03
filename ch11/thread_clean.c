#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void cleanup(void *arg)
{
    printf("cleanup: %s\n", (char *) arg);
}

void *
thr_fn1(void *arg)
{
    printf("thread 1 start\n");
    pthread_cleanup_push(cleanup, "thread 1 first cleanp handler")
    pthread_cleanup_push(cleanup, "thread 1 second cleanup handler")
    printf("thread 1 push complete\n");

    if (arg) {
        return ((void *) 1);
    }

    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);

    return ((void *) 1);
}

void *
thr_fn2(void *arg)
{
    printf("thread 2 start\n");
    pthread_cleanup_push(cleanup, "thread 2 first cleanp handler")
    pthread_cleanup_push(cleanup, "thread 2 second cleanup handler")
    printf("thread 2 push complete\n");

    if (arg) {
        pthread_exit((void *) 2);
    }

    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);

    return ((void *) 1);
}

int main(void)
{
    int err;
    pthread_t tid1, tid2;
    void *ret_val;

    if ((err = pthread_create(&tid1, NULL, thr_fn1, (void *) 1)) != 0)
    {
        printf("can't create thread\n");
        exit(err);
    }

    if ((err = pthread_create(&tid2, NULL, thr_fn2, (void *) 1)) != 0)
    {
        printf("can't create thread\n");
        exit(err);
    }

    if ((err = pthread_join(tid1, &ret_val)) != 0)
    {
        printf("can't join to thread %ld", (long) tid1);
        exit(err);
    }

    printf("thread 1 exit with code %ld\n", (long) ret_val);

    if ((err = pthread_join(tid2, &ret_val)) != 0)
    {
        printf("can't join to thread %ld", (long) tid2);
        exit(err);
    }

    printf("thread 2 exit with code %ld\n", (long) ret_val);

    return 0;
}
