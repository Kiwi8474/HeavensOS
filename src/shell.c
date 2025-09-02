#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>

void print_watermark(int tty, int first_time) {
  const char *text = "                          HeavensOS (C) 2025 - Kiwi8474                         ";
  int len = strlen(text);
  int col = 0;
  char buf[128];
  snprintf(buf, sizeof(buf),
          "\033[s"
          "\033[1;93;44m\033[1;%dH%s"
          "\033[u\033[93;44m",
          col + 1, text);
  write(tty, buf, strlen(buf));

  if (first_time) {
    write(tty, "\033[2;1H", 6);
  }
}

int main() {
    int tty = open("/dev/tty0", O_WRONLY);
    if (tty < 0) {
        tty = 1;
    }

    char input[128];
    srand(time(NULL));

    int first_time = 1;

    while (1) { // Shell-Loop
        // Wasserzeichen/Header und Prompt
        print_watermark(tty, first_time);
        first_time = 0;
        char prompt[] = "> ";
        write(tty, prompt, strlen(prompt));

        ssize_t len = read(0, input, sizeof(input) - 1);
        if (len <= 0) continue;
        input[len - 1] = '\0'; // Newline entfernen

        // Eingabe in Argumente zerlegen
        char *args[16];
        int argc = 0;
        char *token = strtok(input, " ");
        while (token && argc < 15) {
            args[argc++] = token;
            token = strtok(NULL, " ");
        }
        args[argc] = NULL;

        if (args[0] == NULL) continue;

        // Eingebaute Befehle
        if (strcmp(args[0], "exit") == 0) {
            if (args[1] && strcmp(args[1], "--help") == 0) {
                printf("Usage: exit\nExits the HeavensOS shell.\n");
                continue;
            }
            _exit(0);
        }

        if (strcmp(args[0], "cd") == 0) {
            if (args[1] && strcmp(args[1], "--help") == 0) {
                printf("Usage: cd <directory>\nChanges the current working directory.\n");
                continue;
            }

            if (args[1] == NULL) { // Kein Ziel angegeben. Wechsel ins Root
                    if (chdir("/") != 0) {
                        perror("cd");
                    }

            } else {
                    if (chdir(args[1]) != 0) { // Sonst zum Ziel wechseln.
                    perror("cd");
                    }
            }
            continue;
        }

        // Externe Befehle
        pid_t pid = fork();
        if (pid == 0) {
            // Pfad zum Befehl vorbereiten
            char path[256];
            snprintf(path, sizeof(path), "/bin/%s", args[0]);

            execv(path, args); // direkt args vom Elternprozess nutzen
            char msg3[] = "Unknown command.\n";
            write(tty, msg3, strlen(msg3));
            _exit(1);
        } else if (pid > 0) {
            int status;
            waitpid(pid, &status, 0);
        } else {
            char msg4[] = "Error while forking.\n";
            write(tty, msg4, strlen(msg4));
        }
    }
    return 0;
}