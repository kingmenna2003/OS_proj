#include "kernel/types.h"
#include "user/user.h"

// Convert string to integer
int str_to_int(const char *s) {
  int num = 0;
  for (int i = 0; s[i] != '\0'; i++) {
    if (s[i] < '0' || s[i] > '9') return -1; // Invalid input
    num = num * 10 + (s[i] - '0');
  }
  return num;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(2, "Usage: fact <non-negative integer>\n");
    exit(1);
  }

  if (argc > 2) {
    fprintf(2, "Only one number required for fact\n");
    exit(1);
  }

  int n = str_to_int(argv[1]);

  if (n < 0) {
    fprintf(2, "fact: Please provide a non-negative integer\n");
    exit(1);
  }

  uint result = 1;
  for (int i = 2; i <= n; i++) {
    result *= i;
  }

  printf("Factorial of %d is %d\n", n, result);
  exit(0);
}
