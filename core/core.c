#include "core.h"
#include "error_event.h"
#include "async.h"
#include "async_event.h"
#include "wifi.h"
#include "server.h"
#include "control_panel"
#include "switch_action"

static void core_init(void) {
    async_init();
    async_event_init();
    control_panel_init();
    switch_action_init();
    wifi_init(WIFI_SSID, WIFI_PASSWORD);
    server_init(SERVER_ADDRESS, SERVER_PORT);

    // Initialisation complete, we now handle errors asynchronously
    error_event_async_init();
}

void core_run(void) {
    core_init();

    async_event_send(ASYNC_EVENT_START);

    while(true) {
        async_poll();
        error_event_handle();
        async_wait_for_work();
    }
}