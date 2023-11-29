#ifndef _ASYNC_H
#define _ASYNC_H

#include "pico/async_context.h"

bool async_init(void);
async_context_t *async_get_context(void);
void async_add_when_pending_worker(async_when_pending_worker_t *worker);
void async_set_work_pending(async_when_pending_worker_t *worker);
void async_poll(void);
void async_wait_for_work(void);

#endif