#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include "../lib/fileutils.h"

#define BIN_DIR "/bin"

int main(int argc, char *argv[]) {
  // Usage-Flag abfangen
  if (!usage_check(argc, argv, "help", "Displays every command."))
    return 0;

  struct dirent *entry;
  DIR *dir = opendir(BIN_DIR);

  printf("exit\ncd\n");

  if (dir == NULL) {
    perror("Unable to open /bin");
    return 1; // Fehler
  }

  while ((entry = readdir(dir)) != NULL) {
    if (entry->d_name[0] != '.') { // Versteckte Dateien nicht anzeigen
      printf("%s\n", entry->d_name);
    }
  }

  printf("\nUse --help flag after any command to see its usage.\n");

  closedir(dir);
  return 0;
}
