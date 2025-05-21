#include "kernel/types.h"       //defines basic data types (like uint, ushort).
#include "kernel/stat.h"        //provides the stat structure to describe files (e.g., type, size).
#include "user/user.h"          //includes user-space functions like open(), read(), close(), printf(), etc.
#include "kernel/fs.h"          //defines file system constants and structures (like T_DIR, struct dirent).

// A helper function to search for a file inside a directory (recursively)
void find(const char *path, const char *filename)            //const char *path: a constant string pointer to the directory path you want to search.
{                                                            //const char *filename: the name of the file you're looking for.

                                                              //The const keyword means we are not allowed to modify the content of the strings path and filename inside the find function.



  char buf[512];      //buf[512]: a character array to hold file paths (max size 512).
  char *p;            //*p: a pointer used to build the full path.  //In C, strings are arrays of characters, and they are accessed via pointers.
  int fd;
  struct dirent de;     // de : is a directory entry structure.    //It's used to hold information about each file/folder inside a directory while you're looping over it.   //It has a name[] field, which gives you the name of the file or folder.
  struct stat st;       // st : is used to store information about a file.


        //stat is a structure that holds metadata about a file, like:

        //its type (is it a file or directory?), size, inode number, etc.









  // Open the path (it should be a directory)
  fd = open(path, 0);    //Opens the given path in read-only mode (0).
                          //Returns a file descriptor or -1 if it fails.
                         //int fd = open("myfile.txt", 0);   //"myfile.txt" is opened for reading (0 means read-only).   //open() returns something like fd = 3.  //So now, fd = 3 is your handle or ID for accessing "myfile.txt".
                              //You don’t hold the actual file,   //You just have a number that the system uses to know which file you're talking about.
  if (fd < 0) {
    fprintf(2, "find: cannot open %s\n", path);      //Unlike printf(), which always prints to stdout (screen),

                                                    //fprintf() lets you specify where the output goes.
    return;
  }

//| File Descriptor | Purpose         |
//| --------------- | --------------- |
//| `0`             | Standard Input  |
//| `1`             | Standard Output |
//| `2`             | Standard Error  |



//fd : It comes from open()   // It’s used in all file-related system calls (read, write, fstat, etc.)
//Once we open a file and get its fd, we can use that number to:

//Read from it: read(fd, buffer, size)

//Write to it: write(fd, buffer, size)

//Get its info: fstat(fd, &info)

//Close it: close(fd)

//Without fd, the kernel won’t know which file you're trying to access.





//struct stat st;

//This creates a variable st that stores information about a file, like:

//st.type → is it a file or directory?

//st.size → how big is it?

//st.dev → device number

//st.ino → inode number

//You fill it using the fstat() function.





//struct dirent de; This creates a variable de that represents one entry inside a directory.

// So you can access:

//de.name → the file or directory name

//de.inum → the inode number (internal ID of the file)




//struct stat st;	Create a variable st to hold file metadata
//struct dirent de;	Create a variable de to hold one directory entry info






//The inode number : is a unique ID that identifies a file on disk.
//fd: It refers to an open file, not the file itself on disk.


  // Get information about the path
  if (fstat(fd, &st) < 0) {                      //fstat(fd, &st): fills st with info about the opened file.   //	Fills st with info about the file from fd

    fprintf(2, "find: cannot stat %s\n", path);    //If it fails, print error, close the file, and return.
    close(fd);
    return;
  }

  // If not a directory, stop
  if (st.type != T_DIR) {
    fprintf(2, "find: %s is not a directory\n", path);
    close(fd);
    return;
  }

  // Copy the path into a buffer to build full paths to each file
  strcpy(buf, path);         //Copy the path to buf
  p = buf + strlen(buf);     //Move p to the end of the current path.
  *p++ = '/';                //Add '/' to prepare for adding file/folder names.

  // Read each file or folder inside the current directory
  while (read(fd, &de, sizeof(de)) == sizeof(de)) {
    if (de.inum == 0) continue;      //skip invalid entries.

    memmove(p, de.name, DIRSIZ);   //copy the file name to the buffer.
    p[DIRSIZ] = 0;    //add null terminator to make it a proper C string.

    if (stat(buf, &st) < 0) {    //get info about the full path.
      fprintf(2, "find: cannot stat %s\n", buf);
      continue;
    }

    if (st.type == T_FILE) {
      // If it's a file and name matches, print it
      if (strcmp(de.name, filename) == 0) {
        printf("%s\n", buf);
      }
    } else if (st.type == T_DIR) {
      // Skip . and .. to avoid infinite loops
      if (strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0) {
        find(buf, filename); // Recursive call to go into the subdirectory
      }
    }
  }

  close(fd);  //Always close the opened directory after finishing.
}

// Main function
int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(2, "Usage: find <path> <filename>\n");
    exit(0);
  }

  find(argv[1], argv[2]);
  exit(0);
}



//| Symbol | Meaning           | Used for...       |
//| ------ | ----------------- | ----------------- |
//| `.`    | Current directory | Stay in place     |
//| `..`   | Parent directory  | Go one level up   |
//| `/`    | Root directory    | Top of filesystem |
