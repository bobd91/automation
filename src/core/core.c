#include "core.h"
#include "error_event.h"
#include "async.h"
#include "async_event.h"
#include "wifi.h"
#include "server.h"
#include "control_panel.h"
#include "switch_action.h"
#include "pico/stdio.h"

static void core_init(void) {
    stdio_init_all();
    async_init();
    async_event_init();
    control_panel_init();
    switch_action_init();
    wifi_init(WIFI_SSID, WIFI_PASSWORD);
    server_init(SERVER_IP, SERVER_PORT);
}

void core_run(void) {
    // Some components need to set up before we tell everyone to start
    core_init();

    // Init is on main thread so errors can be handled directly
    // Now errors could happen in ISRs and are handled async
    // (log them when they happen, handle them in the main loop)
    error_event_async_init();

    // Tell everyone to start
    async_event_send(ASYNC_EVENT_START);

    while(true) {
        async_poll();
        error_event_async_check();
        async_wait_for_work();
    }
}