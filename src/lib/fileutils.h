#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <sys/stat.h>
#include <stdbool.h>

int remove_recursive(const char *path);
int copy_recursive(const char *src, const char *dest);
int move_recursive(const char *src, const char *dest);
int mkdir_recursive(const char *path, mode_t mode);

bool usage_check(int argc, char *argv[], const char *usage, const char *desc);

#endif

