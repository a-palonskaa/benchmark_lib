#include <time.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include "logger.h"
#include "define_colors.h"

static const char* LogMessageTypePrint(enum LogLevel level, bool color);
static void TimePrint(FILE *out);
static void AestheticizeString(const char *src, char *dst, size_t max_len);

const size_t MAXLINE = 100;

//----------------------------------------------------------------------------------------------

static logger_t* GetLogger() {
    static logger_t logger = {};
    return &logger;
}

void LoggerSetFile(FILE* out) {
    assert(out != nullptr);

    GetLogger()->file_out = out;

    if (setvbuf(GetLogger()->file_out, nullptr, _IONBF, 0)) {
        fprintf(stderr, "WARNING\n");
    }
}

void LoggerSetLevel(enum LogLevel level) {
    GetLogger()->min_level = level;
}

//----------------------------------------------------------------------------------------------

void Log(enum LogLevel status, const char* file, size_t line, const char* func, const char *fmt, ...) {
    assert(fmt != nullptr);

    if (GetLogger()->min_level > status) {
        return;
    }

    char dst[MAXLINE] = "";
    AestheticizeString(fmt, dst, MAXLINE);

    va_list args;
    va_start (args, fmt);

    bool color = GetLogger()->file_out == stderr || GetLogger()->file_out == stdout;
    fprintf(GetLogger()->file_out, "%s:%zu (%s)\n", file, line, func);
    fprintf(GetLogger()->file_out, "%s", LogMessageTypePrint (status, color));
    TimePrint(GetLogger()->file_out);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-nonliteral"
    vfprintf (GetLogger()->file_out, dst, args);
#pragma clang diagnostic pop

    va_end (args);
}

//----------------------------------------------------------------------------------------------

#define ADD_COLOR_(COLOR , str)           \
    do {                                  \
        if (color) {                      \
            return COLOR str COLOR_RESET; \
        }                                 \
        return str;                       \
    } while(0)

static const char* LogMessageTypePrint(const enum LogLevel level, const bool color) {

    switch (level) {
        case ERROR:
            ADD_COLOR_(COLOR_RED, "[ERROR] ");
        case WARNING:
            ADD_COLOR_(COLOR_YELLOW, "[WARNING] ");
        case INFO:
            ADD_COLOR_(COLOR_BLUE, "[INFO] ");
        case DEBUG:
            ADD_COLOR_(COLOR_GREEN, "[DEBUG] ");
        default:
            ADD_COLOR_(COLOR_RED, "!ERROR! ");
            break;
    }
}

#undef ADD_COLOR_

static void TimePrint(FILE *out) {
    assert(out != nullptr);

    time_t mytime = time(NULL);
    struct tm *time = localtime(&mytime);

    fprintf(out, "%02d.%02d.%d %02d:%02d:%02d ",
            time->tm_mday, time->tm_mon + 1, time->tm_year + 1900,
            time->tm_hour, time->tm_min,     time->tm_sec);
}

static void AestheticizeString(const char *src, char *dst, const size_t max_len) {
    assert(src != nullptr);
    assert(dst != nullptr);

    size_t len = strnlen(src, max_len);

    if (len == 0) {
        return;
    }

    size_t j = 0, i = 0;

    dst[j++] = '\n';
    dst[j++] = '\t';

    for(; i < len && j < max_len - 1; i++) {
        if (src[i] == '\n') {
            dst[j++] = '\n';
            dst[j++] = '\t';
        }
        else {
            dst[j++] = src[i];
        }
    }
    if (src[--i] == '\n') {
        dst[--j] = '\0';
    }
    else {
        dst[j] = '\0';
    }
}
