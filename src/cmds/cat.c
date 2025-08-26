#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "../lib/fileutils.h"

int main(int argc, char *argv[]) {
  if (!usage_check(argc, argv, "cat <file>", "Displays any file content."))
    return 0;

  int fd = open(argv[1], O_RDONLY); // Datei öffnen
  if (fd < 0) { // Fehler
    perror("cat");
    return 1;
  }

  char buf[256];
  ssize_t bytes;
  while ((bytes = read(fd, buf, sizeof(buf))) > 0) {
    write(1, buf, bytes); // 1 = stdout
  }
  close(fd); // Datei schließen
  return 0;
}

