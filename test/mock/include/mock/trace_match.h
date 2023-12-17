#ifndef MOCK_TRACE_MATCH_H__
#define MOCK_TRACE_MATCH_H__

#include <regex.h>

typedef void (* mock_trace_function)(const char *func, const char *trace);
typedef struct mock_trace_match_info mock_trace_match;
typedef void (* mock_trace_match_function)(const char *trace, mock_trace_match *match);

struct mock_trace_match_info {
    mock_trace_match *next;
    regex_t *re;
    size_t nmatch;
    regmatch_t *matchptr;
    int eflags;
    mock_trace_match_function function;
    void *user_info;
};

void mock_trace_function_set(mock_trace_function function);
void mock_trace_match_add(mock_trace_match *match);
void mock_trace_match_trace(const char *func, const char *trace);

#endif