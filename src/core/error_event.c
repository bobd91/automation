#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "error_event.h"
#include "pico/time.h"
#include "hardware/watchdog.h"

static error_event_info *the_error;
static bool print_error(error_event_info *event_info);

#define MAX_LISTENERS 10
static error_event_listener listeners[MAX_LISTENERS] = { print_error };
static int next_listener = 1;

static const uint32_t error_sleep_ms = ERROR_EVENT_SLEEP_MS;
static uint32_t error_reboot_ms = ERROR_EVENT_REBOOT_MS;

static bool is_async_init;

static bool print_error(error_event_info *event_info) {
    printf("Error event id=%d, err=%d, in file %s, line %d\n", 
        event_info->event_id,
        event_info->err,
        event_info->file,
        event_info->line);
    return false;
}

static void sleep_or_reboot(void) {
    if(error_reboot_ms > error_sleep_ms) {
        sleep_ms(error_sleep_ms);
        error_reboot_ms -= error_sleep_ms;
    } else {
        watchdog_enable(0, true);
        // should never get here
        exit(the_error->event_id);
    }
}

static error_event_listener call_listener(error_event_listener listener) {
    if(listener) {
        bool repeat = (*listener)(the_error);
        return repeat ? listener : NULL;
    }
    return NULL;
}

static void error_event_handle() {
    assert(the_error);

    while(true) {
        for(int i = 0 ; i < MAX_LISTENERS ; i++) {
            listeners[i] = call_listener(listeners[i]);
        }
        sleep_or_reboot();
    }
}

void error_event_listen(error_event_listener listener) {
    error_if(next_listener == MAX_LISTENERS,, ERROR_EVENT_MAX_LISTENERS, 0);
    listeners[next_listener++] = listener;
}

void error_event_async_init(void) {
    is_async_init = true;
}

// If error_event_async_init has not been called then handle error now, never return
// Otherwise log the error and return
void error_event_log(error_event_id event_id, int err, char *file, int line) {
    static error_event_info event_info;
    if(the_error) return;  // only capture first error
    event_info.event_id = event_id;
    event_info.err = err;
    event_info.file = file;
    event_info.line = line;
    the_error = &event_info;

    if(!is_async_init) error_event_handle();
}

// If there an error has been logged then this function never returns
void error_event_async_check(void) {
    if(the_error) error_event_handle();
}