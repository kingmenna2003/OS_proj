#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

// Prints usage instructions when incorrect arguments are provided
void
usage() {
  fprintf(2, "Usage: mv <source> <destination>\n");
  exit(1);
}

// Prints detailed error messages for common system call failures
void
error(char *msg) {
  fprintf(2, "mv: %s\n", msg);
  exit(1);
}

// Main mv function
int
main(int argc, char *argv[]) {
  if (argc != 3) {
    usage();
  }

  char *src = argv[1];
  char *dst = argv[2];
  struct stat st_src, st_dst;

  // Check if the source file exists
  if (stat(src, &st_src) < 0) {
    error("source file does not exist");
  }

  // Check if the source and destination are the same
  if (strcmp(src, dst) == 0) {
    error("source and destination are the same");
  }

  // Check if the destination exists
  if (stat(dst, &st_dst) == 0) {
    // Handle if destination is a directory
    if (st_dst.type == T_DIR) {
      // Check if the destination is a directory and we are moving a file inside it
      if (st_src.type == T_FILE) {
        char new_dst[512];

        // Manually concatenate the source file's name to the destination directory
        memmove(new_dst, dst, strlen(dst) + 1);
        if (new_dst[strlen(new_dst) - 1] != '/') {
          new_dst[strlen(new_dst)] = '/';
          new_dst[strlen(new_dst) + 1] = 0;
        }
        memmove(new_dst + strlen(new_dst), src, strlen(src) + 1);
        dst = new_dst; // Set the full destination path
      } else {
        error("destination is a directory and source is not a file");
      }
    } else if (st_dst.type == T_FILE) {
      // If destination is a file, unlink it first
      if (unlink(dst) < 0) {
        error("cannot remove destination file");
      }
    }
  }

  // Create a hard link from source to destination
  if (link(src, dst) < 0) {
    error("link failed");
  }

  // Remove the original source file after linking
  if (unlink(src) < 0) {
    error("cannot unlink source file");
  }

  // If we reach here, the file has been successfully moved
  printf("Renamed/moved %s to %s\n", src, dst);

  exit(0);
}
