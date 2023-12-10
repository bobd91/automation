#include "pico/async_context_poll.h"

static async_context_t context;

bool async_context_poll_init_with_defaults(async_context_poll_t *self) {
    assert(self);
    MOCK_TRACE("async_context_poll_t");

    self->core = context;
    return true;
}