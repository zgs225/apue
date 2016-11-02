#include <signal.h>
#include <apue.h>

static void sig_usr(int signo);

int main(void)
{
  if (signal(SIGUSR1, sig_usr) == SIG_ERR) {
    err_sys("can't catch SIGUSR1");
  }

  if (signal(SIGUSR2, sig_usr) == SIG_ERR) {
    err_sys("can't catch SIGUSR2");
  }

  for (;;) {
    pause();
  }
}

static void sig_usr(int signo)
{
  if (signo == SIGUSR1) {
    printf("recieved SIGUSR1\n");
  } else if (signo == SIGUSR2) {
    printf("recieved SIGUSR2\n");
  } else {
    err_dump("recieved signal %d\n", signo);
  }
}
