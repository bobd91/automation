#include "async_event_monitor.h"
#include "async_poll.h"
#include "pico/async_context.h"

static async_when_pending_worker_t async_worker = { .do_work = process_async_event }
static app_event_monitor_t app_event_monitor;

void async_event_monitor_init(app_event_monitor_t monitor) {
    app_event_monitor = monitor;
    async_poll_add_pending_worker(&async_worker);
}

void async_event_monitor(int event) {
    async_worker.user_data = (void *)event
    async_poll_set_work_pending(&async_worker);
}

static void process_async_event(async_context_t *context, async_when_pending_worker_t *worker) {
    *app_event_monitor((int)worker->user_data);
}