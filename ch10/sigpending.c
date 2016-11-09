#include <apue.h>
#include <signal.h>

static void sig_quit(int signo);

int main(void)
{
  sigset_t new_mask, old_mask, pending_mask;

  if (signal(SIGQUIT, sig_quit) == SIG_ERR) {
    err_sys("signal error");
  }

  sigemptyset(&new_mask);
  sigaddset(&new_mask, SIGQUIT);

  if (sigprocmask(SIG_BLOCK, &new_mask, &old_mask) < 0) {
    err_sys("SIG_BLOCK error");
  }

  sleep(5);

  if (sigpending(&pending_mask) < 0) {
    err_sys("sigpending error");
  }

  if (sigismember(&pending_mask, SIGQUIT)) {
    printf("\nSIGQUIT pending\n");
  }

  if (sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0) {
    err_sys("SIG_SETMASK error");
  }

  printf("SIGQUIT unblocked");

  sleep(5);
  exit(0);
}

static void sig_quit(int signo)
{
  printf("caught SIGQUIT\n");
  if (signal(SIGQUIT, SIG_DFL) < 0) {
    err_sys("can't reset SIGQUIT");
  }
}
