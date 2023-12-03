#ifndef PICO_ASYNC_CONTEXT_POLL_H__
#define PICO_ASYNC_CONTEXT_POLL_H__

#include "pico/async_context.h"

typedef struct mock_async_context_poll async_context_poll_t;

struct async_context_poll {
    async_context_t core;
};

bool async_context_poll_init_with_defaults(async_context_poll_t *self);

#endif