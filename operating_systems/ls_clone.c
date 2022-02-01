#include <dirent.h> // contains directory-related functions and the `dirent` struct
#include <sys/stat.h> // contains `stat` struct and `S_ISDIR` macro
#include <stdbool.h> // contains the definition for booleans
#include <stdio.h>
#include <unistd.h> // contains the `optind` variable
#include <string.h> // for `strlen` and `strcmp`

// Global Default Variables for `ls` flags
bool exclude_hidden = true;
bool recursive_ls = false;

int is_directory(const char *path) {
  struct stat stat_buffer;

  // if the path cannot be read, return False
  if (stat(path, &stat_buffer) != 0) {
    return 0;
  }

  // arrow `->` to access if the variable is a pointer
  // dot `.` to access if the variable is not a pointer
  // (*x).foo is equivalent to x->foo
  return S_ISDIR(stat_buffer.st_mode);
}

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

    int buffer_size = strlen(path) + strlen(entry->d_name) + 2;
    char filepath[buffer_size];
    snprintf(filepath, buffer_size, "%s/%s", path, entry->d_name);

    #define TERMINAL_CYAN  "\x1B[36m"
    #define TERMINAL_MAGENTA "\x1B[35m"
    #define TERMINAL_RESET "\x1B[0m"

    if (is_directory(filepath)) {
      printf("%s%s%s\n", TERMINAL_CYAN, filepath, TERMINAL_RESET);
    } else {
      printf("%s\n", filepath);
    }

    if (recursive_ls == true && is_directory(filepath) && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
      printf("\n%s%s:\n%s", TERMINAL_MAGENTA, filepath, TERMINAL_RESET);
      ls_files(filepath);
    }
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
  while((option = getopt(argc, argv, "aR")) != -1) {
    switch(option) {
      case 'a':
        printf("Flag `-%c` passed. Will list hidden files.\n", option);
        exclude_hidden = false;
        break;
      case 'R':
        printf("Given option %c. Will ls recursively.\n", option);
        recursive_ls = true;
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
  if (argc > 3) {
    printf("Usage: `./ls_clone -optional_flags optional_filepath`. Flags supported: aR\n");
    return 1;
  }
  set_ls_flags(argc, argv);

  // From the man page for `optind`:
  // The variable `optind` is the index of the next element to be
  //  processed in argv.  The system initializes this value to 1.  The
  //  caller can reset it to 1 to restart scanning of the same argv, or
  //  when scanning a new argument vector.
  if (argv[optind] == NULL) {
    // if no filepath given, list contents of current directory, just like in ls
    return ls_files(".");
  } else {
  return ls_files(argv[optind]);
  }
}
