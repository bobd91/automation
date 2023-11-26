#include <stdio.h>
#include "error_event.h"
#include "pico/time.h"
#include "hardware/watchdog.h"

static bool print_error(error_event_id event_id, int extra, char *file, int line);

static const int MAX_LISTENERS = 10;
static error_event_listener listeners[MAX_LISTENERS] = { print_error };
static int next_listener = 1;

static const uint32_t error_sleep_ms = 5000;
static const uint32_t error_reboot_ms = 60 * error_sleep_ms;


static bool print_error(error_event_id event_id, int extra, char *file, int line) {
    printf("Error event id=%d, extra=%d, in file %s, line %d", event_id, extra, file, line);
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

void error_event_send(error_event_id event_id, int extra, char *file, int line) {
    while(true) {
        for(int i = 0 ; i < MAX_LISTENERS ; i++) {
            error_event_listener listener = listeners[i];
            if(listener) {
                bool repeat = *listener(event_id, extra, file, line);
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

