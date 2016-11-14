#include <apue.h>

static void sig_int(int signo);

int main(void)
{
  sigset_t new_mask, old_mask, wait_mask;

  pr_mask("program start:");

  if (signal(SIGINT, sig_int) == SIG_ERR) {
    err_sys("signal(SIGINT) error");
  }

  sigemptyset(&new_mask);
  sigaddset(&new_mask, SIGINT);
  sigemptyset(&wait_mask);
  sigaddset(&wait_mask, SIGUSR1);

  if (sigprocmask(SIG_BLOCK, &new_mask, &old_mask) != 0) {
    err_sys("SIG_BLOCK error");
  }

  pr_mask("in critical region:");

  if (sigsuspend(&wait_mask) != -1) {
    err_sys("sigsuspend error");
  }

  pr_mask("after return from suspend:");

  if (sigprocmask(SIG_SETMASK, &old_mask, NULL) != 0) {
    err_sys("SIG_SETMASK error");
  }

  pr_mask("program exit:");
  exit(0);
}

static void sig_int(int signo)
{
  pr_mask("\nin sigint:");
}
