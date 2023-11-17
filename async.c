#include "async_poll.h"
#include "halt.h"
#include "pico/async_context.h"

static async_context_poll_t poll_context;
static async_context_t *context;

static void init(void) {
    if(context) return;

    halt_if(!async_context_poll_init_with_defaults(&poll_context));

    context = &poll_context.core;
}

async_context_t *async_get_context(void) {
    init();
    return context;
}

void async_add_when_pending_worker(async_when_pending_worker_t *worker) {
    halt_if(!async_context_add_when_pending_worker(async_get_context(), worker));
}

void async_set_work_pending(async_when_pending_worker_t *worker) {
    async_context_set_work_pending(async_get_context(), worker);
}