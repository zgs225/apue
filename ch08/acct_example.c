#include "apue.h"
#include <sys/acct.h>

int main(int argc, char *argv[])
{
  struct acct acdata;
  FILE *fp;

  if (argc != 2) {
    err_quit("usage: pracct filename");
  }

  if ((fp = fopen(argv[1], "r")) == NULL)
    err_sys("fopen() error");


}
