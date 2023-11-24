#ifndef _ERROR_EVENT_H
#define _ERROR_EVENT_H

#define error_if(test, ret, fmt, ...) \
do { \
  if(test) { \
    error_event((fmt) __VA_OPT__(,) __VA_ARGS___); \
    return ret; \
  } \
} while(0)

void error_event(const char *fmt, ...);

#endif