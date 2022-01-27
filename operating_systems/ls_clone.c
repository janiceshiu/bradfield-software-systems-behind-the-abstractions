#include <dirent.h> // contains directory-related functions and the `dirent` struct
#include <stdbool.h> // contains the definition for booleans
#include <stdio.h>
#include <unistd.h> // contains the `optind` variable

// Global Variables for `ls` flags
bool exclude_hidden = true;

int ls_files(const char *path) {
  DIR *directory_ptr = opendir(path);

  if (directory_ptr == NULL) {
    printf("Could not open directory %s\n", path);
    return 1;
  }

  while (1) {
    struct dirent *entry;

    entry = readdir(directory_ptr);
    if (entry == NULL) {
      break;
    }

    // hidden files start with `.`
    if (exclude_hidden && entry->d_name[0] == '.') {
      continue;
    }

    printf("%s\n", entry->d_name);
  }

  // We get a segmentation fault if we try to close a directory that doesn't exist.
  // This is because our program is trying to close a null pointer
  closedir(directory_ptr);

  return 0;
}

void set_ls_flags(int argc, char *argv[]) {
  opterr = 0;
  int option;

  // getopt returns -1 when there are no more options to process
  while((option = getopt(argc, argv, "a")) != -1) {
    switch(option) {
      case 'a':
        printf("Flag `-%c` passed. Will list hidden files.\n", option);
        exclude_hidden = false;
        break;
      case '?': // getopt returns '?' when there is an option without a case
        // By default, getopt() prints an error message on standard
        // error, places the erroneous option character in optopt, and
        // returns '?' as the function result.
        printf("Flag `-%c` passed. Unknown flag.\n", optopt);
    }
  }

  // Separates flag info from the actual `ls` printout
  printf("\n");
}

int main(int argc, char *argv[]) {
  set_ls_flags(argc, argv);

  // From the man page for `optind`:
  // The variable `optind` is the index of the next element to be
  //  processed in argv.  The system initializes this value to 1.  The
  //  caller can reset it to 1 to restart scanning of the same argv, or
  //  when scanning a new argument vector.
  return ls_files(argv[optind]);
}
