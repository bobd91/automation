#include "core.h"
#include "error_event.h"
#include "async.h"
#include "async_event.h"
#include "wifi.h"
#include "server.h"
#include "control_panel"
#include "switch_action"

static bool core_init(void) {
    return async_init()
    && async_event_init()
    && control_panel_init()
    && switch_action_init()
    && wifi_init(WIFI_SSID, WIFI_PASSWORD)
    && server_init(SERVER_ADDRESS, SERVER_PORT)
}

void core_run(void) {
    if(core_init()) {
        async_event_send(ASYNC_EVENT_START);
    }

    while(true) {
        error_event_handle();
        async_poll();
        async_wait_for_work();
    }
}