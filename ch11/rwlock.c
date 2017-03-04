#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

#define LOOP_INTERVAL 3

struct job {
    struct job *j_prev;
    struct job *j_next;
    pthread_t j_id;
    void *(*j_fn)(void *);
    char *j_fn_arg;
    bool is_run;
    bool is_recv;
    void *recv_data;
};

struct queue {
    struct job *q_head;
    struct job *q_tail;
    pthread_rwlock_t q_lock;
};

struct queue main_q = {0};

// Init queue
int init_queue(struct queue *q)
{
    int err;
    q->q_head = NULL;
    q->q_tail = NULL;
    err = pthread_rwlock_init(&q->q_lock, NULL);
    if (err != 0)
        return err;
    return 0;
}

// Insert a job to queue
void job_insert(struct queue *q, struct job *j)
{
    pthread_rwlock_wrlock(&q->q_lock);

    j->j_next = q->q_head;
    j->j_prev = NULL;
    if (q->q_head != NULL) {
        q->q_head->j_prev = j;
    } else
        q->q_tail = j;
    q->q_head = j;

    pthread_rwlock_unlock(&q->q_lock);
}

// Append a job to queue
void job_append(struct queue *q, struct job *j)
{
    pthread_rwlock_wrlock(&q->q_lock);

    j->j_next = NULL;
    j->j_prev = q->q_tail;
    if (q->q_tail == NULL) {
        q->q_head = j;
    } else {
        q->q_tail->j_next = j;
    }
    q->q_tail = j;

    pthread_rwlock_unlock(&q->q_lock);
}

// Remove a job from queue
void job_remove(struct queue *q, struct job *j)
{
    pthread_rwlock_wrlock(&q->q_lock);

    if (j == q->q_head) {
        q->q_head = j->j_next;
        if (q->q_tail == j)
            q->q_tail = NULL;
        else
            j->j_next->j_prev = j->j_prev;
    } else if (j == q->q_tail) {
        q->q_tail = j->j_prev;
        j->j_prev->j_next = j->j_next;
    } else {
        j->j_prev->j_next = j->j_next;
        j->j_next->j_prev = j->j_prev;
    }

    pthread_rwlock_unlock(&q->q_lock);
}

// Find a job by id
struct job * job_find(struct queue *q, pthread_t id)
{
    struct job *j;

    pthread_rwlock_rdlock(&q->q_lock);

    for (j = q->q_head; j != NULL; j = j->j_next)
        if (pthread_equal(j->j_id, id))
            break;

    pthread_rwlock_unlock(&q->q_lock);

    return j;
}

void * worker_loop(void *arg)
{
    while(1)
    {
        struct job *j;
        pthread_rwlock_rdlock(&main_q.q_lock);

        for (j = main_q.q_head; j != NULL; j = j->j_next)
        {
            if (!j->is_run)
            {
                pthread_t tid;

                if (pthread_create(&tid, NULL, j->j_fn, j->j_fn_arg) != 0)
                {
                    printf("job with arg: %s run error\n", (char *) j->j_fn_arg);
                    job_remove(&main_q, j);
                    continue;
                }

                j->j_id = tid;
                j->is_run = true;
                j->is_recv = false;
            }
        }

        pthread_rwlock_unlock(&main_q.q_lock);
        sleep(LOOP_INTERVAL);
    }

    return (void *) 0;
}

void * reciever_loop(void *arg)
{
    while(1)
    {
        struct job *j;
        pthread_rwlock_rdlock(&main_q.q_lock);

        for (j = main_q.q_head; j != NULL; j = j->j_next)
        {
            if (j->is_run && !j->is_recv)
            {
                if (pthread_join(j->j_id, &j->recv_data) != 0)
                {
                    printf("thread %ld joined error\n", (long) j->j_id);
                    continue;
                }

                j->is_recv = true;
                printf("thread %ld executed, code: %d\n", (long) j->j_id, (int) j->recv_data);
            }
        }

        pthread_rwlock_unlock(&main_q.q_lock);
    }

    return (void *) 0;
}

void * fn(void *arg)
{
    printf("\nJOB: %s\n", (char *) arg);
    return (void *) 0;
}

void * inputer_loop(void *arg)
{
    while(1)
    {
        char buf[256] = {0};
        setvbuf(stdin, NULL, _IONBF, 0);
        fflush(stdout);
        puts("Please enter words: ");

        if (scanf("%s\n", buf) != 0)
        {
            printf("INPUT: %s\n", buf);
            struct job *j = malloc(sizeof(struct job));
            j->j_id = NULL;
            j->j_fn = fn;
            j->j_fn_arg = (void *) buf;
            j->is_run = false;
            j->is_recv = false;
            j->recv_data = NULL;
            job_append(&main_q, j);
        }

        fflush(stdout);
    }

    return (void *) 0;
}

int worker_start(pthread_t *tid)
{
    int err;
    pthread_t id;

    if ((err = pthread_create(&id, NULL, worker_loop, NULL)) != 0)
    {
        return err;
    }

    *tid = id;
    return 0;
}

int reciever_start(pthread_t *tid)
{
    int err;
    pthread_t id;

    if ((err = pthread_create(&id, NULL, reciever_loop, NULL)) != 0)
    {
        return err;
    }

    *tid = id;
    return 0;
}

int inputer_start(pthread_t *tid)
{
    int err;
    pthread_t id;

    if ((err = pthread_create(&id, NULL, inputer_loop, NULL)) != 0)
    {
        return err;
    }

    *tid = id;
    return 0;
}

int main(void)
{
    int err;
    pthread_t work_tid, recv_tid, input_tid;
    void *data;

    puts("init main queue");
    if ((err = init_queue(&main_q)) != 0)
    {
        printf("Init queue error: %d", err);
        exit(err);
    }
    puts("inited");

    puts("worker thread starting");
    if ((err = worker_start(&work_tid)) != 0)
    {
        printf("Worker thread start error: %d", err);
        exit(err);
    }
    puts("worker thread started");

    puts("reciever thread starting");
    if ((err = reciever_start(&recv_tid)) != 0)
    {
        printf("Reciever thread start error: %d", err);
        exit(err);
    }
    puts("reciever thread started");

    puts("inputer thread starting");
    if ((err = inputer_start(&input_tid)) != 0)
    {
        printf("Inputer thread start error: %d", err);
        exit(err);
    }
    puts("inputer thread started");

    pthread_join(work_tid, &data);
    pthread_join(recv_tid, &data);
    pthread_join(input_tid, &data);

    return 0;
}
