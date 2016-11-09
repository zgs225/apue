#include <apue.h>
#include <signal.h>
#include <errno.h>

void pr_mask(const char *str)
{
  sigset_t sigset;
  int      errno_save;

  errno_save = errno;

  if (sigprocmask(0, NULL, &sigset) < 0) {
    err_ret("sigprocmask error");
  } else {
    printf("%s", str);

    if (sigismember(&sigset, SIGINT)) {
      printf(" SIGINT");
    } else if (sigismember(&sigset, SIGQUIT)) {
      printf(" SIGQUIT");
    } else if (sigismember(&sigset, SIGUSR1)) {
      printf(" SIGUSR1");
    } else if (sigismember(&sigset, SIGALRM)) {
      printf(" SIGALRM");
    }

    printf("\n");
  }

  errno = errno_save;
}

int main(void)
{
  pr_mask("Mask:");
  exit(0);
}
