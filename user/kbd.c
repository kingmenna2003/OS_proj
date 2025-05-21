#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc == 2 && strcmp(argv[1], "?") == 0) {
    printf("Usage: kbd\n");
    exit(0);
  }
  if(argc != 1) {
    printf("Usage: kbd\n");
    exit(1);
  }
  int kbd_int_count = kbdint();
  printf("Number of keyboard interrupts: %d\n", kbd_int_count);

  exit(0);
}
