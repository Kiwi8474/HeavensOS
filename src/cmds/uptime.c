#include <stdio.h>
#include <stdlib.h>
#include "../lib/fileutils.h"

int main(int argc, char *argv[]) {
  if (!usage_check(argc, argv, "uptime", "Displays the time that has passed since system boot."))
    return 0;

  FILE *f = fopen("/proc/uptime", "r");
  if (!f) {
    perror("uptime");
    return 1;
  }

  long seconds;
  if (fscanf(f, "%ld", &seconds) != 1) {
    fprintf(stderr, "uptime: Failed to read uptime\n");
    fclose(f);
    return 1;
  }
  fclose(f);

  long hours = seconds / 3600;
  long minutes = (seconds % 3600) / 60;
  long secs = seconds % 60;

  printf("Uptime: %02ld:%02ld:%02ld\n", hours, minutes, secs);
  return 0;
}

