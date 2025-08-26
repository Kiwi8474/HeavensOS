#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include "../lib/fileutils.h"

int main(int argc, char *argv[]) {
  if (!usage_check(argc, argv, "kill <PID>", "Sends SIGTERM to the process with the given PID."))
    return 0;

  int pid = atoi(argv[1]);
  if (pid <= 0) {
    fprintf(stderr, "kill: invalid PID '%s'\n", argv[1]);
    return 1;
  }

  if (kill(pid, SIGTERM) == -1) {
    perror("kill");
    return 1;
  }

  printf("Process %d terminated.\n", pid);
  return 0;
}

