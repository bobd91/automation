#include "wifi.h"
#include "cyw43.h"
#include "pico/cyw43_arch.h"

static repeating_timer_t link_timer;

int wifi_connect(char *ssid, char *password) {
    int err = cyw43_arch_init();
    error_if(err, err, "Failed to initialise wifi: %d", err);

    cyw43_arch_enable_sta_mode();

    err = cyw43_arch_wifi_connect_async(ssid, password, CYW43_AUTH_WPA2_AES_PSK);
    error_if(err, err, "Failed to connect to wifi: %d", err);

    error_if(!add_repeating_timer_ms(1000, check_link, NULL, &link_timer),, "Failed to add repeating timer");
} 

static bool check_link(repeating_timer_t *timer) {
    int status = cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA);

    error_if(status < 0,false,"Wifi connection failed: %s", cyw43_tcpip_link_status_name(status));

    if(status == CYM43_LINK_UP) {
        async_event_send(ASYNC_EVENT_WIFI_CONNECTED, NULL);
        return false;
    } else {
        return true;
    }
}