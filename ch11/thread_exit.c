#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *
thr_fn1(void *arg)
{
    printf("thread 1 is returning...\n");
    return ((void *) 1);
}

void *
thr_fn2(void *arg)
{
    printf("thread 2 is exiting...\n");
    pthread_exit((void *) 2);
}

int main(void)
{
    int err;
    pthread_t tid1, tid2;
    void * ret_val;

    if ((err = pthread_create(&tid1, NULL, thr_fn1, NULL)) != 0)
    {
        printf("can't create thread\n");
        exit(err);
    }

    if ((err = pthread_create(&tid2, NULL, thr_fn2, NULL)) != 0)
    {
        printf("can't create thread\n");
        exit(err);
    }

    if ((err = pthread_join(tid1, &ret_val)) != 0)
    {
        printf("can't join thread\n");
        exit(err);
    }

    printf("thread 1 exit code: %ld\n", (long) ret_val);

    if ((err = pthread_join(tid2, &ret_val)) != 0)
    {
        printf("can't join thread\n");
        exit(err);
    }

    printf("thread 2 exit code %ld\n", (long) ret_val);

    return 0;
}
