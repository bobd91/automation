#ifndef MOCK_TRACE_H__
#define MOCK_TRACE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#define MOCK_TRACE(format, ...) mock_trace_print(__func__, format __VA_OPT__(,) __VA_ARGS__)

void mock_trace_print(const char *func, const char *format, ...);

#endif