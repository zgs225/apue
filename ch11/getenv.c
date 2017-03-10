#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAX_STRLEN 4096

pthread_once_t once_done = PTHREAD_ONCE_INIT;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_key_t key;

extern char **environ;

static void thread_init(void)
{
    pthread_key_create(&key, free);
}

char *getenv(const char *name)
{
    int i, len;
    char *envbuf;

    pthread_once(&once_done, thread_init);
    pthread_mutex_lock(&mutex);

    if ((envbuf = pthread_getspecific(key)) == NULL)
    {
        envbuf = malloc(MAX_STRLEN);
        if (envbuf == NULL)
        {
            pthread_mutex_unlock(&mutex);
            return NULL;
        }
        pthread_setspecific(key, envbuf);
    }

    len = strlen(name);

    for (i = 0; environ[i] != NULL; i++)
    {
        if (strncmp(name, environ[i], len) == 0 && environ[i][len] == '=')
        {
            strncpy(envbuf, &environ[i][len+1], MAX_STRLEN - 1);
            pthread_mutex_unlock(&mutex);
            return envbuf;
        }
    }

    pthread_mutex_unlock(&mutex);
    return NULL;
}
