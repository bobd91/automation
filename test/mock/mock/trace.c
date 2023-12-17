#include "mock/trace.h"
#include "mock/trace_match.h"
#include <stdio.h>
#include <stdarg.h>

#define BUFFER_SIZE 255 // Any lines longer than this deserve to be truncated
static char buffer[1 + BUFFER_SIZE];
static mock_trace_match *matches;

static mock_trace_function trace_function;

void mock_trace_print(const char *func, const char *format, ...) {
    va_list va_print;

    va_start(va_print, format);
    vsnprintf(buffer, BUFFER_SIZE, format, va_print);
    va_end(va_print);

    fputs(func, stdout);
    putc('(', stdout);
    fputs(buffer, stdout);
    puts(")");
    
    if(trace_function) (*trace_function)(func, buffer);
}

void mock_trace_function_set(mock_trace_function function) {
    trace_function = function;
}

void mock_trace_match_trace(const char *func, const char *trace) {
    mock_trace_match *match = matches;
    while(match) {
        int fail = regexec(match->re, trace, match->nmatch, match->matchptr, match->eflags);
        if(!fail) {
            (*match->function)(trace, match);
            return;
        }
        match = match->next;
    }
}

void mock_trace_match_add(mock_trace_match *match) {
    if(!matches) {
        mock_trace_function_set(mock_trace_match_trace);
    }
    match->next = matches;
    matches = match;
}