#include <stdio.h>
#include <string.h>
#include "../lib/fileutils.h"

void clear_screen() {
  printf("\033[2;1H"); // Cursor auf Zeile 2, Spalte 1
  printf("\033[J"); // Alles ab hier löschen
}

void fill_background() {
  for (int y = 0; y < 25; y++) {
    for (int x = 0; x < 80; x++) {
      printf(" ");
    }
  }
  printf("\033[2;1H"); // Cursor auf Zeile 2, Spalte 1
}

int main(int argc, char *argv[]) {
  // Usage-Flag abfangen
  if (!usage_check(argc, argv, "clear", "Clears the Terminal"))
    return 0;

  clear_screen();
  printf("\033[44;93m");
  fill_background();
}
