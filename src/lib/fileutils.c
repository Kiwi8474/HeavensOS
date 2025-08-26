#include "fileutils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

int remove_recursive(const char *path) {
    struct stat st;
    if (lstat(path, &st) != 0) return -1;

    if (S_ISDIR(st.st_mode)) {
        DIR *dir = opendir(path);
        if (!dir) return -1;

        struct dirent *entry;
        char buf[1024];
        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            snprintf(buf, sizeof(buf), "%s/%s", path, entry->d_name);
            if (remove_recursive(buf) != 0) {
                closedir(dir);
                return -1;
            }
        }
        closedir(dir);
        return rmdir(path);
    } else {
        return unlink(path);
    }
}

int copy_recursive(const char *src, const char *dest) {
    struct stat st;
    if (stat(src, &st) != 0) return -1;

    if (S_ISDIR(st.st_mode)) {
        mkdir(dest, st.st_mode);
        DIR *dir = opendir(src);
        if (!dir) return -1;

        struct dirent *entry;
        char src_path[1024], dest_path[1024];
        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            snprintf(src_path, sizeof(src_path), "%s/%s", src, entry->d_name);
            snprintf(dest_path, sizeof(dest_path), "%s/%s", dest, entry->d_name);
            if (copy_recursive(src_path, dest_path) != 0) {
                closedir(dir);
                return -1;
            }
        }
        closedir(dir);
    } else {
        FILE *src_f = fopen(src, "rb");
        FILE *dest_f = fopen(dest, "wb");
        if (!src_f || !dest_f) return -1;

        char buf[4096];
        size_t bytes;
        while ((bytes = fread(buf, 1, sizeof(buf), src_f)) > 0) {
            if (fwrite(buf, 1, bytes, dest_f) != bytes) {
                fclose(src_f);
                fclose(dest_f);
                return -1;
            }
        }
        fclose(src_f);
        fclose(dest_f);
    }
    return 0;
}

int move_recursive(const char *src, const char *dest) {
    if (rename(src, dest) == 0) return 0;
    if (copy_recursive(src, dest) != 0) return -1;
    return remove_recursive(src);
}

int mkdir_recursive(const char *path, mode_t mode) {
    char tmp[1024];
    snprintf(tmp, sizeof(tmp), "%s", path);
    size_t len = strlen(tmp);
    if (tmp[len - 1] == '/') tmp[len - 1] = '\0';

    for (char *p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            mkdir(tmp, mode);
            *p = '/';
        }
    }
    return mkdir(tmp, mode);
}

bool usage_check(int argc, char *argv[], const char *usage, const char *desc) {
  if (argc > 1 && strcmp(argv[1], "--help") == 0) {
    printf("Usage: %s\n%s\n", usage, desc);
    return false;
  }
  return true;
}

