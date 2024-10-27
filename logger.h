#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

#define STRERROR(ERRNO) ": %s", strerror(errno)

enum LogLevel {
    DEBUG   = 0,
    INFO    = 1,
    WARNING = 2,
    ERROR   = 3
};

typedef struct {
    FILE* file_out;
    enum LogLevel min_level;
} logger_t;

void Log(enum LogLevel status, const char* file, size_t line, const char* func, const char *fmt, ...);

void LoggerSetFile(FILE* out);

void LoggerSetLevel(enum LogLevel level);

#define LOG(status, ...)                                        \
    do {                                                        \
        Log(status, __FILE__, __LINE__, __func__, __VA_ARGS__); \
    } while(0)

#endif /* LOGGER_H */
