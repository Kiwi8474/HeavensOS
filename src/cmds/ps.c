#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include "../lib/fileutils.h"

int is_number(const char *str) {
  for (int i = 0; str[i]; i++) {
    if (!isdigit((unsigned char)str[i])) return 0;
  }
  return 1;
}

int main(int argc, char *argv[]) {
  // Usage-Flag abfangen
  if (!usage_check(argc, argv, "ps", "Displays every active process."))
    return 0;

  DIR *proc = opendir("/proc");
  if (!proc) {
    perror("opendir /proc");
    return 1;
  }

  printf("PID\tNAME\n");

  struct dirent *entry;
  while ((entry = readdir(proc)) != NULL) {
    if (is_number(entry->d_name)) {
      char path[256];
      snprintf(path, sizeof(path), "/proc/%s/comm", entry->d_name);

      FILE *f = fopen(path, "r");
      if (f) {
        char name[256];
        if (fgets(name, sizeof(name), f)) {
          // Entfernt das \n am Ende
          name[strcspn(name, "\n")] = '\0';
          printf("%s\t%s\n", entry->d_name, name);
        }
        fclose(f);
      }
    }
  }
  closedir(proc);
  return 0;
}
