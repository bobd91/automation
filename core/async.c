#include "async_poll.h"
#include "error_event.h"
#include "pico/async_context.h"

static async_context_poll_t poll_context;
static async_context_t *context;
static const uint32_t poll_loop_ms = 1000 * 60;

void async_init(void) {
    error_if(!async_context_poll_init_with_defaults(&poll_context),, ERROR_EVENT_ASYNC_POLL_INIT, 0);
    context = &poll_context.core;
}

async_context_t *async_get_context(void) {
    return context;
}

void async_add_when_pending_worker(async_when_pending_worker_t *worker) {
    error_if(!async_context_add_when_pending_worker(context, worker),, ERROR_EVENT_ASYNC_ADD_WORKER, 0);
}

void async_set_work_pending(async_when_pending_worker_t *worker) {
    async_context_set_work_pending(context, worker);
}

void async_poll(void) {
    async_context_poll(context);
}

void async_wait_for_work(void) {
    async_context_wait_for_work_ms(context, poll_loop_ms);
}