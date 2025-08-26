#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "../lib/fileutils.h"

int main(int argc, char *argv[]) {
  if (!usage_check(argc, argv, "mkdir <directory>", "Creates a directory."))
    return 0;

  if (argc < 2) {
    fprintf(stderr, "Usage: %s <path>\n", argv[0]);
    return 1;
  }

  if (mkdir_recursive(argv[1], 0777) != 0) {
    perror("mkdir");
    return 1;
  }

  return 0;
}

