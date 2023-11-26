#include <stdio.h>
#include "error_event.h"
#include "async_event.h"

static const int MAX_LISTENERS = 10;
static error_event_listener listeners[MAX_LISTENERS];
static int next_listener = 0;

void error_event_send(error_event_id id, int extra, char *file, int line) {
    printf("Error event id=%d, extra=%d, in file %s, line %d", id, extra, file, line);
    for(int i = 0 ; i < next_listener ; i++) {
        *listeners[i](id, extra, file, line);
    }
}

void error_event_listen(error_event_listener listener) {
    error_if(next_listener == MAX_LISTENERS,, ERROR_EVENT_MAX_LISTENERS, 0);
    listeners[next_listener++] = listener;
}