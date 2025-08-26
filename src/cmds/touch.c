#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "../lib/fileutils.h"

int main(int argc, char *argv[]) {
  // Usage-Flag abfangen
  if (!usage_check(argc, argv, "touch <file name>", "Creates an empty file."))
    return 0;

  int fd = open(argv[1], O_WRONLY | O_CREAT, 0644); // Datei öffnen
  if (fd < 0) {
    perror("open");
    return 1; // irgendein Fehler. Keine Ahnung
  }

  close(fd); // Datei schließen
  return 0;
}
