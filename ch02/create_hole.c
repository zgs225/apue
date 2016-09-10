#include <apue.h>
#include <fcntl.h>

char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";

int create(const char * path, mode_t mode);

int main(void)
{
  int fd;

  if ((fd = create("file.hole", FILE_MODE)) < 0)
    err_sys("create error");

  if (write(fd, buf1, 10) != 10)
    err_sys("write error");

  if (lseek(fd, 16384, SEEK_CUR) == -1)
    err_sys("lseek error");

  if (write(fd, buf2, 10) != 10)
    err_sys("write error");

  exit(0);
}

int create(const char * path, mode_t mode)
{
  return open(path, O_WRONLY | O_CREAT | O_TRUNC, mode);
}
