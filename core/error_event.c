#include <stdio.h>
#include "error_event.h"
#include "pico/time.h"
#include "hardware/watchdog.h"

static error_event_info *the_error;
static bool print_error(error_event_id event_id, int err, char *file, int line);

static const int MAX_LISTENERS = 10;
static error_event_listener listeners[MAX_LISTENERS] = { print_error };
static int next_listener = 1;

static const uint32_t error_sleep_ms = 5000;
static const uint32_t error_reboot_ms = 60 * error_sleep_ms;

static bool is_async_init;

static bool print_error(error_event_info *event_info) {
    printf("Error event id=%d, err=%d, in file %s, line %d", 
        event_info->event_id,
        event_info->err,
        event_info->file,
        event_info-> line);
    return false;
}

static void sleep_or_reboot(void) {
    if(error_reboot_ms > error_sleep_ms) {
        sleep_ms(error_sleep_ms);
        error_reboot_ms -= error_sleep_ms;
    } else {
        watchdog_enable(0, true);
    }
}

void error_event_async_init(void) {
    is_async_init = true;
}

// If error_event_async_init has not been called then handle error now, never return
// Otherwise log the error and return
void error_event_log(error_event_id event_id, int err, char *file, int line) {
    static error_event_info event_info;
    if(the_error) return;  // only capture first error
    event_info = {
        .event_id = event_id,
        .err = err,
        .file = file,
        .line = line
    };
    the_error = &event_info;

    if(!is_async_init) error_event_handle();
}

// If there is an error to handle then this function never returns
void error_event_handle(void) {
    if(!the_error) return;

    while(true) {
        for(int i = 0 ; i < MAX_LISTENERS ; i++) {
            error_event_listener listener = listeners[i];
            if(listener) {
                bool repeat = *listener(&the_error);
                if(!repeat) listeners[i] = NULL;
            }
        }
        sleep_or_reboot();
    }
}

void error_event_listen(error_event_listener listener) {
    error_if(next_listener == MAX_LISTENERS,, ERROR_EVENT_MAX_LISTENERS, 0);
    listeners[next_listener++] = listener;
}

