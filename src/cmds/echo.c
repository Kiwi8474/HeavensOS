#include <stdio.h>
#include <string.h>
#include "../lib/fileutils.h"

int main(int argc, char *argv[]) {
  // Usage-Flag abfangen
  if (!usage_check(argc, argv, "echo <text>", "Writes any text into the Terminal."))
    return 0;

  for (int i = 1; i < argc; i++) {
    printf("%s", argv[i]);
    if (i < argc - 1) printf(" ");
  }

  printf("\n");
  return 0;
}
