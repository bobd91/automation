#include "app.h"
#include "async.h"
#include "async_event.h"
#include "wifi.h"
#include "server.h"
#include "control_panel"
#include "switch_action"

static void app_init(void) {
    async_init();
    async_event_init();
    control_panel_init();
    switch_action_init();
    wifi_init(WIFI_SSID, WIFI_PASSWORD);
    server_init(SERVER_ADDRESS, SERVER_PORT);
}

void app_run(void) {
    app_init();

    async_event_send(ASYNC_EVENT_START);

    async_poll_loop();
}