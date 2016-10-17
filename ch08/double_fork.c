#include <apue.h>

int main(void)
{
  pid_t pid;

  if ((pid = fork()) < 0)
    err_sys("fork error");
  else if (pid == 0) {
    if ((pid = fork()) < 0)
      err_sys("fork error");
    else if (pid > 0)
      exit(0);

    sleep(2);
    printf("second child process, parent pid = %ld, pid = %ld\n", (long)getppid(), (long)getpid());
    exit(0);
  }

  if (waitpid(pid, NULL, 0) < 0)
    err_sys("waitpid error");
  exit(0);
}
