#include "wifi.h"
#include "pico/cyw43_arch.h"

int wifi_connect(char *ssid, char *password, uint32_t timeout_ms) {
    int err = cyw43_arch_init();
    if(err) return err;
    cyw43_arch_enable_sta_mode();
    return cyw43_arch_wifi_connect_timeout_ms(
        ssid,
        password,
        CYW43_AUTH_WPA2_AES_PSK,
        timeout_ms);
}