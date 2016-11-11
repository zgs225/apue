#include <apue.h>
#include <time.h>
#include <setjmp.h>
#include <errno.h>

static void sig_usr1(int signo);
static void sig_alrm(int signo);

void pr_mask(const char *str);

static sigjmp_buf env;
static volatile sig_atomic_t canjump;

int main(void)
{
  if (signal(SIGUSR1, sig_usr1) < 0) {
    err_sys("signal SIGUSR1 error");
  }

  if (signal(SIGALRM, sig_alrm) < 0) {
    err_sys("signal SIGALRM error");
  }

  pr_mask("starting main: ");

  if (sigsetjmp(env, 1)) {
    pr_mask("ending main: ");
    exit(0);
  }

  canjump = 1;

  for (;;)
    pause();
}

static void sig_usr1(int signo)
{
  time_t starttime;

  if (canjump == 0)
    return;
  pr_mask("starting sig_usr1: ");
  alarm(3);
  starttime = time(NULL);
  for (;;)
    if (time(NULL) > starttime + 5)
      break;
  pr_mask("finishing sig_usr1: ");
  canjump = 0;
  siglongjmp(env, 1);
}

static void sig_alrm(int signo)
{
  pr_mask("in alarm: ");
}


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
    }

    if (sigismember(&sigset, SIGQUIT)) {
      printf(" SIGQUIT");
    }
    if (sigismember(&sigset, SIGUSR1)) {
      printf(" SIGUSR1");
    }

    if (sigismember(&sigset, SIGALRM)) {
      printf(" SIGALRM");
    }

    printf("\n");
  }

  errno = errno_save;
}
