#include "logger.h"
#include <stdlib.h>
#include <time.h>

static FILE *log_file = NULL;

void set_log_file(const char *filename) {
    log_file = fopen(filename, "a");
    if (!log_file) {
        fprintf(stderr, "error: Could not open log file: %s\n", filename);
        abort();
    }
}

void log_message(LogLevel level, const char *format, ...) {
    if (!log_file) {
        fprintf(stderr, "error: Log file not initialized.\n");
        exit(EXIT_FAILURE);
    }

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    fprintf(log_file, "[%02d-%02d-%04d %02d:%02d:%02d] ", 
            t->tm_mday, t->tm_mon + 1, t->tm_year + 1900, 
            t->tm_hour, t->tm_min, t->tm_sec);

    switch (level) {
        case LOG_LEVEL_INFO:
            fprintf(log_file, "[INFO] ");
            break;
        case LOG_LEVEL_WARNING:
            fprintf(log_file, "[WARNING] ");
            break;
        case LOG_LEVEL_ERROR:
            fprintf(log_file, "[ERROR] ");
            break;
    }

    va_list args;
    va_start(args, format);
    vfprintf(log_file, format, args);
    va_end(args);

    fprintf(log_file, "\n");

    fflush(log_file);
}

void close_log_file(void) {
    if (log_file) {
        fclose(log_file);
        log_file = NULL;
    }
}
