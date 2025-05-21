#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main(int argc, char *argv[]) {
    // Check for help argument
    if (argc == 2 && strcmp(argv[1], "?") == 0) {
        printf("Usage: touch filename\n");
        exit(0);
    }

    // If not exactly one argument is given, print usage and exit
    if (argc != 2) {
        printf("Usage: touch filename\n");
        exit(0);
    }

    // Try to open the file in read-only mode to check if it already exists
    int fd = open(argv[1], O_RDONLY);

    if (fd >= 0) {
        // The file exists, close the file descriptor and print the error message
        close(fd);
        printf("touch: %s already exists\n", argv[1]);
        exit(0);
    }

    // If the file doesn't exist, try creating it with O_CREATE | O_RDWR
    fd = open(argv[1], O_CREATE | O_RDWR);

    if (fd < 0) {
        // If the file can't be created, print an error
        printf("touch: failed to create file %s\n", argv[1]);
        exit(1);
    }

    // Successfully created the file (if it didn't exist)
    printf("%s created\n", argv[1]);

    // Close the file descriptor
    close(fd);
    exit(0);
}
