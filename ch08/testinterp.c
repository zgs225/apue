#include <apue.h>
#include <sys/wait.h>

int main(void)
{
  pid_t pid;

  if ((pid = fork()) < 0)
    err_sys("fork error");
  else if (pid == 0) {
    if (execl("/Users/Yuez/Development/C/apue/ch08/testinterpre", "testinterpre",
              "Arg 1", "MY ARG 2", "HELA", (char *)0) < 0)
      err_sys("exec error");
  }

  if (waitpid(pid, NULL, 0) < 0)
    err_sys("waitpid error");

  exit(0);
}
