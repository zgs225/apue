#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

char buf[10000];

int main(void)
{
    char *ptr;
    int nwrite = 0, ntowrite = 0;
    ntowrite = read(STDIN_FILENO, buf, sizeof(buf));

    int oldflag = fcntl(STDOUT_FILENO, F_GETFL);
    fcntl(STDOUT_FILENO, F_SETFL, O_NONBLOCK);

    ptr = buf;

    while(ntowrite > 0)
    {
        errno = 0;
        nwrite = write(STDOUT_FILENO, ptr, ntowrite);
        fprintf(stderr, "nwrite = %d, errno = %d\n", nwrite, errno);

        if (nwrite > 0)
        {
            ptr += nwrite;
            ntowrite -= nwrite;
        }
    }

    fcntl(STDOUT_FILENO, F_SETFL, oldflag & ~O_NONBLOCK);
    return 0;
}
