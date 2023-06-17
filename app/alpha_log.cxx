#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include "alpha_func.h"
#include "alpha_macro.h"

alpha_log_t alpha_log;

static u_char err_levels[][20] = {
    {"stderr"},
    {"emerg"},
    {"alert"},
    {"crit"},
    {"error"},
    {"warn"},
    {"notice"},
    {"info"},
    {"debug"}};

void log_init(char *display, u_char *log_name, int log_level)
{
    log_name = (u_char *)"logs/error.log";
    if (log_name == nullptr)
    {
        log_name = (u_char *)ALPHA_ERROR_LOG_PATH;
    }

    alpha_log.display = display;
    alpha_log.log_level = log_level;
    alpha_log.fd = open((const char *)log_name, O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (alpha_log.fd == -1)
    {
        log_stderr(errno, "[alert] could not open error log file: open() \"%s\" failed", log_name);
        alpha_log.fd = STDERR_FILENO;
    }

    return;
}

void log_stderr(int err, const char *fmt, ...)
{
    va_list args;
    u_char errstr[ALPHA_MAX_ERROR_STR + 1];
    u_char *p, *last;

    memset(errstr, 0, sizeof(errstr));
    last = errstr + ALPHA_MAX_ERROR_STR;

    p = Alpha_Cpy_Mem(errstr, alpha_log.display, 11);

    va_start(args, fmt);
    p = vslprintf(p, last, fmt, args);
    va_end(args);

    if (err)
    {
        p = log_errno(p, last, err);
    }

    if (p >= (last - 1))
    {
        p = (last - 1) - 1;
    }
    *p++ = '\n';

    write(STDERR_FILENO, errstr, p - errstr);
    printf("33333");
    return;
}

u_char *log_errno(u_char *buffer, u_char *last, int err)
{
    char *errorInfo = strerror(err);
    size_t len = strlen(errorInfo);

    char leftstr[10] = {0};
    sprintf(leftstr, " (%d: ", err);
    size_t leftlen = strlen(leftstr);

    char rightstr[] = ") ";
    size_t rightlen = strlen(rightstr);

    size_t extralen = leftlen + rightlen;
    if ((buffer + len + extralen) < last)
    {
        buffer = Alpha_Cpy_Mem(buffer, leftstr, leftlen);
        buffer = Alpha_Cpy_Mem(buffer, errorInfo, len);
        buffer = Alpha_Cpy_Mem(buffer, rightstr, rightlen);
    }
    return buffer;
}

void log_error_core(int level, int err, const char *fmt, ...)
{
    u_char *last;
    u_char errstr[ALPHA_MAX_ERROR_STR + 1];

    memset(errstr, 0, sizeof(errstr));
    last = errstr + ALPHA_MAX_ERROR_STR;

    struct timeval tv;
    struct tm tm;
    time_t sec;
    u_char *p;
    va_list args;

    memset(&tv, 0, sizeof(timeval));
    memset(&tm, 0, sizeof(tm));

    gettimeofday(&tv, NULL);

    sec = tv.tv_sec;
    localtime_r(&sec, &tm);
    tm.tm_mon++;
    tm.tm_year += 1900;

    u_char strcurrtime[40] = {0};
    slprintf(strcurrtime, (u_char *)-1,
             "%4d/%02d/%02d %02d:%02d:%02d",
             tm.tm_year, tm.tm_mon,
             tm.tm_mday, tm.tm_hour,
             tm.tm_min, tm.tm_sec);

    p = Alpha_Cpy_Mem(errstr, strcurrtime, strlen((const char *)strcurrtime));
    p = slprintf(p, last, " [%s] ", err_levels[level]);
    p = slprintf(p, last, "%P: ", getpid());

    va_start(args, fmt);
    p = vslprintf(p, last, fmt, args);
    va_end(args);

    if (err)
    {
        p = log_errno(p, last, err);
    }

    if (p >= (last - 1))
    {
        p = (last - 1) - 1;
    }
    *p++ = '\n';

    ssize_t n;
    while (1)
    {
        if (level > alpha_log.log_level)
        {
            break;
        }

        n = write(alpha_log.fd, errstr, p - errstr);
        if (n == -1)
        {
            if (errno == ENOSPC)
            {
                // maybe no space.
            }
            else
            {
                if (alpha_log.fd != STDERR_FILENO)
                {
                    n = write(STDERR_FILENO, errstr, p - errstr);
                }
            }
        }
        break;
    }
    return;
}