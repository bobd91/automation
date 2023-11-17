#include <stdbool.h>
#include "pico/async_context.h"
#include "hardware/watchdog.h"
#include "async.h"

struct event_s {
    event_handler_t handler,
    void *args
};

typedef struct event_s event_t;
typedef event_t (* event_handler_t)(event_t *event);

event_t EVENT_START = { .handler = start };
event_t EVENT_CONNECTED = { .handler = connected };
event_t EVENT_IDENTIFY = { .handler = identify };
event_t EVENT_RUN = { .handler = run };
event_t EVENT_TURNED_OFF = { .handler = turned_off };
event_t EVENT_TURNED_ON = { .handler = turned_on };
event_t EVENT_ERROR = { .handler = error };

// TODO: move to switch specific code event_t EVENT_TURN_ON = { .handler = turn_on };
// TODO: move to switch specific code event_t EVENT_TURN_OFF = { .handler = turn_off };

static async_when_pending_worker_t event_worker = { .do_work = process_event }

static void init() {
    static done; 
    if(done) return;

    async_add_when_pending_worker(event_worker);
    done = true;
}

void event_async(event_t event) {
    event_worker.user_data = &event;
    async_set_work_pending(event_worker);
}

static void process_event(async_when_pending_worker_t worker) {
    event_t *event = worker->user_data;
    event_t *next_event = *event->handler(event);
    if(next_event) event_async(next_event);
}

static event_t *start(event_t event) {
    led_button_state_started();

    int wifi_err = wifi_connect(WIFI_SSID, WIFI_PASSWORD, WIFI_TIMEOUT);
    if(wifi_err) return error_event("Failed to connect to wifi", wifi_error);


    int port = atoi(SERVER_PORT);
    if(!port) return error_event("Invalid port number", SERVER_PORT);
    err_t tcp_err = server_connect(SERVER_ADDRESS, PORT);
    if(tcp_err) return error_event("Failed to connect to server", tcp_err);

    return NULL;
} 

static event_t *connected(event_t *event) {
    led_button_state_connected();

    server_send(/*TODO: command name and pico_get_unique_board_id_string*/); 

    return NULL;
}

static event_t *identify(event_t *event) {
    led_button_state_identify();

    return NULL;
}

static event_t *run(event_t *event) {
    led_button_state_run();

    return NULL;
}

static event_t *turned_off(event_t *event) {
    server_send(/* TODO: command name*/);

    return NULL;
}

static event_t *turned_on(event_t *event) {
    server_send(/* TODO: command name */);

    return NULL;
}

static event_t *error(event_t *event) {
    led_button_state_error();

    printf(event->args);    

    return NULL;
}