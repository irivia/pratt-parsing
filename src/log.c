#include "log.h"
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define FAIL(...) do { fprintf(stderr, __VA_ARGS__); return false; } while (0)

LoggingInfo log_create(const char *path, FILE *file, double flush_interval)
{
    if (!path) {
        fprintf(stderr, "Error 'log_create' : Empty file path\n");
        return (LoggingInfo){0};
    }

    LoggingInfo info = {
        .path = path,
        .file = file,
        .flush_interval = flush_interval > 0.f ? flush_interval : 0.25,
        .last_flushed = time(0),
    };

    return info;
}

bool log_info(LoggingInfo *li, const char *message, ...)
{
    if (!li)
        FAIL("Error: Login Info is NULL\n");
    if (!li->path)
        FAIL("Error 'log_info' : Empty file path\n");

    va_list args;
    enum { t_len = 128 };
    char t_buff[t_len];
    time_t timer;
    time(&timer);
    struct tm t = {0};

   if (localtime_r(&timer, &t) != &t)
        FAIL("Failed to get local time\n");

    size_t time_written = strftime(t_buff, t_len, "-- %Z %d-%m-%Y %I:%M:%S %p --\n", &t);

    if (!time_written)
        FAIL("Failed to parse local time\n");

    if (!li->file) {
        if (!(li->file = fopen(li->path, "wb")))
            FAIL("Failed to open '%s'\n", li->path);
    }

    fprintf(li->file, "%s", t_buff);

    va_start(args, message);
    vfprintf(li->file, message, args);
    va_end(args);
    fprintf(li->file, "\n-------------------------------------------------\n");

    double elapsed = difftime(time(0), li->last_flushed);

    if (elapsed >= li->flush_interval) {
        li->last_flushed = time(0);
        if (fflush(li->file) != 0)
            FAIL("Failed to flush file '%s'\n", li->path);
    }

    return true;
}
