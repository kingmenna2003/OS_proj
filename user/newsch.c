// #include "kernel/types.h"
// #include "kernel/stat.h"
// #include "user/user.h"
// #include "kernel/fcntl.h"


// struct avg_t {
//   int wait;
//   int turn;
//   int run;
// };



// int main(int argc, char *argv[]) {

//   int pid;
//   int k, nprocess = 10;
//   int z, steps = 1000000;
//   char buffer_src[1024], buffer_dst[1024];


//   for (k = 0; k < nprocess; k++) {
//     // ensure different creation times (proc->ctime)
//     // needed for properly testing FCFS scheduling
//     sleep(2);

//     pid = fork();
//     if (pid < 0) {
//       printf("%d failed in fork!\n", getpid());
//       exit(0);

//     }
//     else if (pid == 0) {
//       // child
//       printf("[pid=%d] created\n", getpid());

//       for (z = 0; z < steps; z += 1) {
//          // copy buffers one inside the other and back
//          // used for wasting cpu time
//          memmove(buffer_dst, buffer_src, 1024);
//          memmove(buffer_src, buffer_dst, 1024);
//       }
//       exit(0);
//     }
//   }

//   for (k = 0; k < nprocess; k++) {
//     pid = wait(0);
//     printf("[pid=%d] terminated\n", pid);
//   }


//   struct avg_t a;
//   if(getavgt(&a) < 0){
//       printf("getavgt failed\n");
//       exit(1);
//   }

//   printf("Average waiting time    : %d ticks\n", a.wait);
//   printf("Average turnaround time : %d ticks\n", a.turn);
//   printf("Average run time        : %d ticks\n", a.run);
//   exit(0);

// }





#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"


struct avg_t {
  int wait;
  int turn;
  int run;
};

int main(int argc, char *argv[]) {
  if(argc < 2) {
    printf("Usage: newsch [rr|fcfs|pri]\n");
    exit(1);
  }

  // Set scheduler mode
  if(strcmp(argv[1], "rr") == 0) {
    setsched(0); // Round Robin
  }
  else if(strcmp(argv[1], "fcfs") == 0) {
    setsched(1); // FCFS
  }
  else if(strcmp(argv[1], "pri") == 0) {
    setsched(2); // Priority
  }
  else {
    printf("Invalid scheduler: use rr, fcfs, or pri\n");
    exit(1);
  }

  int pid;
  int k, nprocess = 10;
  int z, steps = 1000000;
  char buffer_src[1024], buffer_dst[1024];

  for (k = 0; k < nprocess; k++) {
    sleep(2); // Ensure different creation times

    pid = fork();
    if (pid < 0) {
      printf("%d failed in fork!\n", getpid());
      exit(0);
    }
    else if (pid == 0) {
      // Child process
      if(strcmp(argv[1], "pri") == 0) {
        // For priority scheduling, set different priorities
        // Alternating between high(7), medium(5), low(3)
        setpriority(7 - (k % 3) * 2);
      }

      printf("[pid=%d] created\n", getpid());

      for (z = 0; z < steps; z += 1) {
        memmove(buffer_dst, buffer_src, 1024);
        memmove(buffer_src, buffer_dst, 1024);
      }
      exit(0);
    }
  }

  for (k = 0; k < nprocess; k++) {
    pid = wait(0);
    printf("[pid=%d] terminated\n", pid);
  }

  struct avg_t a;
  if(getavgt(&a) < 0) {
    printf("getavgt failed\n");
    exit(1);
  }

  printf("Average waiting time    : %d ticks\n", a.wait);
  printf("Average turnaround time : %d ticks\n", a.turn);
  printf("Average run time        : %d ticks\n", a.run);
  exit(0);
}
