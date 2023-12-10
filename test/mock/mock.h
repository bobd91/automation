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

#define MOCK_TRACE(fmt, ...) printf("%s(" #fmt ")\n", __func__ __VA_OPT__(,) __VA_ARGS__)

#endif