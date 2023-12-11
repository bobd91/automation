#ifndef MOCK_H__
#define MOCK_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

typedef unsigned int uint;
typedef char s8_t;
typedef unsigned char u8_t;
typedef unsigned short u16_t;

#ifndef MOCK_TRACE_FUNCTION
#define MOCK_TRACE_FUNCTION printf
#endif

#define MOCK_TRACE(fmt, ...) MOCK_TRACE_FUNCTION("%s(" fmt ")\n", __func__ __VA_OPT__(,) __VA_ARGS__)

#endif