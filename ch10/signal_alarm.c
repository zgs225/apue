#include <apue.h>
#include <pwd.h>

static void my_alarm(int signo)
{
  struct passwd *rootptr;
  printf("In signal handler");
  if ((rootptr = getpwnam("root")) == NULL)
    err_sys("getpwnam(root) error");
  alarm(1);
}

int main(void)
{
  struct passwd *ptr;
  signal(SIGALRM, my_alarm);
  alarm(1);

  for (;;) {
    if ((ptr = getpwnam("Yuez")) == NULL) {
      err_sys("getpwnam(Yuez) error");
    }

    if (strcmp(ptr->pw_name, "Yuez") != 0) {
      printf("return value corrupted! pw_name = %s\n", ptr->pw_name);
    } else {
      printf("return value: pw_name = %s\n", ptr->pw_name);
    }
  }

  return 0;
}
