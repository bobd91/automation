#include <stdio.h>
#include <stdarg.h>
#include "error_event.h"
#include "async_event.h"

#define BUF_SIZE 1024; // way too big
static char *err = "Failed to format error message";

void error_event(const char *fmt, ...) {
    va_list argp;
    va_start(argp, fmt);    
    char *buf = malloc(BUF_SIZE);

    if(buf) {
        int size = vsnprintf(buf, BUF_SIZE, fmt, argp);

        if(!size) {
            strncpy(buf, err, BUF_SIZE);
        }
    }

    va_end(argp);

    async_event_send_arg(ASYNC_EVENT_ERROR, buf);
}