#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/fileutils.h"

int main(int argc, char *argv[]) {
  if (!usage_check(argc, argv, "free", "Displays memory usage in kB."))
    return 0;

  FILE *f = fopen("/proc/meminfo", "r");
  if (!f) {
    perror("free");
    return 1;
  }

  long mem_total = -1, mem_free = -1;

  char key[64];
  long value;
  char unit[16];

  while (fscanf(f, "%63s %ld %15s", key, &value, unit) == 3) {
    if (strcmp(key, "MemTotal:") == 0) {
      mem_total = value;
    } else if (strcmp(key, "MemFree:") == 0) {
      mem_free = value;
    }
  }
  fclose(f);

  if (mem_total < 0 || mem_free < 0) {
    fprintf(stderr, "free: could not parse /proc/meminfo\n");
    return 1;
  }

  long mem_used = mem_total - mem_free;

  printf("       total       used       free\n");
  printf("Mem: %8ld %8ld %8ld kB\n", mem_total, mem_used, mem_free);

  return 0;
}

