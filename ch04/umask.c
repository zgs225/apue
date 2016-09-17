#include <apue.h>
#include <fcntl.h>

#define RWRWRW (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

int create(const char *pathname, mode_t mode);

int main(void)
{
  umask(0);

  if (create("foo", RWRWRW) < 0)
    err_sys("create error for foo");

  umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

  if (create("bar", RWRWRW) < 0)
    err_sys("create error for bar");

  exit(0);
}

int create(const char *pathname, mode_t mode)
{
  return open(pathname, O_WRONLY | O_CREAT | O_TRUNC, mode);
}
