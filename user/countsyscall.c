#include "kernel/types.h"
#include "user/user.h"

int main()
{
  int countsys = countsyscall();
  printf("kernel access syscalls: %d\n", countsys);

  exit(0);
}
