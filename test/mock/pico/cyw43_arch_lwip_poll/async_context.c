#include "pico/async_context.h"
#include "pico/time.h"
#include "lwip/tcp.h"

static mock_async_context_no_work_function no_work_function;

static bool workers_has_worker(async_when_pending_worker_t *workers, async_when_pending_worker_t *worker) {
    while(workers && workers != worker) {
        workers = workers->next;
    }
    return workers == worker;
}

static void do_pending_work(async_when_pending_worker_t *workers) {
    async_when_pending_worker_t *worker = workers;
    async_when_pending_worker_t *pending = NULL;

    while(worker) {
        if(worker->work_pending) {
            worker->next_pending = pending;
            pending = worker;
            worker->work_pending = false;
        }
        worker = worker->next;
    }

    while(pending) {
        (*pending->do_work)(pending->context, pending);
        pending = pending->next_pending;
    }
}

static void mock_sleep_ms(uint32_t ms) {
    usleep(1000 * ms);
}

bool async_context_add_when_pending_worker(async_context_t *context, async_when_pending_worker_t *worker) {
    assert(context);
    assert(context->is_init);
    assert(worker);
    assert(!worker->context);
    assert(!worker->number);
    worker->number = ++context->worker_count;
    MOCK_TRACE("context, { .number = %d }", worker->number);

    worker->context = context;
    worker->next = context->workers;
    context->workers = worker;

    return true;

}

void async_context_set_work_pending(async_context_t *context, async_when_pending_worker_t *worker) {
    assert(context);
    assert(context->is_init);
    assert(worker);
    assert(context == worker->context);
    assert(workers_has_worker(context->workers, worker));
    MOCK_TRACE("context, { .number = %d }", worker->number);

    context->work_pending = true;
    worker->work_pending = true;
 }

void async_context_poll(async_context_t *context) {
    assert(context);
    assert(context->is_init);
    MOCK_TRACE("context");

    context->work_pending = false;
    do_pending_work(context->workers);

    mock_tcp_call_pending_callbacks();
}

void async_context_wait_for_work_ms(async_context_t *context, uint32_t ms) {
    assert(context);
    assert(context->is_init);
    MOCK_TRACE("context, %u", ms);

    uint32_t time_left_ms = ms;
    while(time_left_ms && !context->work_pending) {
        if(no_work_function) no_work_function();
        uint32_t wait_ms = mock_time_run_pending_timers();
        wait_ms = wait_ms ? wait_ms : time_left_ms;
        mock_sleep_ms(wait_ms);
        time_left_ms = time_left_ms > wait_ms ? time_left_ms - wait_ms : 0;
    }
}

void mock_async_context_no_work_function_set(mock_async_context_no_work_function function) {
    no_work_function = function;
}

