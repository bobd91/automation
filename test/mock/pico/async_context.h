#ifndef PICO_ASYNC_CONTEXT_H__
#define PICO_ASYNC_CONTEXT_H__

typedef struct mock_async_context async_context_t;
typedef struct mock_async_when_pending_worker async_when_pending_worker_t;
typedef struct void (*mock_worker)(async_context_t *context, async_when_pending_worker_t *worker);

struct async_context {
    void *dummy;
};

struct async_when_pending_worker {
    mock_worker do_work;
}

bool async_context_add_when_pending_worker(async_context_t *context, async_when_pending_worker_t *worker);
void async_context_set_work_pending(async_context_t *context, async_when_pending_worker_t *worker);
void async_context_poll(async_context_t *context);
void async_context_wait_for_work_ms(async_context_t *context, uint32_t ms);

#endif