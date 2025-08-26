#include <stdio.h>
#include <string.h>
#include "../lib/fileutils.h"

int main(int argc, char *argv[]) {
  if (!usage_check(argc, argv, "rm <destination>", "Removes a destination."))
    return 0;

  if (argc < 2) {
    fprintf(stderr, "Usage: %s <path>\n", argv[0]);
    return 1;
  }

  if (remove_recursive(argv[1]) != 0) {
    perror("rm");
    return 1;
  }

  return 0;
}

