#include <apue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>

int main(int argc, char *argv[])
{
  int             i, fd;
  struct stat     statbuf;
  struct timeval  times[2];

  for (i = 1; i < argc; i ++) {
    if (stat(argv[i], &statbuf) < 0) {
      err_ret("%s, stat error.", argv[i]);
      continue;
    }

    if ((fd = open(argv[i], O_RDWR | O_TRUNC)) < 0) {
      err_ret("%s, open error.", argv[i]);
      continue;
    }

    times[0].tv_sec = statbuf.st_atime;
    times[1].tv_sec = statbuf.st_mtime;
  }
}
