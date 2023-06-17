#ifndef __ALPHA_FUNC_H__
#define __ALPHA_FUNC_H__

#include <sys/types.h>
#include <stdarg.h>

typedef struct
{
    char *display;
    int log_level;
    int fd;
} alpha_log_t;

extern alpha_log_t alpha_log;

// Related to log.
void log_init(char *display, u_char *log_name, int log_level);
void log_stderr(int err, const char *fmt, ...);
void log_error_core(int level, int err, const char *fmt, ...);

u_char *log_errno(u_char *buffer, u_char *last, int err);
u_char *snprintf(u_char *buffer, size_t max, const char *fmt, ...);
u_char *slprintf(u_char *buffer, u_char *last, const char *fmt, ...);
u_char *vslprintf(u_char *buffer, u_char *last, const char *fmt, va_list args);

#endif