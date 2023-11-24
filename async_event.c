#include <stdbool.h>
#include <memory.h>
#include "pico/async_context.h"
#include "async_event.h"
#include "async.h"


static struct event_info {
    async_event_id event_id,
    char *arg
};

static struct event_listener_info {
    async_event_listener_info *next,
    union {
        async_event_listener listener,
        async_event_listener_arg listener_args
    },
    bool with_arg
}

static typedef struct event_info event_info;

static const queue_size = 10;
static async_event_info event_queue[queue_size];
static int queue_head = 0;
static int queue_tail = -1;

static typedef struct event_listener_info event_lisener_info;

static event_listener_info *listeners[ASYNC_EVENT_MAX_ID];

static async_when_pending_worker_t event_worker = { .do_work = process_worker }

static void init(void) {
    static done; 
    if(done) return;

    async_add_when_pending_worker(event_worker);
    done = true;
}

static void enqueue_event(async_event_id event_id, char *arg) {
    if(queue_push(event_id, arg)) {
        async_set_work_pending(event_worker);
    } else {
        // If the queue has this many events then something has gone very wrong!
        // Remove all events and enqueue an error event
        queue_reset();
        enqueue_event(ASYNC_EVENT_ERROR, async_event_copy_arg("Async Event Queue overflow"));
    }
}

static void queue_reset() {
    queue_head = 0;
    queue_tail = -1;
}

static event_info *queue_push(async_event_id event_id, char *arg) {
    queue_tail++;
    if(queue_is_full()) return NULL;
    event_info *event = event_queue + queue_tail;
    event->event_id = id;
    event->arg = arg;
    return event;
}

static bool queue_is_full() {
    return queue_tail >= queue_size;    
}

static bool queue_is_empty() {
    return queue_tail < queue_head;
}

static event_info *queue_pop() {
    return event_queue + queue_head++;
}

static async_event *dequeue_event() {
    if(queue_is_empty()) return NULL;

    event_info *event = queue_pop();
    if(queue_is_empty()) {
        queue_head = 0;
        queue_tail = -1;
    }
    return event;
}

static void process_event(async_event *event) {
    listener_info *info = listeners[event->id];
    while(info) {
        if(info->with_arg)  *info->listener_arg(event->arg);
        else                *info->listener();

        info = info->next;
    }
}

static void process_worker(async_when_pending_worker_t worker) {
    async_event *event = dequeue_event();
    if(event) {
        process_event(event);
        if(event->arg) free(event->arg);
        if(!queue_is_empty()) async_set_work_pending(event_worker);
    }
}

static void add_listener(async_event_id event_id, async_event_listener listener, async_event_listener_arg listener_arg) {
    listener_info *info = malloc(sizeof(listener_info));
    error_if(!info,,"Malloc failed");
    if(listener) {
        info->listener = listener;
        info->with_arg = false;
    } else {
        info->listener_arg = listener_arg;
        info->with_arg = true;
    }
    info->next = listeners[event_id];
    listeners[event_id].info;
}

void async_event_send(async_event_id event_id, char *arg) {
    init();
    enqueue_event(event_id, arg);
}

void async_event_listen(async_event_id event_id, async_event_listener listener) {
    add_listener(event_id, listener, NULL);
}

void async_event_listen_arg(async_event_id event_id, async_event_listener_arg listener_arg) {
    add_listener(event_id, NULL, listener_arg);
}

void async_event_start(char *arg) {
    led_button_state_started();
    wifi_connect(WIFI_SSID, WIFI_PASSWORD, WIFI_TIMEOUT);
    server_connect(SERVER_ADDRESS, SERVER_PORT);
}

void async_event_connected(char *arg) {
    led_button_state_connected();
    server_send_identify();
}

void async_event_identify(char *arg) {
    led_button_state_identify();
}

void async_event_run(char *arg) {
    led_button_state_run();
}

void async_event_turned_off(char *arg) {
    server_send_turned_off();
}

void async_event_turned_on(char *arg) {
    server_send_turned_on();
}

void async_event_error(char *arg) {
    led_button_state_error();
    printf(event->arg);    
}

char *async_event_copy_arg(char *arg) {
    char *ret = NULL;
    if(arg) {
        ret = malloc(1 + strlen(arg));
        if(ret) {
            strcpy(ret, arg);
        }
    }
    return ret;
}