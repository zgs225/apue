#include <apue.h>

int main(int argc, char *argv[])
{
  int i;
  struct stat buf;
  char *ptr;

  for (i = 0; i < argc; i ++) {
    if (lstat(argv[i], &buf) < 0) {
      err_ret("lstat error");
      continue;
    }

    if (S_ISREG(buf.st_mode))
      ptr = "regular file";
    else if (S_ISDIR(buf.st_mode))
      ptr = "directory";
    else if (S_ISCHR(buf.st_mode))
      ptr = "character special file";
    else if (S_ISBLK(buf.st_mode))
      ptr = "block special file";
    else if (S_ISFIFO(buf.st_mode))
      ptr = "FIFO";
    else if (S_ISSOCK(buf.st_mode))
      ptr = "socket";
    else if (S_ISLNK(buf.st_mode))
      ptr = "link";
    else
      ptr = "unknown file type";

    printf("%s\n", ptr);
  }

  return 0;
}
