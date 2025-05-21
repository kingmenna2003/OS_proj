#include "kernel/types.h"   //defines data types like `int`, `uint`, etc.
#include "kernel/stat.h"    //file status info (not needed here, but often included).
#include "user/user.h"      //gives you access to system calls like `sleep()`, `exit()`, `printf()`, etc.



int is_number(char *s)                    //char *s = a pointer to a character, which usually means the start of a string
{                                         //In C, a string is an array of characters that ends with a special null character '\0'
  for (int i = 0; s[i] != '\0'; i++)
  {
    if (s[i] < '0' || s[i] > '9')
    {
      return 0;     // if false
    }
  }
  return 1;
}



int main(int argc, char *argv[])  //This is the standard main function signature when a program needs to accept command-line arguments.
{

                                      //argc stands for argument count. It tells you how many arguments were passed to your program from the terminal.


  if(argc == 2 && strcmp(argv[1], "?") == 0)
  {                                                    // argc: "Argument Count" — the number of command-line arguments.
    printf("Usage: sleep <positive_number>\n");                   // argv: "Argument Vector" — an array of strings (char pointers) holding the actual arguments.
    exit(0);     //ends the program **successfully**
  }

  if(argc != 2)
  {
    printf("Invalid command, you must enter a single integer\n");
    exit(0);
  }

  int sleeptime = atoi(argv[1]);    //`atoi()` = "ASCII to Integer" — converts a **string to an int**.

//⏱ 1 tick ≈ 10ms in xv6 by default, so sleep(100) sleeps ~1 second.





  if (!is_number(argv[1]))
  {
    printf("Error: Sleep time must be a positive integer\n");
    exit(1);
  }


  printf("Starting sleep for %d\n", sleeptime);
  sleep(sleeptime);
  printf("Finished sleeping...\n");

  exit(0);
}
