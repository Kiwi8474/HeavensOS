#include <stdio.h>
#include <stdlib.h>
#include <sys/mount.h>
#include <string.h>
#include "../lib/fileutils.h"

int main(int argc, char *argv[]) {
  // Usage-Flag abfangen
  if (!usage_check(argc, argv, "umount <mountpoint>", "Unmounts a mountpoint."))
    return 0;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <mountpoint>\n", argv[0]);
    return 1;
  }

  if (umount(argv[1]) != 0) {
    perror("umount");
    return 1;
  }

  return 0;
}

