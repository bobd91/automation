#include "async_event.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "pico/async_context.h"
#include "async.h"
#include "error_event.h"

typedef struct event_info event_info;
struct event_info {
    async_event_id event_id;
    char *arg;
};

typedef struct event_listener_info event_listener_info;
struct event_listener_info {
    event_listener_info *next;
    union {
        async_event_listener listener;
        async_event_listener_arg listener_arg;
    };
    bool with_arg;
};

// It is unlikely that there will be multiple events in the queue
// If it reaches 10 then we have a problem
#define QUEUE_SIZE 10
static event_info event_queue[QUEUE_SIZE];
static int queue_head = 0;
static int queue_tail = -1;

static event_listener_info *listeners[ASYNC_EVENT_MAX_ID];

static void process_worker(async_context_t *context, async_when_pending_worker_t *worker);
static async_when_pending_worker_t event_worker = { .do_work = process_worker };

static bool queue_is_full() {
    return QUEUE_SIZE > queue_tail + 1;
}

static bool queue_is_empty() {
    return queue_tail < queue_head;
}

static void queue_push(async_event_id event_id, char *arg) {
    assert(!queue_is_full());

    event_info *event = event_queue + ++queue_tail;
    event->event_id = event_id;
    event->arg = arg;
}

static event_info *queue_pop() {
    assert(!queue_is_empty());

    event_info *event = event_queue + queue_head++;
    if(queue_is_empty()) {
        queue_head = 0;
        queue_tail = -1;
    }
    return event;
}

static void enqueue_event(async_event_id event_id, char *arg) {
    error_if(queue_is_full(),, ERROR_EVENT_ASYNC_QUEUE_FULL, 0);
    queue_push(event_id, arg);
    async_set_work_pending(&event_worker);
}

static event_info *dequeue_event() {
    return queue_is_empty() ? NULL : queue_pop();
}

static void process_event(event_info *event) {
    event_listener_info *info = listeners[event->event_id];
    while(info) {
        if(info->with_arg)  (*info->listener_arg)(event->arg);
        else                (*info->listener)();

        info = info->next;
    }
}

static void process_worker(async_context_t *context, async_when_pending_worker_t *worker) {
    event_info *event = dequeue_event();
    if(event) {
        process_event(event);
        if(event->arg) free(event->arg);
        if(!queue_is_empty()) async_set_work_pending(&event_worker);
    }
}

static void add_listener(async_event_id event_id, async_event_listener listener, async_event_listener_arg listener_arg) {
    assert(listener || listener_arg);

    // This memory is never freed, there is no listener removal
    event_listener_info *info = malloc(sizeof(event_listener_info));
    error_if(!info,, ERROR_EVENT_NO_MEMORY, 0);
    if(listener) {
        info->listener = listener;
        info->with_arg = false;
    } else {
        info->listener_arg = listener_arg;
        info->with_arg = true;
    }
    info->next = listeners[event_id];
    listeners[event_id] = info;
}

void async_event_init(void) {
    async_add_when_pending_worker(&event_worker);
}

void async_event_send_arg(async_event_id event_id, char *arg) {
    enqueue_event(event_id, arg);
}

void async_event_send(async_event_id event_id) {
    async_event_send_arg(event_id, NULL);
}

void async_event_listen(async_event_id event_id, async_event_listener listener) {
    add_listener(event_id, listener, NULL);
}

void async_event_listen_arg(async_event_id event_id, async_event_listener_arg listener_arg) {
    add_listener(event_id, NULL, listener_arg);
}

char *async_event_format_arg(const char *format, ...) {
    if(!format) return NULL;

    static const size_t max_arg_len = 256;
    char *ret = malloc(max_arg_len);
    if(!ret) return NULL;

    va_list args;
    va_start(args, format);
    int count = vsnprintf(ret, max_arg_len, format, args);
    va_end(args);

    if(count <= 0 || count >= max_arg_len) {
        free(ret);
        return NULL;
    }

    // allocated memory will be freed by process_worker
    return ret;
}