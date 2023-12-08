#ifndef MOCK_H__
#define MOCK_H__

#include <stdio.h>

#define MOCK_TRACE(fmt, ...) printf("%s(" #fmt ")\n", __func__ __VA_OPT__(,) __VA_ARGS__)