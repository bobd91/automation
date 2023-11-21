#include <stdbool.h>
#include "pico/async_context.h"
#include "hardware/watchdog.h"
#include "event.h"
#include "async.h"


static struct async_event {
    async_event_handler handler,
    char *arg
};

static typedef struct async_event async_event;

static const queue_size = 10;
static async_event event_queue[queue_size];
static int queue_head = 0;
static int queue_tail = -1;

static async_when_pending_worker_t event_worker = { .do_work = process_event }

static void init(void) {
    static done; 
    if(done) return;

    async_add_when_pending_worker(event_worker);
    done = true;
}

static void enqueue_event(async_event_handler handler, char *arg) {
    queue_tail++;
    if(queue_tail >= queue_size) {
        // If the queue has this many events then something has gone very wrong!
        // Remove all events and enqueue an error event
        queue_head = 0;
        queue_tail = 0;
        event_queue[0].handler = ASYNC_EVENT_ERROR;
        event_queue[0].arg = async_event_copy_arg("Async Event Queue overflow");
    } else {
        event_queue[queue_tail].handler = handler;
        event_queue[queue_tail].arg = arg;
    }
}

static bool queue_is_empty() {
    return queue_tail < queue_head;
}
static async_event *dequeue_event() {
    if(queue_is_empty()) return NULL;

    async_event *event = event_queue + queue_head++;
    if(queue_is_empty()) {
        queue_head = 0;
        queue_tail = -1;
    }
    return event;
}

static void process_event(async_when_pending_worker_t worker) {
    async_event *event = dequeue_event();
    if(event) {
        *event->handler(event->arg);
        if(event->arg) {
            free(event->arg);
        }
        if(!queue_is_empty()) {
            async_set_work_pending(event_worker);
        }
    }
}

void async_event(async_event_handler handler, char *arg) {
    init();
    enqueue_event(handler, arg);
    async_set_work_pending(event_worker);
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