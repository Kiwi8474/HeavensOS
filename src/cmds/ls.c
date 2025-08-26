#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include "../lib/fileutils.h"

int main(int argc, char *argv[]) {
  // Usage-Flag abfangen
  if (!usage_check(argc, argv, "ls | ls . | ls .. | ls <destination>", "Shows the children of the destination."))
    return 0;

  const char *path = argc > 1 ? argv[1] : ".";
  DIR *dir = opendir(path);
  if (!dir) {
    perror("opendir");
    return 1;
  }

  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    printf("%s\n", entry->d_name);
  }

  closedir(dir);
  return 0;
}
