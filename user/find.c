/*#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int found = 0;  // Track if a match was found

void my_strcat(char *dest, const char *src) {
    while (*dest) dest++;
    while ((*dest++ = *src++));
}

void find_in_dir(char *dir, char *filename) {
    char buf[512];
    struct dirent de;
    struct stat st;
    int fd;

    if ((fd = open(dir, 0)) < 0) {
        printf("find: cannot open %s\n", dir);
        return;
    }

    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0) continue;

        strcpy(buf, dir);
        my_strcat(buf, "/");
        my_strcat(buf, de.name);

        if (stat(buf, &st) < 0) {
            printf("find: cannot stat %s\n", buf);
            continue;
        }

        if (st.type == T_DIR) {
            if (strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0) {
                find_in_dir(buf, filename);
            }
        } else if (st.type == T_FILE) {
            if (strcmp(de.name, filename) == 0) {
                printf("%s\n", buf);
                found = 1;
            }
        }
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc == 2 && strcmp(argv[1], "?") == 0) {
        printf("Usage: find <directory> <filename>\n");
        exit(0);
    }

    if (argc != 3) {
        printf("find: you need to provide a directory and a filename\n");
        exit(0);
    }

    find_in_dir(argv[1], argv[2]);

    if (!found) {
        printf("%s not found\n", argv[2]);
    }

    exit(0);
}
*/



#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

#define MAX_PATH_LEN 512

int found = 0; // global variable to track if we found the file

void
find(const char *path, const char *target) {
  char buf[MAX_PATH_LEN], *p;
  int fd;
  struct dirent de;
  struct stat st;

  // open the directory
  fd = open(path, 0);
  if (fd < 0)
  {
    fprintf(2, "find: cannot open %s\n", path);
    exit(0);
  }

  // get stats for the path
  if (fstat(fd, &st) < 0)
  {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    exit(0);
  }

  // if it's not a directory, just return
  if (st.type != T_DIR)
  {
    close(fd);
    exit(0);
  }

  // prepare the buffer to build full paths
  strcpy(buf, path);
  p = buf + strlen(buf);
  *p++ = '/';

  // read directory entries
  while (read(fd, &de, sizeof(de)) == sizeof(de)) {
    if (de.inum == 0)
      continue;

    // skip "." and ".."
    if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
      continue;

    // append the name to path
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;

    // get stats for the new path
    if (stat(buf, &st) < 0) {
      fprintf(2, "find: cannot stat %s\n", buf);
      continue;
    }

    // check if this is the target file
    if (st.type == T_FILE && strcmp(de.name, target) == 0) {
      found = 1;
      printf("Found: %s\n", buf);
    }

    // if it's a directory, search inside it recursively
    if (st.type == T_DIR) {
      find(buf, target);
    }
  }

  close(fd);
}

int
main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(2, "find: you need to provide a directory and a filename\n");
    exit(0);
  }

  if (argc == 2 && strcmp(argv[1], "?") == 0) {
    fprintf(2, "Usage: find <directory> <filename>\n");
    exit(0);
  }


  if (!found) {
    fprintf(2, "find: file '%s' not found in '%s'\n", argv[2], argv[1]);
    exit(0);
  }

  find(argv[1], argv[2]);
  exit(0);
}
