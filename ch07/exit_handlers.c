#include <apue.h>

static void exit_handler1(void);
static void exit_handler2(void);

int main(void)
{
  if (atexit(exit_handler1) != 0)
    err_sys("can't register exit_handler1");

  if (atexit(exit_handler2) != 0)
    err_sys("can't register exit_handler2");

  if (atexit(exit_handler1) != 0)
    err_sys("can't register exit_handler1");

  printf("main is done\n");

  return(0);
}

static void exit_handler1(void)
{
  printf("first exit handler\n");
}

static void exit_handler2(void)
{
  printf("second exit handler\n");
}
