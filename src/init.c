#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/mount.h>
#include <stdio.h>

void clear_screen() {
  write(STDOUT_FILENO, "\033[2J\033[H", 7);
}

void fill_background(int tty) {
  for (int y = 0; y < 25; y++) {
    for (int x = 0; x < 80; x++) {
      write(tty, " ", 1);
    }
  }
  write(tty, "\033[H", 3);
}

int main() {
  int tty = open("/dev/tty0", O_WRONLY);
  if (tty < 0) {
    tty = 1;
  }

  mount("proc", "/proc", "proc", 0, "");

  clear_screen();
  write(tty, "\033[44;93m", 8);
  fill_background(tty);

  while (1) {
    pid_t pid = fork();
    if (pid == 0) {
      // Kindprozess: Shell starten
      char *argv[] = { "/shell", NULL };
      execv("/shell", argv);
      char msg2[] = "Shell couldn't start.\n";
      write(tty, msg2, strlen(msg2));
      _exit(1);
    } else if (pid > 0) {
      int status;
      waitpid(pid, &status, 0);

      // Falls Shell mit Exit-Code 0 beendet: System anhalten
      if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        char msg3[] = "Halting system...\n";
        write(tty, msg3, strlen(msg3));
        while (1) pause(); // System wird nicht beendet. CPU wartet auf ewig.
    }

    // Sonst einfach Shell neu starten
    } else {
      char msg4[] = "Error while forking.\n";
      write(tty, msg4, strlen(msg4));
    }
  }
}
