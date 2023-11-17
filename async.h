#ifndef _ASYNC_POLL_H
#define _ASYNC_POLL_H

#include "pico/async_context.h"

async_context_t *async_get_context(void);
void async_add_when_pending_worker(async_when_pending_worker_t *worker);
void async_set_work_pending(async_when_pending_worker_t *worker);

#endif