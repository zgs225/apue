#include <apue.h>

volatile sig_atomic_t quit_flag;

static void sig_int(int signo);

int main(void)
{
  sigset_t newmask, oldmask, zeromask;

  if (signal(SIGINT, sig_int) == SIG_ERR) {
    err_sys("signal(SIGINT) error");
  }

  if (signal(SIGQUIT, sig_int) == SIG_ERR) {
    err_sys("signal(SIGQUIT) error");
  }

  sigemptyset(&zeromask);
  sigemptyset(&newmask);
  sigaddset(&newmask, SIGQUIT);

  if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) {
    err_sys("SIG_BLOCK error");
  }

  while (quit_flag == 0) {
    sigsuspend(&zeromask);
  }

  quit_flag = 0;

  if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
    err_sys("SIG_SETMASK error");
  }

  exit(0);
}

static void sig_int(int signo)
{
  if (signo == SIGINT) {
    printf("\ninterrupt\n");
  } else if (signo == SIGQUIT) {
    quit_flag = 1;
  }
}
