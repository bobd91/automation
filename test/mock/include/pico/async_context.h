#ifndef PICO_ASYNC_CONTEXT_H__
#define PICO_ASYNC_CONTEXT_H__

#include "mock/trace.h"

typedef struct mock_async_context async_context_t;
typedef struct mock_async_when_pending_worker async_when_pending_worker_t;
typedef void  (* mock_async_context_no_work_function)(void);

struct mock_async_context {
    bool is_init;
    bool work_pending;
    int worker_count;
    async_when_pending_worker_t *workers;
};

struct mock_async_when_pending_worker {
    async_when_pending_worker_t *next;
    async_when_pending_worker_t *next_pending;
    async_context_t *context;
    void (*do_work)(async_context_t *context, async_when_pending_worker_t *worker);
    bool work_pending;
    int number;
};

bool async_context_add_when_pending_worker(async_context_t *context, async_when_pending_worker_t *worker);
void async_context_set_work_pending(async_context_t *context, async_when_pending_worker_t *worker);
void async_context_poll(async_context_t *context);
void async_context_wait_for_work_ms(async_context_t *context, uint32_t ms);

void mock_async_context_no_work_function_set(mock_async_context_no_work_function function);

#endif