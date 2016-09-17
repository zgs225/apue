#include "apue.h"
#include <unistd.h>

#define BUF_SIZE 4096

int main(void)
{
  int  n;
  char buf[BUF_SIZE];

  while((n = read(STDIN_FILENO, buf, BUF_SIZE)) > 0)
    if (write(STDOUT_FILENO, buf, n) != n)
      err_sys("write error");

  if (n < 0)
    err_sys("read error");

  exit(0);
}
