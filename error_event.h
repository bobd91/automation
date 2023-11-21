#ifndef _ERROR_EVENT_H
#define _ERROR_EVENT_H

#define error_if(test, ret, fmt, ...) \
do { \
  if(test) { \
    typeof (ret) ret_ = (ret); \
    error_event((fmt) __VA_OPT__(,) __VA_ARGS__, ret_); \
    return ret_; \
  } \
} while(0)

void error_event(const char *fmt, ...);

#endif