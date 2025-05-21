#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  if(argc != 2) {
    printf("Usage: sched [fcfs|prio|rr]\n");
    exit(1);
  }

  if(strcmp(argv[1], "fcfs") == 0) {
    sched_fcfs();
  } else if(strcmp(argv[1], "prio") == 0) {
    sched_priority();
  } else if(strcmp(argv[1], "rr") == 0) {
    // Default Round Robin
  } else {
    printf("Invalid policy\n");
    exit(1);
  }
  exit(0);
}
