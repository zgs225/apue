#include <apue.h>
#include <pwd.h>

struct passwd *my_getpwnam(const char *);

char *trim(char *);

int main(void)
{
  char *name;
  char buf[128];
  struct passwd *pwent;

  fputs("Please enter a user name: ", stdout);

  if ((name = fgets(buf, sizeof(buf), stdin)) == NULL)
    err_sys("fgets error");

  name = trim(name);

  if ((pwent = my_getpwnam(name)) == NULL)
    err_sys("my_getpwnam error");

  printf("Name: %s\n", pwent->pw_name);
  printf("Password:%s\n", pwent->pw_passwd);
  printf("User Id: %d\n", pwent->pw_uid);
}

struct passwd *my_getpwnam(const char *name)
{
  struct passwd *ptr;

  setpwent();
  while ((ptr = getpwent()) != NULL)
    if (strcmp(ptr->pw_name, name) == 0)
      break;
  endpwent();

  return ptr;
}

char *trim(char *s)
{
  int i = strlen(s) - 1;
  if ((i > 0) && (s[i] == '\n'))
    s[i] = '\0';
  return s;
}
