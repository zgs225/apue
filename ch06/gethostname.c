#include <apue.h>
#include <unistd.h>

int main(void)
{
  char *name = NULL;
  if ((gethostname(name, sizeof(name))) == -1)
    err_sys("gethostname error");
  fputs(name, stdout);

  return 0;
}
