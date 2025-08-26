#include <stdio.h>
#include <string.h>
#include "../lib/fileutils.h"

int main(int argc, char *argv[]) {
  if (!usage_check(argc, argv, "cp <source> <destination>", "Copies a file or directory to the destination."))
    return 0;

  if (argc != 3) {
    fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]);
    return 1;
  }

  if (copy_recursive(argv[1], argv[2]) != 0) {
    perror("cp");
    return 1;
  }
  return 0;
}

