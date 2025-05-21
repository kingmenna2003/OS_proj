#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

char buf[512];

void
error(char *msg) {
  fprintf(2, "cp: %s\n", msg);
  exit(1);
}

int
main(int argc, char *argv[]) {
  int fd_src, fd_dst, n;

  if (argc != 3) {
    fprintf(2, "Usage: cp <source> <destination>\n");
    exit(1);
  }

  fd_src = open(argv[1], O_RDONLY);
  if (fd_src < 0)
    error("cannot open source file");

  fd_dst = open(argv[2], O_CREATE | O_WRONLY);
  if (fd_dst < 0)
    error("cannot open destination file");

  while ((n = read(fd_src, buf, sizeof(buf))) > 0) {
    if (write(fd_dst, buf, n) != n)
      error("write failed");
  }

  if (n < 0)
    error("read failed");

  close(fd_src);
  close(fd_dst);

  exit(0);
}
