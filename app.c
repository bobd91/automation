#include "app.h"


static void app_init(void) {
    async_init();
    async_event_init();
    wifi_init(WIFI_SSID, WIFI_PASSWORD);
    server_init(SERVER_ADDRESS, SERVER_PORT);
    control_panel_init();
    generic_switch_init();
}

void app_run(void) {
    app_init();

    async_event_send(ASYNC_EVENT_START);

    async_poll_loop();
}